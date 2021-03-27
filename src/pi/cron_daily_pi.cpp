#include "cron_daily_pi.h"

auto CleanupActivators(CMysql *db)
{
	auto		result = true;

	MESSAGE_DEBUG("", "", "start");

	db->Query("DELETE FROM `activators` WHERE  `date`<=(now() - INTERVAL " + to_string(ACTIVATOR_SESSION_LEN) + " MINUTE);");
	
	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto CleanupRemovedSessions(CMysql *db)
{
	auto		result = true;

	MESSAGE_DEBUG("", "", "start");

	db->Query("DELETE FROM `sessions` WHERE `remove_flag_timestamp`< (UNIX_TIMESTAMP() - 3600*24*365) AND `remove_flag`=\"Y\";");
	
	MESSAGE_DEBUG("", "", "finish");

	return result;
}


auto RemoveOldCaptcha()
{
	auto		result = true;
	DIR 		*dir;
	struct 		dirent *ent;
	string		dirName = IMAGE_CAPTCHA_DIRECTORY;
	time_t		now;

	MESSAGE_DEBUG("", "", "start");

	time(&now);

	if ((dir = opendir( dirName.c_str() )) != NULL)
	{
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL)
		{
			struct	stat	sb;
			string	fileName = dirName + ent->d_name;

			if(stat(fileName.c_str(), &sb) == 0)
			{
				double		secondsBetween = difftime(now, sb.st_mtime);


				if(secondsBetween > 2600 * 24)
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: remove file [" + ent->d_name + "] created " + to_string(secondsBetween) + " secs ago");
					}
					unlink(fileName.c_str());
				}
			}
			else
			{
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: file stat [" + ent->d_name + "]");
				}
			}
		}
		closedir (dir);
	}
	else
	{
		/* could not open directory */
		result = false;
	}



	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto RemoveTempMedia(CMysql *db)
{
	auto	result = true;
	auto	affected = 0;

	MESSAGE_DEBUG("", "", "start");

	affected = db->Query("SELECT * FROM `temp_media` WHERE `mediaType`=\"image\" AND `eventTimestamp`<DATE_SUB(CURDATE(), INTERVAL 2 DAY);");
	for(int i = 0; i < affected; ++i)
	{
		string	filename = IMAGE_TEMP_DIRECTORY + "/" + db->Get(i, "folder") + "/" + db->Get(i, "filename");

		if(isFileExists(filename))
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: deleting file [filename=" + filename + "]");
			}
			unlink(filename.c_str());
		}
		else
		{
			CLog	log;
			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) +  "]:ERROR: file doesn't exists  [filename=" + filename + "]");
		}
	}

	db->Query("DELETE FROM `temp_media` WHERE `mediaType`=\"image\" AND `eventTimestamp`<DATE_SUB(CURDATE(), INTERVAL 2 DAY);");

	MESSAGE_DEBUG("", "", "finish (" + to_string(result) + ")");

	return result;
}

auto MoveQuietCasesTo_ClosePending_State(CMysql *db)
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	auto affected = db->Query("SELECT `helpdesk_ticket_id`, `user_id`, `severity` FROM `helpdesk_ticket_history_last_case_state_view` WHERE `state`=\"customer_pending\" AND `eventTimestamp`<UNIX_TIMESTAMP()-" + to_string(HELPDESK_TICKET_CUSTOMER_PENDING_TIMEOUT) + "*24*3600;");
	vector<string> tickets;
	vector<string> users;
	vector<string> severity;

	for(auto i = 0; i < affected; ++i)
	{
		tickets.push_back(db->Get(i, "helpdesk_ticket_id"));
		users.push_back(db->Get(i, "user_id"));
		severity.push_back(db->Get(i, "severity"));
	}

	for(auto i = 0u; i < tickets.size(); ++i)
	{
		db->InsertQuery("INSERT INTO `helpdesk_ticket_history` (`helpdesk_ticket_id`, `user_id`, `severity`, `state`, `description`, `eventTimestamp`) VALUES "s +
						"(" +
						quoted(tickets[i]) + ", " +
						quoted(users[i]) + ", " +
						quoted(severity[i]) + ", " +
						quoted(HELPDESK_STATE_CLOSE_PENDING) + ", " +
						quoted(RemoveQuotas(gettext("Automatic message") + ": "s + gettext("move case to the Close_Pending state due to long waiting period"))) + ", " +
						"UNIX_TIMESTAMP()"
						")"
						";");
	}

	MESSAGE_DEBUG("", "", "finish (" + error_message + ")");

	return	error_message;
}

