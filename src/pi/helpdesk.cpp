#include "helpdesk.h"

// --- try to avoid using this function outside this header
// --- function naming is confusing, state actually "not"
static auto	Get_HelpdeskTicketID_By_UserID_And_NotState_sqlquery(const string &id, const string &state)
{
	return (
			"SELECT `helpdesk_ticket_history_last_case_state_view`.`helpdesk_ticket_id` "
				"FROM `helpdesk_ticket_history_last_helpdesk_user_update_view` "
				"INNER JOIN `helpdesk_ticket_history_last_case_state_view` "
				"ON `helpdesk_ticket_history_last_case_state_view`.`helpdesk_ticket_id`=`helpdesk_ticket_history_last_helpdesk_user_update_view`.`helpdesk_ticket_id` "
				"WHERE "
				"`helpdesk_ticket_history_last_helpdesk_user_update_view`.`user_id` IN (" + id + ") "
				"AND "
				"`helpdesk_ticket_history_last_case_state_view`.`state`!=\"" + state + "\""
		);
}

static auto isUserAllowedToChangeTicket(string ticket_id, string user_id, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start (ticket_id:" + ticket_id + ", user_id:" + user_id + ")");

	auto	result	 = ""s;

	if((db->Query("SELECT `id` FROM `users` WHERE "
					"(`id`=" + quoted(user_id) + " AND `type`=" + quoted(HELPDESK_USER_ROLE) + ") "
					"OR "
					"(" + quoted(user_id) + "=(SELECT `customer_user_id` FROM `helpdesk_tickets` WHERE `id`=" + quoted(ticket_id) + ")) "
					"LIMIT 0,1;"))
		)
	{
	}
	else
	{
		result = gettext("you are not authorized");
		MESSAGE_DEBUG("", "", "user (" + user_id + ") not allowed to change ticket")
	}

	MESSAGE_DEBUG("", "", "finish (" + result + ")");

	return result;
}

static auto	Get_OpenHelpdeskTicketIDByUserID_sqlquery(const string &id)
{
	return Get_HelpdeskTicketID_By_UserID_And_NotState_sqlquery(id, "closed");
}

static auto amINewEngineer(string ticket_id, CMysql *db, CUser *user)
{
	MESSAGE_DEBUG("", "", "start");

	auto	result	= false;

	if(db->Query("SELECT `user_id` FROM `helpdesk_ticket_history` WHERE "
					"`helpdesk_ticket_id`=" + quoted(ticket_id) + " "
					"AND "
					"`user_id` IN (SELECT `id` FROM `users` WHERE `type`=\"helpdesk\") "
					"ORDER BY `eventTimestamp` DESC LIMIT 0,1;"))
	{
		if(db->Get(0, 0) == user->GetID())
		{
		}
		else
		{
			result = true;
		}
	}
	else
	{
		result = true;
	}

	MESSAGE_DEBUG("", "", "finish (" + to_string(result) + ")");

	return result;
}

static auto GetTicketSeverity(string ticket_id, CMysql *db)
{
	MESSAGE_DEBUG("", "", "start");

	auto result = ""s;

	if(db->Query("SELECT `severity` FROM `helpdesk_ticket_history` WHERE `helpdesk_ticket_id`=" + quoted(ticket_id) + "  ORDER BY `eventTimestamp` DESC LIMIT 0,1;"))
		result = db->Get(0, 0);

	MESSAGE_DEBUG("", "", "finish(" + result + ")");

	return result;
}

static auto	ReturnVectorFromTwoFields(string sql, CMysql *db)
{
	MESSAGE_DEBUG("", "", "start");

	vector<string>	result;
	auto 			affected = db->Query(sql);

	for(int i = 0; i < affected; ++i)
	{
		result.push_back(db->Get(i, 0) + db->Get(i, 1));
	}

	MESSAGE_DEBUG("", "", "finish(size is " + to_string(result.size()) + ")");

	return result;

}

