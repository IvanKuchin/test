#include "group.h"

int main()
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	string			action, partnerID;
	CMysql			db;
	struct timeval	tv;

	{
		CLog	log;
		log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: " + __FILE__);
	}

	signal(SIGSEGV, crash_handler); 

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);

	try
	{

		indexPage.ParseURL();

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect() < 0)
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);

		action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));

		MESSAGE_DEBUG("", "", "action taken from HTTP is " + action);

		// ------------ generate common parts
		{
			if(RegisterInitialVariables(&indexPage, &db, &user))
			{
			}
			else
			{
				MESSAGE_ERROR("", "", "RegisterInitialVariables failed, throwing exception");
				throw CExceptionHTML("environment variable error");
			}

			action = GenerateSession(action, &indexPage, &db, &user);
		}
		// ------------ end generate common parts

		MESSAGE_DEBUG("", "", "pre-condition if(action == \"" + action + "\")");

		if(action == "groups_i_own_list")
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]::groups_i_own_list: start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == " + action + ": re-login required");
				}

				ost.str("");
				ost << "/?rand=" << GetRandom(10);
				indexPage.Redirect(ost.str());
			}
			else
			{
				indexPage.RegisterVariableForce("title_head", "Мои группы");

				strFriendsOnSinglePage	= indexPage.GetVarsHandler()->Get("FriendsOnSinglePage");
				strPageToGet 			= indexPage.GetVarsHandler()->Get("page");
				if(strPageToGet.empty()) strPageToGet = "0";
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == " + action + ": page ", strPageToGet, " requested");
				}

				indexPage.RegisterVariableForce("myFirstName", user.GetName());
				indexPage.RegisterVariableForce("myLastName", user.GetNameLast());


				if(!indexPage.SetTemplate("groups_i_own_list.htmlt"))
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]: action == " + action + ":ERROR: can't find template my_network.htmlt");
					throw CExceptionHTML("user not activated");
				} // if(!indexPage.SetTemplate("my_network.htmlt"))
			}


			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]::groups_i_own_list: finish");
			}
		}

		if(action == "getGroupWall")
		{
			ostringstream	ost;
			string			id = "", link = "";

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": start");
			}