auto CloseHelpDeskTicketIn_ClosePending_and_SolutionProvided_States(CMysql *db)
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	auto affected = db->Query("SELECT `helpdesk_ticket_id`, `user_id`, `severity` FROM `helpdesk_ticket_history_last_case_state_view` WHERE `state` IN (\"close_pending\", \"solution_provided\") AND `eventTimestamp`<UNIX_TIMESTAMP()-" + to_string(HELPDESK_TICKET_CLOSE_PENDING_TIMEOUT) + "*24*3600");
	vector<string> tickets;
	vector<string> users;
	vector<string> severity;

	for(auto i = 0; i < affected; ++i)
	{
		tickets.push_back(db->Get(i, "helpdesk_ticket_id"));
		users.push_back(db->Get(i, "user_id"));
		severity.push_back(db->Get(i, "severity"));
	}

	for(auto i = 0u; i < tickets.size(); ++i)
	{
		db->InsertQuery("INSERT INTO `helpdesk_ticket_history` (`helpdesk_ticket_id`, `user_id`, `severity`, `state`, `description`, `eventTimestamp`) VALUES "s +
						"(" +
						quoted(tickets[i]) + ", " +
						quoted(users[i]) + ", " +
						quoted(severity[i]) + ", " +
						quoted(HELPDESK_STATE_CLOSED) + ", " +
						quoted(RemoveQuotas(gettext("Automatic message") + ": "s + gettext("case closed"))) + ", " +
						"UNIX_TIMESTAMP()"
						")"
						";");
	}

	MESSAGE_DEBUG("", "", "finish (" + error_message + ")");

	return	error_message;
}

int main()
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CMysql			user;
	CMysql			db;
	struct timeval	tv;

	{
		CLog	log;
		log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: " + __FILE__);
	}

	signal(SIGSEGV, crash_handler);

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);

	if(SetLocale(LOCALE_DEFAULT)) {}
	else
	{
		MESSAGE_ERROR("", "", "fail to setup locale");
	}

	try
	{
		auto	error_message = ""s;

		if(db.Connect() < 0)
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		//--- start of daily cron main functionality
		CleanupActivators(&db);
		CleanupRemovedSessions(&db);

		//--- Remove temporarily media files
		RemoveTempMedia(&db);

		//--- Remove old captcha
		RemoveOldCaptcha();

		// --- helpdesk tickets management
		if((error_message = MoveQuietCasesTo_ClosePending_State(&db))	.length()) MESSAGE_ERROR("", "", error_message);
		if((error_message = CloseHelpDeskTicketIn_ClosePending_and_SolutionProvided_States(&db))
																		.length()) MESSAGE_ERROR("", "", error_message);

		//--- end of daily cron main functionality
	}
	catch(CExceptionHTML &c)
	{
		CLog	log;

		c.SetLanguage("ru");
		c.SetDB(&db);

		log.Write(DEBUG, string(__func__) + ": catch CExceptionHTML: DEBUG exception reason: [", c.GetReason(), "]");

		return(0);
	}
	catch(CException &c)
	{
		CLog 	log;

		log.Write(ERROR, string(__func__) + ": catch CException: exception: ERROR  ", c.GetReason());

		return(-1);
	}
	catch(exception& e)
	{
		CLog 	log;
		log.Write(ERROR, string(__func__) + ": catch(exception& e): catch standard exception: ERROR  ", e.what());

		return(-1);
	}

	return(0);
}