static auto Get_ExistingTicket_NotificationRecipients_Email(string ticket_id, string severity, CMysql *db, CUser *user)
{
	return ReturnVectorFromTwoFields("SELECT DISTINCT(`login`),\"\" FROM `users` WHERE "
											"LENGTH(`email`) > 0 "
											"AND "
											"`helpdesk_subscription_S" + severity + "_email` = \"Y\" "
											"AND "
											"`id` IN (SELECT DISTINCT(`user_id`) FROM `helpdesk_ticket_history` WHERE `helpdesk_ticket_id`=" + quoted(ticket_id) + ") "
											"AND "
											"`id` != " + quoted(user->GetID()) + " "
											";"
										, db);
}

static auto Get_NewTicket_NotificationRecipients_Email(string ticket_id, string severity, CMysql *db, CUser *user)
{
	return ReturnVectorFromTwoFields("SELECT DISTINCT(`login`),\"\" FROM `users` WHERE "
											"LENGTH(`email`) > 0 "
											"AND "
											"`helpdesk_new_notification_S" + severity + "_email` = \"Y\" "
											";"
										, db);
}

static auto Get_ExistingTicket_NotificationRecipients_SMS(string ticket_id, string severity, CMysql *db, CUser *user)
{
	return ReturnVectorFromTwoFields("SELECT `country_code`,`phone` FROM `users` WHERE "
											"`is_phone_confirmed` = \"Y\" "
											"AND "
											"`helpdesk_subscription_S" + severity + "_sms` = \"Y\" "
											"AND "
											"`id` IN (SELECT DISTINCT(`user_id`) FROM `helpdesk_ticket_history` WHERE `helpdesk_ticket_id`=" + quoted(ticket_id) + ") "
											"AND "
											"`id` != " + quoted(user->GetID()) + " "
											";"
										, db);
}

static auto Get_NewTicket_NotificationRecipients_SMS(string ticket_id, string severity, CMysql *db, CUser *user)
{
	return ReturnVectorFromTwoFields("SELECT `country_code`,`phone` FROM `users` WHERE "
											"`is_phone_confirmed` = \"Y\" "
											"AND "
											"`helpdesk_new_notification_S" + severity + "_sms` = \"Y\" "
											";"
										, db);
}

static auto SendSMSNotification(vector<string> recipients, string message, CCgi *indexPage, CMysql *db, CUser *user)
{
	MESSAGE_DEBUG("", "", "start");

	auto	error_message = ""s;
	c_smsc	smsc(db);

	for(auto &recipient: recipients)
	{
		error_message = smsc.send_sms(recipient, message, 0, "", 0, SMSC_DELIVERY_MODE, SMSC_SENDER_NAME, "", "");

		if(error_message.length())
		{
			MESSAGE_ERROR("", "", "fail to send message to recipient(" + recipient + ")");

			// --- do your best to notify everybody, don't stop in the middle of notification process.
			error_message = "";
		}
	}

	MESSAGE_DEBUG("", "", "finish(" + error_message + ")");

	return error_message;	
}

static auto SendSMSNotification_NewTicket(vector<string> recipients, CCgi *indexPage, CMysql *db, CUser *user)
{
	return SendSMSNotification(recipients, gettext("case") + " "s + indexPage->GetVarsHandler()->Get("case_title") + " (" + indexPage->GetVarsHandler()->Get("case_id") + ") (S" + indexPage->GetVarsHandler()->Get("severity") + ") " + gettext("opened") + ".", indexPage, db, user);
}

static auto SendSMSNotification_ExistingTicket(vector<string> recipients, CCgi *indexPage, CMysql *db, CUser *user)
{
	return SendSMSNotification(recipients, gettext("case") + " "s + indexPage->GetVarsHandler()->Get("case_title") + " (" + indexPage->GetVarsHandler()->Get("case_id") + ") (S" + indexPage->GetVarsHandler()->Get("severity") + ") " + gettext("updated") + ".", indexPage, db, user);
}