/*
			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + " re-login required");
				}

				ost.str("");
				ost << "/?rand=" << GetRandom(10);
				indexPage.Redirect(ost.str());
			}
*/
			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			link = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("link"));

			if(!id.length())
			{
				if(link.length())
				{
					if(db.Query("SELECT `id` FROM `groups` WHERE `link`=\"" + link + "\";"))
					{
						id = db.Get(0, "id");
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ":ERROR: group.link(" + link + ") not found");
						}
					}
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ":ERROR: id and link are empty");
					}

				}
			}

			if(!link.length())
			{
				if(id.length())
				{
					if(db.Query("SELECT `link` FROM `groups` WHERE `id`=\"" + id + "\";"))
					{
						link = db.Get(0, "link");
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ":ERROR: group.id(" + id + ") not found");
						}
					}
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ":ERROR: id and link are empty");
					}

				}
			}

			if(id.length()) indexPage.RegisterVariableForce("id", id);
			if(link.length()) indexPage.RegisterVariableForce("link", link);

			if(!indexPage.SetTemplate("view_group_profile.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + string("]:template file getGroupWall.htmlt was missing"));
				throw CException("Template file was missing");
			}
		}


		if(action == "AJAX_getGroupProfile")
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == " + action + ": start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		groupID;

				groupID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				if(groupID.length())
				{
					ostringstream		ost;

					ost.str("");
					ost << "SELECT * FROM `groups` WHERE `id`=\"" << groupID << "\" AND (`isBlocked`='N' OR `owner_id`=\"" << user.GetID() << "\");";

					ostResult	<< "{"
								<< "\"result\":\"success\","
								<< "\"groups\":[" << GetGroupListInJSONFormat(ost.str(), &db, &user) << "]"
								<< "}";
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == " + action + ":ERROR: in groupID [", groupID, "]");

					ostResult << "{\"result\":\"error\",\"description\":\"ERROR in groupID\",\"groups\":[]}";
				}
			}



			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_precreateNewOpenVacancy.htmlt"))

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == " + action + ": start");
			}
		}

		if((action == "AJAX_blockGroup") || (action == "AJAX_unblockGroup"))
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == " + action + ": start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"re-login required\"}";
			}
			else
			{
				string		groupID;

				groupID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				if(groupID.length())
				{
					if(db.Query("SELECT `id` FROM `groups` WHERE `id`=\"" + groupID + "\" AND `owner_id`=\"" + user.GetID() + "\";"))
					{
						string	dbQuery;

						if(action == "AJAX_blockGroup")
							dbQuery = "UPDATE `groups` SET `isBlocked`=\"Y\" WHERE `id`=\"" + groupID + "\";";
						if(action == "AJAX_unblockGroup")
							dbQuery = "UPDATE `groups` SET `isBlocked`=\"N\" WHERE `id`=\"" + groupID + "\";";

						db.Query(dbQuery);
						if(!db.isError())
						{
							ostResult	<< "{"
										<< "\"result\":\"success\","
										<< "\"groups\":[" << GetGroupListInJSONFormat("SELECT * FROM `groups` WHERE `id`=\"" + groupID + "\";", &db, &user) << "]"
										<< "}";
						}
						else
						{

							{
								CLog			log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_blockGroup:ERROR: updating DB");
							}

							ostResult.str("");
							ostResult << "{";
							ostResult << "\"result\" : \"error\",";
							ostResult << "\"description\" : \"ошибка БД\"";
							ostResult << "}";
						}
					}
					else
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:AJAX_blockGroup:ERROR: groupID [" + groupID + "] doesn't belongs to you");

						ostResult << "{\"result\":\"error\",\"description\":\"вы не можете редактировать группу\",\"groups\":[]}";
					}

				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == " + action + ":ERROR: in groupID [", groupID, "]");

					ostResult << "{\"result\":\"error\",\"description\":\"ERROR in groupID\",\"groups\":[]}";
				}
			}



			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_precreateNewOpenVacancy.htmlt"))

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == " + action + ": start");
			}
		}

	    if(action == "AJAX_getGroupWall")
	    {
	        ostringstream   ost;
			auto			success_message = ""s;
			auto			error_message = ""s;
	        int             currPage = 0, newsOnSinglePage = 0;
	        auto			result = ""s;

	        auto			strNewsOnSinglePage = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("NewsOnSinglePage"));
	        auto			strPageToGet        = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("page"));
	        auto			groupLink			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("link"));
	        auto			groupID				= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

	        if(strPageToGet.empty()) strPageToGet = "0";

			MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");

	        try{
	            currPage = stoi(strPageToGet);
	        } catch(...) {
	            currPage = 0;
	        }

	        try{
	            newsOnSinglePage = stoi(strNewsOnSinglePage);
	        } catch(...) {
	            newsOnSinglePage = 30;
	        }

	/*
	        if(user.GetLogin() == "Guest")
	        {
	            ostringstream   ost;

	            {
	                CLog    log;
	                log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == " + action + ": re-login required");
	            }

	            ost.str("");
	            ost << "/?rand=" << GetRandom(10);
	            indexPage.Redirect(ost.str());
	        }
	*/
			if(groupLink.length() && db.Query("SELECT * FROM `groups` WHERE `link`=\"" + groupLink + "\" AND `isblocked`=\"N\";"))
				result = GetNewsFeedInJSONFormat(" ((`feed`.`dstType`=\"group\") AND `feed`.`dstID` IN (SELECT `id` FROM `groups` WHERE `link`=\"" + groupLink + "\" AND `isBlocked`=\"N\")) ", currPage, newsOnSinglePage, &user, &db);
			else if(groupID.length() && db.Query("SELECT * FROM `groups` WHERE `id`=\"" + groupID + "\" AND `isblocked`=\"N\";"))
				result = GetNewsFeedInJSONFormat(" ((`feed`.`dstType`=\"group\") AND `feed`.`dstID` IN (SELECT `id` FROM `groups` WHERE `id`=\"" + groupID + "\" AND `isBlocked`=\"N\")) ", currPage, newsOnSinglePage, &user, &db);
			else
			{
                CLog    log;
                log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]: action == " + action + ":ERROR: group login not found");
			}

			success_message = "\"feed\":[" + result + "]";

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
	    }


		if(action == "AJAX_getGroupProfileAndUser")
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == " + action + ": start");
			}

			ostResult.str("");
/*
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
*/
			{
				string			groupID, groupLink, strNewsOnSinglePage, strPageToGet;

				groupLink 			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("link"));
				groupID 			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("id"));

				if(groupLink.length() || groupID.length())
				{
					ostringstream		ost;

					ost.str("");

					if(groupLink.length())
						ost << "SELECT * FROM `groups` WHERE `link`=\"" << groupLink << "\" and `isBlocked`='N';";
					if(groupID.length())
						ost << "SELECT * FROM `groups` WHERE `id`=\"" << groupID << "\" and `isBlocked`='N';";

					ostResult	<< "{"
								<< "\"result\":\"success\","
								<< "\"groups\":[" << GetGroupListInJSONFormat(ost.str(), &db, &user) << "],"
								<< "\"users\":[" << GetUserListInJSONFormat("SELECT * FROM `users` WHERE `id`=\"" + user.GetID() + "\" AND `isblocked`=\"N\";", &db, &user) << "]"
								<< "}";
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == " + action + ":ERROR: in groupLink [", groupLink, "]");

					ostResult << "{\"result\":\"error\",\"description\":\"ERROR in groupLink\",\"groups\":[]}";
				}
			}



			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_precreateNewOpenVacancy.htmlt"))

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == " + action + ": start");
			}
		}

		// --- AJAX friend list for autocomplete
		if((action == "AJAX_getFindGroupsListAutocomplete") || (action == "AJAX_getFindGroupsList") || (action == "AJAX_getFindGroupByID") || (action == "AJAX_getMyGroupsList"))
		{
			ostringstream	ost, ostFinal;
			string			sessid, lookForKey, groupsList;
			vector<string>	searchWords;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == " + action + ": start");
			}

			// --- Initialization
			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == " + action + ": re-login required");
				}

				ost.str("");
				ost << "/?rand=" << GetRandom(10);
				indexPage.Redirect(ost.str());
			}
			else
			{
				lookForKey = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("lookForKey"));

				if( (lookForKey.length() >= 3) || ((action == "AJAX_getFindGroupByID") && lookForKey.length())  || (action == "AJAX_getMyGroupsList")) 
				{
					ostringstream	ost;

					// --- Looking through group name
					ost.str("");
					if(action == "AJAX_getFindGroupByID")
						ost << "SELECT * FROM `groups` WHERE (`isBlocked`='N' OR `owner_id`=\"" << user.GetID() << "\") AND (`id`=\"" << lookForKey << "\");";
					else if (action == "AJAX_getMyGroupsList")
						ost << "SELECT * FROM `groups` WHERE (`owner_id`=\"" << user.GetID() << "\") OR ((`isBlocked`='N') AND `id` IN (SELECT `entity_id` FROM `users_subscriptions` WHERE `entity_type`=\"group\" AND `user_id`=\"" + user.GetID() + "\"));";
					else
						ost << "SELECT * FROM `groups` WHERE (`isBlocked`='N' OR `owner_id`=\"" << user.GetID() << "\") AND ((`title` like \"%" << lookForKey << "%\") OR (`description` like \"%" << lookForKey << "%\"));";

					ostFinal << "{\"status\":\"success\",\"groups\":[" << GetGroupListInJSONFormat(ost.str(), &db, &user) << "]}";

				}
				else
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == " + action + ": searching key is empty");
					}
					ostFinal << "{\"status\":\"error\",\"description\":\"searching key is empty or less then 3\", \"groups\":[]}";
				}

				indexPage.RegisterVariableForce("result", ostFinal.str());

				if(!indexPage.SetTemplate("json_response.htmlt"))
				{
					CLog	log;

					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + string("]:ERROR: template file json_response.htmlt was missing"));
					throw CException("Template file was missing");
				}
			}

			{
				CLog	log;

				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": final response [length = " + to_string(ostFinal.str().length()) + "]");
			}
		}

		if((action == "AJAX_SubscribeOnGroup") || (action == "AJAX_UnsubscribeFromGroup"))
		{
			ostringstream   ostResult;
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == " + action + ": start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == " + action + ": re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		  groupID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				ostResult	<< "{" << (action == "AJAX_SubscribeOnGroup" ? SubscribeToGroup(groupID, &user, &db) : UnsubscribeFromGroup(groupID, &user, &db)) << ",";
				ostResult	<< "\"subscriptions\":[" << GetUserSubscriptionsInJSONFormat("SELECT * FROM `users_subscriptions` WHERE `user_id`=\"" + user.GetID() + "\";", &db) << "]";
				ostResult	<< "}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == " + action + ":ERROR: can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == " + action + ": finish");
			}
		}
		if(action == "edit_group")
		{
			ostringstream	ost;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == " + action + ": re-login required");
				}

				ost.str("");
				ost << "/?rand=" << GetRandom(10);
				indexPage.Redirect(ost.str());
			}

			indexPage.RegisterVariableForce("title", "Редактирование группы");

			if(!indexPage.SetTemplate("edit_group.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + string("]:template file edit_group.htmlt was missing"));
				throw CException("Template file edit_group.htmlt was missing");
			}  // if(!indexPage.SetTemplate("edit_group.htmlt"))

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": end");
			}
		} 	// if(action == "edit_group")

		if(action == "createnewgroup")
		{
			ostringstream	ost;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == " + action + ": re-login required");
				}

				ost.str("");
				ost << "/?rand=" << GetRandom(10);
				indexPage.Redirect(ost.str());
			}

			indexPage.RegisterVariableForce("title", "Новая группа");

			if(!indexPage.SetTemplate("create_group.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + string("]:template file createnewgroup.htmlt was missing"));
				throw CException("Template file createnewgroup.htmlt was missing");
			}  // if(!indexPage.SetTemplate("createnewgroup.htmlt"))

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": end");
			}
		} 	// if(action == "createnewgroup")

		if(action == "AJAX_updateGroupTitle")
		{
			string			value, id;
			ostringstream	ostFinal;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == " + action + ": re-login required");
				}

				ost.str("");
				ost << "/?rand=" << GetRandom(10);
				indexPage.Redirect(ost.str());
			}

			value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("groupid"));
			ostFinal.str("");

			if(value.length() && id.length()) 
			{
				if(db.Query("SELECT `id` FROM `groups` WHERE `id`=\"" + id + "\" AND `owner_id`=\"" + user.GetID() + "\";"))
				{
					db.Query("UPDATE `groups` SET `title`=\"" + value + "\"  WHERE `id`=\"" + id + "\";");

					if(!db.isError())
					{
						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"description\" : \"\"";
						ostFinal << "}";
					}
					else
					{

						{
							CLog			log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_updateGroupTitle:ERROR: updating DB");
						}

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"description\" : \"error updating DB\"";
						ostFinal << "}";
					}
				}
				else
				{
					{
						CLog			log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_updateGroupTitle:ERROR: user.id(" + user.GetID() + ") is not a group(" + id + ") owner");
					}

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"вы не можете изменить данные группы\"";
					ostFinal << "}";
				}
			}
			else
			{
				ostringstream	ost;
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR:AJAX_updateGroupTitle: id(" + id + ") or value(" + value + ") is empty");
				}

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"пустые параметры id или value\"";
				ostFinal << "}";
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + string("]:template file json_response.htmlt was missing"));
				throw CException("Template file was missing");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": end");
			}

		}

		if(action == "AJAX_updateGroupDescription")
		{
			string			value, id;
			ostringstream	ostFinal;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == " + action + ": re-login required");
				}

				ost.str("");
				ost << "/?rand=" << GetRandom(10);
				indexPage.Redirect(ost.str());
			}

			value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("description"));
			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("groupid"));
			ostFinal.str("");

			if(value.length() && id.length()) 
			{
				if(db.Query("SELECT `id` FROM `groups` WHERE `id`=\"" + id + "\" AND `owner_id`=\"" + user.GetID() + "\";"))
				{
					db.Query("UPDATE `groups` SET `description`=\"" + value + "\"  WHERE `id`=\"" + id + "\";");

					if(!db.isError())
					{
						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"description\" : \"\"";
						ostFinal << "}";
					}
					else
					{

						{
							CLog			log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_updateGroupDescription:ERROR: updating DB");
						}

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"description\" : \"error updating DB\"";
						ostFinal << "}";
					}
				}
				else
				{
					{
						CLog			log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_updateGroupDescription:ERROR: user.id(" + user.GetID() + ") is not a group(" + id + ") owner");
					}

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"вы не можете изменить данные группы\"";
					ostFinal << "}";
				}
			}
			else
			{
				ostringstream	ost;
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR:AJAX_updateGroupDescription: id(" + id + ") or value(" + value + ") is empty");
				}

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"пустые параметры id или value\"";
				ostFinal << "}";
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + string("]:template file json_response.htmlt was missing"));
				throw CException("Template file was missing");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": end");
			}

		}

		if(action == "AJAX_createGroup")
		{
			string			title, link, description;
			long int		id;
			ostringstream	ostFinal;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == " + action + ": re-login required");
				}

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"link\" : \"/autologin?rand=" << GetRandom(10) << "\",";
				ostFinal << "\"description\" : \"re-login required\"";
				ostFinal << "}";
			}
			else
			{
				title = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
				link = toLower(CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("link")));
				link = link.substr(0, 64);
				description = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("description"));

				ostFinal.str("");
				if(title.length() == 0) 
				{
					ostringstream	ost;
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR:AJAX_createGroup: title(" + title + ") is empty");
					}

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"Группе необходимо название\"";
					ostFinal << "}";
				}
				else if(link.length() && (link.length() < 12))
				{
					// --- if link is empty it became equal to group id
					// --- maximum length group.id is 11
					// --- to avoid overlapping between group.links, it must be longer than 11 
					{
						CLog			log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:AJAX_createGroup: link[" + link + "] too short");
					}

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"link\" : \"" + link + "\",";
					ostFinal << "\"description\" : \"ссылка должна быть длиннее 10 символов\"";
					ostFinal << "}";
				}
				else if(link.find_first_not_of("abcdefghijklmnopqrstuvwxyz_1234567890") != string::npos)
				{
					{
						CLog			log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:AJAX_createGroup: link[" + link + "] contains prohibited symbols");
					}

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"link\" : \"" + link + "\",";
					ostFinal << "\"description\" : \"ссылка можеть содержать только английские буквы или цифры\"";
					ostFinal << "}";
				}
				else if(title.length() && db.Query("SELECT `id` FROM `groups` WHERE `title`=\"" + title + "\";"))
				{
					{
						CLog			log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:AJAX_createGroup: group[" + title + "] already exists");
					}
					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"группа с таким названием уже существует\"";
					ostFinal << "}";
				}
				else if(link.length() && db.Query("SELECT `id` FROM `groups` WHERE `link`=\"" + link + "\";"))
				{
					{
						CLog			log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:AJAX_createGroup: link[" + link + "] already exists");
					}
					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"такая ссылка уже существует\"";
					ostFinal << "}";
				}
				else
				{
					id = db.InsertQuery("INSERT INTO `groups` SET `link`=\"" + link + "\","
																"`title`=\"" + title + "\","
																"`description`=\"" + description + "\","
																"`owner_id`=\"" + user.GetID() + "\","
																"`eventTimestampCreation`=UNIX_TIMESTAMP()"
																";");

					if(id)
					{
						if(!link.length()) 
						{
							db.Query("UPDATE `groups` SET `link`=\"" + to_string(id) + "\" WHERE `id`=\"" + to_string(id) + "\";");
							if(db.isError())
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_createGroup:ERROR: updating `group` table");
							}
						}

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"groups\" : [" << GetGroupListInJSONFormat("SELECT * FROM `groups` WHERE `id`=\"" + to_string(id) + "\";", &db, &user) << "],";
						ostFinal << "\"description\" : \"\"";
						ostFinal << "}";

						db.InsertQuery("INSERT INTO `users_subscriptions` SET `user_id`=\"" + user.GetID() + "\","
																			"`entity_type`=\"group\","
																			"`entity_id`=\"" + to_string(id) + "\","
																			"`eventTimestamp`=UNIX_TIMESTAMP()"
																			";");

						// --- insert notification into feed
						if(db.Query("SELECT `id` FROM `feed` WHERE `userId`=\"" + user.GetID() + "\" AND `srcType`=\"user\" AND `actionTypeId`=\"65\" AND `actionId`=\"" + to_string(id) + "\";"))
						{
							CLog		log;
							log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: already subscribed [groupID: " + to_string(id) + ", userID: " + user.GetID() + "]");
						}
						else
						{
							if(db.InsertQuery("INSERT INTO `feed` (`userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"" + user.GetID() + "\", \"65\", \"" + to_string(id) + "\", NOW())"))
							{
							}
							else
							{
								CLog		log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: inserting into `feed` table");
							}
						}
					}
					else
					{

						{
							CLog			log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_createGroup:ERROR: inserting into `group` table");
						}

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"description\" : \"ошибка БД\"";
						ostFinal << "}";
					}
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + string("]:template file json_response.htmlt was missing"));
				throw CException("Template file was missing");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": end");
			}

		}


		if(action == "AJAX_updateGroupLink")
		{
			string			value, id;
			ostringstream	ostFinal;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == " + action + ": re-login required");
				}

				ost.str("");
				ost << "/?rand=" << GetRandom(10);
				indexPage.Redirect(ost.str());
			}

			value = toLower(CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value")));
			value = value.substr(0, 64);
			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("groupid"));
			ostFinal.str("");

			if(value.length() && id.length()) 
			{
				if(db.Query("SELECT `link` FROM `groups` WHERE `id`=\"" + id + "\" AND `owner_id`=\"" + user.GetID() + "\";"))
				{
					string	originalLink = db.Get(0, "link");

					if(value.length() < 10)
					{
						{
							CLog			log;
							log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]::" + action + ": link[" + value + "] is too short, must be longer than 10 symbols");
						}

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"link\" : \"" + originalLink + "\",";
						ostFinal << "\"description\" : \"ссылка должна быть минимум 10 символов\"";
						ostFinal << "}";
					}
					else if(value.find_first_not_of("abcdefghijklmnopqrstuvwxyz_-1234567890") != string::npos)
					{
						{
							CLog			log;
							log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]::" + action + ": link[" + value + "] contains prohibited symbols");
						}

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"link\" : \"" + originalLink + "\",";
						ostFinal << "\"description\" : \"ссылка можеть содержать только английские буквы или цифры\"";
						ostFinal << "}";
					}
					else if(db.Query("SELECT `id` FROM `groups` WHERE `link`=\"" + value + "\";"))
					{
						{
							CLog			log;
							log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]::" + action + ": link[" + value + "] contains prohibited symbols");
						}

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"link\" : \"" + originalLink + "\",";
						ostFinal << "\"description\" : \"группа(" + value + ") уже существует\"";
						ostFinal << "}";
					}
					else
					{
						db.Query("UPDATE `groups` SET `link`=\"" + value + "\"  WHERE `id`=\"" + id + "\";");

						if(!db.isError())
						{
							ostFinal.str("");
							ostFinal << "{";
							ostFinal << "\"result\" : \"success\",";
							ostFinal << "\"description\" : \"\"";
							ostFinal << "}";
						}
						else
						{

							{
								CLog			log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::" + action + ":ERROR: updating DB");
							}

							ostFinal.str("");
							ostFinal << "{";
							ostFinal << "\"result\" : \"error\",";
							ostFinal << "\"link\" : \"" + originalLink + "\",";
							ostFinal << "\"description\" : \"ошибка БД\"";
							ostFinal << "}";
						}
					}
				}
				else
				{
					{
						CLog			log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::" + action + ":ERROR: user.id(" + user.GetID() + ") is not a group(" + id + ") owner");
					}

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"вы не можете изменить данные группы\"";
					ostFinal << "}";
				}
			}
			else
			{
				ostringstream	ost;
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR:" + action + ": id(" + id + ") or value(" + value + ") is empty");
				}

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"пустые параметры id или value\"";
				ostFinal << "}";
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + string("]:template file json_response.htmlt was missing"));
				throw CException("Template file was missing");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == " + action + ": end");
			}

		}


		{
			CLog	log;
			ostringstream	ost;

			ost.str("");
			ost << __func__ << "[" << __LINE__ << "]: end (action's == \"" << action << "\") condition";
			log.Write(DEBUG, ost.str());
		}

		indexPage.OutTemplate();

	}
/*
	catch(CExceptionRedirect &c) {
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << string(__func__) + ":: catch CRedirectHTML: exception used for redirection";
		log.Write(DEBUG, ost.str());

		c.SetDB(&db);

		if(!indexPage.SetTemplate(c.GetTemplate())) {

			ost.str("");
			ost << string(__func__) + ":: catch CRedirectHTML: ERROR, template redirect.htmlt not found";
			log.Write(ERROR, ost.str());

			throw CException("Template file was missing");
		}

		indexPage.RegisterVariableForce("content", "redirect page");
		indexPage.OutTemplate();

	}
*/
	catch(CExceptionHTML &c)
	{
		CLog	log;

		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);

		log.Write(DEBUG, string(__func__) + ": catch CExceptionHTML: DEBUG exception reason: [", c.GetReason(), "]");

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
		CLog 	log;

		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}

		log.Write(ERROR, string(__func__) + ": catch CException: exception: ERROR  ", c.GetReason());

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(-1);
	}
	catch(exception& e)
	{
		CLog 	log;
		log.Write(PANIC, string(__func__) + ": catch(exception& e): catch standard exception: ERROR  ", e.what());

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