static auto SendEmailNotification(vector<string> recipients, string email_template, CCgi *indexPage, CMysql *db, CUser *user)
{
	MESSAGE_DEBUG("", "", "start (# of recipients:" + to_string(recipients.size()) + ")");

	auto		error_message = ""s;
	CMailLocal	mail;


	for(auto &recipient: recipients)
	{
		mail.Send(recipient, email_template, indexPage->GetVarsHandler(), db);
	}

	MESSAGE_DEBUG("", "", "finish(" + error_message + ")");

	return error_message;	
}

static auto SendEmailNotification_NewTicket(vector<string> recipients, CCgi *indexPage, CMysql *db, CUser *user)
{
	return SendEmailNotification(recipients, "helpdesk_notification_new_case", indexPage, db, user);
}

static auto SendEmailNotification_ExistingTicket(vector<string> recipients, CCgi *indexPage, CMysql *db, CUser *user)
{
	return SendEmailNotification(recipients, "helpdesk_notification_existing_case", indexPage, db, user);
}

static auto Notify_ExistingTicket_Subscribers(string ticket_id, CMysql *db, CUser *user, CCgi *indexPage)
{
	MESSAGE_DEBUG("", "", "start");

	auto	error_message	= ""s;

	if(db->Query("SELECT `title` FROM `helpdesk_tickets` WHERE `id`=" + quoted(ticket_id) + ";"))
	{
		auto	title = db->Get(0, 0);
		auto	severity = GetTicketSeverity(ticket_id, db);

		indexPage->RegisterVariableForce("case_title", title);
		indexPage->RegisterVariableForce("case_id", ticket_id);
		indexPage->RegisterVariableForce("severity", severity);

		{
			auto	email_recipients = Get_ExistingTicket_NotificationRecipients_Email(ticket_id, severity, db, user);

			if((error_message = SendEmailNotification_ExistingTicket(email_recipients, indexPage, db, user)).empty())
			{
				auto	sms_recipients = Get_ExistingTicket_NotificationRecipients_SMS(ticket_id, severity, db, user);

				if((error_message = SendSMSNotification_ExistingTicket(sms_recipients, indexPage, db, user)).empty())
				{

				}
				else
				{
					MESSAGE_ERROR("", "", error_message);
				}
			}
			else
			{
				MESSAGE_ERROR("", "", error_message);
			}
		}

	}
	else
	{
		error_message = gettext("helpdesk ticket not found");
		MESSAGE_ERROR("", "", error_message);
	}

	MESSAGE_DEBUG("", "", "finish (" + error_message + ")");

	return error_message;
}

static auto Notify_NewTicket_Subscribers(string ticket_id, CMysql *db, CUser *user, CCgi *indexPage)
{
	MESSAGE_DEBUG("", "", "start");

	auto	error_message	= ""s;

	if(db->Query("SELECT `title` FROM `helpdesk_tickets` WHERE `id`=" + quoted(ticket_id) + ";"))
	{
		auto	title = db->Get(0, 0);
		auto	severity = GetTicketSeverity(ticket_id, db);

		indexPage->RegisterVariableForce("case_title", title);
		indexPage->RegisterVariableForce("case_id", ticket_id);
		indexPage->RegisterVariableForce("severity", severity);

		{
			auto	email_recipients = Get_NewTicket_NotificationRecipients_Email(ticket_id, severity, db, user);

			if((error_message = SendEmailNotification_NewTicket(email_recipients, indexPage, db, user)).empty())
			{
				auto	sms_recipients = Get_NewTicket_NotificationRecipients_SMS(ticket_id, severity, db, user);

				if((error_message = SendSMSNotification_NewTicket(sms_recipients, indexPage, db, user)).empty())
				{

				}
				else
				{
					MESSAGE_ERROR("", "", error_message);
				}
			}
			else
			{
				MESSAGE_ERROR("", "", error_message);
			}
		}

	}
	else
	{
		error_message = gettext("helpdesk ticket not found");
		MESSAGE_ERROR("", "", error_message);
	}

	MESSAGE_DEBUG("", "", "finish (" + error_message + ")");

	return error_message;
}

static auto SaveFilesAndUpdateDB(string ticket_history_id, CCgi *indexPage, CMysql *db)
{
	MESSAGE_DEBUG("", "", "start");

	auto	error_message	= ""s;
	auto	itemType		= "helpdesk_ticket_attach"s;

	for(auto filesCounter = 0; filesCounter < indexPage->GetFilesHandler()->Count(); ++filesCounter)
	{
		if(indexPage->GetFilesHandler()->GetSize(filesCounter) > GetSpecificData_GetMaxFileSize(itemType))
		{
			error_message = "file [" + indexPage->GetFilesHandler()->GetName(filesCounter) + "] size exceed permitted maximum: " + to_string(indexPage->GetFilesHandler()->GetSize(filesCounter)) + " > " + to_string(GetSpecificData_GetMaxFileSize(itemType));
			MESSAGE_ERROR("", "", error_message);

			break;
		}
	}

	if(error_message.empty())
	{
		for(auto filesCounter = 0; filesCounter < indexPage->GetFilesHandler()->Count(); ++filesCounter)
		{
			auto	finalFilename	= ""s;
			auto	short_filename	= ""s;
			auto	file_name_ext	= CleanupFilename(indexPage->GetFilesHandler()->GetName(filesCounter));
			auto	ext_pos			= file_name_ext.rfind(".");
			auto	file_name		= file_name_ext.substr(0, ext_pos);
			auto	file_ext		= (ext_pos != string::npos ? file_name_ext.substr(ext_pos, file_name_ext.length() - ext_pos) : ""s);
			auto	folderID		= 0;
			auto	filePrefix		= ""s;

			do
			{
				folderID	= (int)(rand()/(RAND_MAX + 1.0) * GetSpecificData_GetNumberOfFolders(itemType)) + 1;
				filePrefix	= GetRandom(20);

				short_filename = to_string(folderID) + "/" + file_name + "_" + filePrefix + file_ext;
				finalFilename = GetSpecificData_GetBaseDirectory(itemType) + "/" + short_filename;

			} while(isFileExists(finalFilename));

			{
				// --- Save file to "/tmp/" for checking of image validity
				auto	f = fopen(finalFilename.c_str(), "w");
				if(f)
				{
					fwrite(indexPage->GetFilesHandler()->Get(filesCounter), indexPage->GetFilesHandler()->GetSize(filesCounter), 1, f);
					fclose(f);

					{
						auto	attach_id = db->InsertQuery("INSERT INTO `helpdesk_ticket_attaches` "
															"(`helpdesk_ticket_history_id`, `filename`) "
															"VALUES"
															"(" + quoted(ticket_history_id) + ", " + quoted(short_filename) + ");"
															);
						if(attach_id)
						{

						}
						else
						{
							error_message = gettext("SQL syntax error");
							MESSAGE_ERROR("", "", error_message);

							unlink(finalFilename.c_str());
						}
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "fail to write file (" + finalFilename + "). Some files has not been attached to helpdesk_ticket_history.id(" + ticket_history_id + ")");
				}
			}
		}
	}

	MESSAGE_DEBUG("", "", "finish (" + error_message + ")");

	return error_message;
}

int main(void)
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	CMysql			db;
	string			action = "";

	MESSAGE_DEBUG("", action, __FILE__);

	signal(SIGSEGV, crash_handler);

	// --- randomization
	{
		struct timeval	tv;
		gettimeofday(&tv, NULL);
		srand(tv.tv_sec * tv.tv_usec * 100000);
	}

	try
	{

		indexPage.ParseURL();

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			MESSAGE_ERROR("", action, "template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect() < 0)
		{
			MESSAGE_ERROR("", action, "Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);


		action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));

		MESSAGE_DEBUG("", "", "action taken from HTTP is " + action);

		// --- generate common parts
		{
			// --- it has to be run before session, because session relay upon Apache environment variable
			if(RegisterInitialVariables(&indexPage, &db, &user))
			{
			}
			else
			{
				MESSAGE_ERROR("", action, "RegisterInitialVariables failed, throwing exception");
				throw CExceptionHTML("environment variable error");
			}

			//------- Generate session
			action = GenerateSession(action, &indexPage, &db, &user);
			action = LogoutIfGuest(action, &indexPage, &db, &user);
		}
	// ------------ end generate common parts

		MESSAGE_DEBUG("", "", "pre-condition if(action == \"" + action + "\")");

		if((action.length() > 10) && (action.compare(action.length() - 9, 9, "_template") == 0))
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage;

			MESSAGE_DEBUG("", action, "start");

			{
				string		template_name = action.substr(0, action.length() - 9) + ".htmlt";

				if(!indexPage.SetTemplate(template_name))
				{
					MESSAGE_ERROR("", action, "can't find template " + template_name);
				}
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_openCase")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			auto			severity	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("severity"));
			auto			title		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
			auto			description	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("description"));

			if(severity.length())
			{
				if(title.length())
				{
					if(description.length())
					{
						auto	ticket_id = db.InsertQuery("INSERT INTO `helpdesk_tickets` (`title`, `customer_user_id`) VALUES (" + quoted(title) + ", " + user.GetID() + ")");

						if(ticket_id)
						{
							auto	ticket_history_id = db.InsertQuery("INSERT INTO `helpdesk_ticket_history` "
																		"(`helpdesk_ticket_id`, `user_id`, `severity`, `state`, `description`, `eventTimestamp`) "
																		"VALUES "
																		"(" + quoted(to_string(ticket_id)) + ", " + quoted(user.GetID()) + ", " + quoted(severity) + ", " + quoted(HELPDESK_STATE_NEW) + ", " + quoted(description) + ", UNIX_TIMESTAMP())");
							if(ticket_history_id)
							{
								if((error_message = SaveFilesAndUpdateDB(to_string(ticket_history_id), &indexPage, &db)).empty())
								{
									auto	local_error_message = Notify_NewTicket_Subscribers(to_string(ticket_id), &db, &user, &indexPage);
									if(local_error_message.length())
									{
										MESSAGE_ERROR("", action, local_error_message);
									}

									success_message = quoted("tickets"s) + ":[" + GetHelpDeskTicketsInJSONFormat("SELECT * FROM `helpdesk_tickets` WHERE `id`=" + quoted(to_string(ticket_id)) + ";", &db, &user) + "]";
								}
								else
								{
									MESSAGE_ERROR("", action, error_message);

									db.Query("DELETE FROM `helpdesk_ticket_history` WHERE `id`=" + quoted(to_string(ticket_history_id)) + ";");
									db.Query("DELETE FROM `helpdesk_tickets` WHERE `id`=" + quoted(to_string(ticket_id)) + ";");
								}
							}
							else
							{
								error_message = gettext("SQL syntax error");
								MESSAGE_ERROR("", action, error_message);
							}
						}
						else
						{
							error_message = gettext("SQL syntax error");
							MESSAGE_ERROR("", action, error_message);
						}
					}
					else
					{
						error_message = gettext("parameters incorrect");
						MESSAGE_ERROR("", action, "user.id(" + user.GetID() + ") didn't set description");
					}
				}
				else
				{
					error_message = gettext("parameters incorrect");
					MESSAGE_ERROR("", action, "user.id(" + user.GetID() + ") didn't set title");
				}
			}
			else
			{
				error_message = gettext("parameters incorrect");
				MESSAGE_ERROR("", action, "user.id(" + user.GetID() + ") didn't set severity");
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(
			(action == "AJAX_updateCase") || 
			(action == "AJAX_monitoringCase") || 
			(action == "AJAX_solutionProvidedCase") || 
			(action == "AJAX_closePendingCase") || 
			(action == "AJAX_closeCase") 
			)
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			auto			ticket_id	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			new_severity= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("severity"));
			auto			description	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("description"));

			if(new_severity.length())
			{
				if(ticket_id.length())
				{
					if(description.length())
					{
						if((error_message = isUserAllowedToChangeTicket(ticket_id, user.GetID(), &db, &user)).empty())
						{
/*
							if((user.GetType() == "helpdesk")  && amINewEngineer(ticket_id, &db, &user))
							{
								auto	ticket_history_id = db.InsertQuery("INSERT INTO `helpdesk_ticket_history` "
																			"(`helpdesk_ticket_id`, `user_id`, `severity`, `state`, `description`, `eventTimestamp`) "
																			"VALUES "
																			"(" + quoted((ticket_id)) + ", " + quoted(user.GetID()) + ", " + quoted(new_severity) + ", " + quoted(HELPDESK_STATE_ASSIGNED) + ", " + quoted(description) + ", UNIX_TIMESTAMP() - 1);");

								if(ticket_history_id) {}
								else
								{
									MESSAGE_ERROR("", action, "SQL syntax error");
								}
							}
*/
							auto	affected = db.Query("SELECT `severity` FROM `helpdesk_ticket_history` WHERE `helpdesk_ticket_id`=" + quoted(ticket_id) + " ORDER BY `helpdesk_ticket_history`.`eventTimestamp` DESC LIMIT 0,1; ");

							if(affected)
							{
								auto	curr_severity = db.Get(0, 0);

								auto	new_ticket_state = 
											(
												curr_severity != new_severity ?											quoted(HELPDESK_STATE_SEVERITY_CHANGED) :
												action == "AJAX_closeCase" ?											quoted(HELPDESK_STATE_CLOSED) : 
												action == "AJAX_closePendingCase" ?										quoted(HELPDESK_STATE_CLOSE_PENDING) : 
												action == "AJAX_monitoringCase" ?										quoted(HELPDESK_STATE_MONITORING) : 
												action == "AJAX_solutionProvidedCase" ?									quoted(HELPDESK_STATE_SOLUTION_PROVIDED) : 
												user.GetType() == "helpdesk" && amINewEngineer(ticket_id, &db, &user) ? quoted(HELPDESK_STATE_ASSIGNED) :
												user.GetType() == "helpdesk" ? 											quoted(HELPDESK_STATE_CUSTOMER_PENDING) :
																														quoted(HELPDESK_STATE_COMPANY_PENDING)
											);
								// --- regular ticket update
								auto	ticket_history_id = db.InsertQuery("INSERT INTO `helpdesk_ticket_history` "
																			"(`helpdesk_ticket_id`, `user_id`, `severity`, `state`, `description`, `eventTimestamp`) "
																			"VALUES "
																			"(" + quoted((ticket_id)) + ", " + quoted(user.GetID()) + ", " + quoted(new_severity) + ", " + new_ticket_state + ", " + quoted(description) + ", UNIX_TIMESTAMP());");

								if(ticket_history_id)
								{
									if((error_message = SaveFilesAndUpdateDB(to_string(ticket_history_id), &indexPage, &db)).empty())
									{
										auto	local_error_message = Notify_ExistingTicket_Subscribers(ticket_id, &db, &user, &indexPage);

										if(local_error_message.length())
										{
											MESSAGE_ERROR("", "", local_error_message);
										}

										success_message = quoted("tickets"s) + ":[" + GetHelpDeskTicketsInJSONFormat("SELECT * FROM `helpdesk_tickets` WHERE `id`=" + quoted((ticket_id)) + ";", &db, &user) + "]";
									}
									else
									{
										MESSAGE_ERROR("", action, error_message);
									}
								}
								else
								{
									error_message = gettext("SQL syntax error");
									MESSAGE_ERROR("", "", error_message)
								}
							}
							else
							{
								MESSAGE_ERROR("", "", "ticket.id(" + ticket_id + ") history is empty. History must be built during ticket open or ticket update.");
							}
						}
						else
						{
							MESSAGE_ERROR("", action, error_message);
						}

					}
					else
					{
						error_message = gettext("parameters incorrect");
						MESSAGE_ERROR("", action, "user.id(" + user.GetID() + ") didn't set description");
					}
				}
				else
				{
					error_message = gettext("parameters incorrect");
					MESSAGE_ERROR("", action, "user.id(" + user.GetID() + ") didn't set id");
				}
			}
			else
			{
				error_message = gettext("parameters incorrect");
				MESSAGE_ERROR("", action, "user.id(" + user.GetID() + ") didn't set severity");
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_getCase")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			auto			id	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("id"));

			if(id.length())
			{
				if((user.GetType() == "helpdesk") || (isHelpdeskTicketOwner(id, user.GetID(), &db, &user)))
				{
					success_message = quoted("tickets"s) + ":[" + GetHelpDeskTicketsInJSONFormat("SELECT * FROM `helpdesk_tickets` WHERE `id`=" + quoted(id) + ";", &db, &user) + "]";
				}
				else
				{
					error_message = gettext("you are not authorized");
					MESSAGE_ERROR("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("parameters incorrect");
				MESSAGE_ERROR("", action, "user.id(" + user.GetID() + ") didn't set severity");
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_getCasesList")
		{
			MESSAGE_DEBUG("", action, "start");

			auto	success_message = ""s;
			auto	error_message = ""s;

			auto	filter_param	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("filter"));
			auto	filter			= ""s;
			auto	sql_query		= ""s;

			if(filter_param == "my")
			{
				filter = "SELECT `helpdesk_ticket_id` FROM `helpdesk_ticket_history_last_helpdesk_user_update_view` WHERE `user_id`=" + quoted(user.GetID());
				filter = "WHERE `id` IN (" + filter + ")";
			}
			if(filter_param == "my_open")
			{
/*				filter = "SELECT `helpdesk_ticket_history_last_case_state_view`.`helpdesk_ticket_id` "
							"FROM `helpdesk_ticket_history_last_helpdesk_user_update_view` "
							"INNER JOIN `helpdesk_ticket_history_last_case_state_view` "
							"ON `helpdesk_ticket_history_last_case_state_view`.`helpdesk_ticket_id`=`helpdesk_ticket_history_last_helpdesk_user_update_view`.`helpdesk_ticket_id`"
							"WHERE "
							"`helpdesk_ticket_history_last_helpdesk_user_update_view`.`user_id`=" + quoted(user.GetID()) + " "
							"AND "
							"`helpdesk_ticket_history_last_case_state_view`.`state`!=\"closed\"";
*/
				filter = Get_OpenHelpdeskTicketIDByUserID_sqlquery(user.GetID());
				filter = "WHERE `id` IN (" + filter + ")";
			}
			if(filter_param == "open")
			{
				filter = "SELECT `helpdesk_ticket_id` FROM `helpdesk_ticket_history_last_case_state_view` WHERE `state`!=\"closed\"";
				filter = "WHERE `id` IN (" + filter + ")";
			}
			if(filter_param == "new")
			{
				filter = "SELECT `helpdesk_ticket_id` FROM `helpdesk_ticket_history_last_case_state_view` WHERE `state`=\"new\"";
				filter = "WHERE `id` IN (" + filter + ")";
			}

			if(user.GetType() == "helpdesk")
				sql_query = "SELECT * FROM `helpdesk_tickets` " + filter + " ORDER BY `id` DESC";
			else
				sql_query = "SELECT * FROM `helpdesk_tickets` WHERE `customer_user_id`=" + quoted(user.GetID()) + " ORDER BY `id` DESC;";

			if(sql_query.length())
			{
				success_message = quoted("tickets"s) + ":[" + GetHelpDeskTicketsInJSONFormat(sql_query, &db, &user) + "]";
			}
			else
			{
				error_message = gettext("you are not authorized");
				MESSAGE_ERROR("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateTicketTitle")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			auto			id		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			value	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

			if(id.length() && value.length())
			{
				if((error_message = isUserAllowedToChangeTicket(id, user.GetID(), &db, &user)).empty())
				{
					db.Query("UPDATE `helpdesk_tickets` SET `title`=" + quoted(value) + " WHERE `id`=" + quoted(id) + ";");
				}
				else
				{
					MESSAGE_ERROR("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("parameters incorrect");
				MESSAGE_ERROR("", action, "user.id(" + user.GetID() + ") didn't set severity");
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_getUserInfo")
		{
			MESSAGE_DEBUG("", action, "start");

			auto	error_message = ""s;

			AJAX_ResponseTemplate(&indexPage, "\"users\":[" + GetHelpdeskBaseUserInfoInJSONFormat("SELECT * FROM `users` WHERE `id`=" + quoted(user.GetID()) + ";", &db, &user) + "]", error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_getDashboardData")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			if((user.GetType() == "helpdesk"))
			{
				auto affected = db.Query(Get_OpenHelpdeskTicketIDByUserID_sqlquery(user.GetID()));

				success_message += (success_message.length() ? "," : "") + quoted("my_active_cases"s) + ":" + to_string(affected) + "";

				if(db.Query("SELECT COUNT(*) FROM `helpdesk_ticket_history_last_case_state_view` WHERE `state`!=" + quoted("closed"s) + ";"))
					success_message += (success_message.length() ? "," : "") +  quoted("active_cases"s) + ":" + db.Get(0, 0) + "";

				if(db.Query("SELECT COUNT(*) FROM `helpdesk_ticket_history_last_case_state_view` WHERE `state`=" + quoted("new"s) + ";"))
					success_message += (success_message.length() ? "," : "") + quoted("new_cases"s) + ":" + db.Get(0, 0) + "";

/*				if(db.Query("SELECT COUNT(*) FROM `helpdesk_ticket_history_last_helpdesk_user_update_view` WHERE `state`!=" + quoted("closed"s) + " AND `user_id`=" + quoted(user.GetID()) + ";"))
					success_message += (success_message.length() ? "," : "") + quoted("my_active_cases"s) + ":" + db.Get(0, 0) + "";
*/
				if(db.Query("SELECT COUNT(*) FROM `helpdesk_ticket_history_last_case_state_view` WHERE "
								"`state`=\"company_pending\" "
								"AND "
								"`helpdesk_ticket_id` IN (SELECT `helpdesk_ticket_id` FROM `helpdesk_ticket_history_last_helpdesk_user_update_view` WHERE `user_id`=" + quoted(user.GetID()) + ")"
							";"))
					success_message += (success_message.length() ? "," : "") + quoted("my_company_pending_cases"s) + ":" + db.Get(0, 0) + "";
			}
			else
			{
				error_message = gettext("you are not authorized");
				MESSAGE_ERROR("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}


		MESSAGE_DEBUG("", "", "post-condition if(action == \"" + action + "\")");

		indexPage.OutTemplate();

	}
	catch(CExceptionHTML &c)
	{

		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);
		MESSAGE_DEBUG("", action, "catch CExceptionHTML: DEBUG exception reason: [" + c.GetReason() + "]");

		if(!indexPage.SetTemplate(c.GetTemplate()))
		{
			return(-1);
		}
		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(0);
	}
	catch(CException &c)
	{
		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}
		{
			MESSAGE_ERROR("", action, "catch CException: exception: ERROR  " + c.GetReason());
		}

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(-1);
	}
	catch(exception& e)
	{
		{
			MESSAGE_ERROR("", action, "catch(exception& e): catch standard exception: ERROR  " + e.what());
		}

		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}
		indexPage.RegisterVariable("content", e.what());
		indexPage.OutTemplate();
		return(-1);
	}

	return(0);
}
