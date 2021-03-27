#include "account.h"

int main()
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	string			action, partnerID;
	CMysql			db;
	struct timeval	tv;

	MESSAGE_DEBUG("", "", __FILE__);

	signal(SIGSEGV, crash_handler); 

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);

	try
	{

		indexPage.ParseURL();

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect() < 0)
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);


		action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));

		MESSAGE_DEBUG("", "", "action = " + action);

		// --- generate common parts
		{
			// --- it has to be run before session, because session relay upon Apache environment variable
			if(RegisterInitialVariables(&indexPage, &db, &user))
			{
			}
			else
			{
				MESSAGE_ERROR("", "", "RegisterInitialVariables failed, throwing exception");
				throw CExceptionHTML("environment variable error");
			}

			//------- Generate session
			action = GenerateSession(action, &indexPage, &db, &user);
			action = LogoutIfGuest(action, &indexPage, &db, &user);
		}
	// ------------ end generate common parts

		MESSAGE_DEBUG("", "", "pre-condition action = " + action);

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

		if(action == "AJAX_changeLogin")
		{
			auto		    loginFromUser = ""s;
			auto			success_message = ""s;
			auto			error_message = ""s;

			MESSAGE_DEBUG("", "", "start");

			loginFromUser = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("login"));

			if(loginFromUser.length() >= MIN_LOGIN_LENGTH)
			{
				if(loginFromUser.find_first_of(" \\/%?+-,*&^$#!абвгдеёжзийклмнопрстуфхцчшщьыъэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЬЫЪЭЮЯ") == string::npos)
				{
					if(loginFromUser.find_first_not_of(" 1234567890") != string::npos)
					{
						if(db.Query("SELECT `id` FROM `users` WHERE `login`=\"" + loginFromUser + "\" AND `id`!=\"" + user.GetID() + "\";"))
						{
							error_message = gettext("login") + " "s + gettext("already exists");
							MESSAGE_DEBUG("", action, error_message);
						}
						else
						{

							db.Query("UPDATE `users` SET `login`=\"" + loginFromUser + "\" WHERE `id`=\"" + user.GetID() + "\";");
							if(!db.isError())
							{
								success_message = "\"userLogin\":\"" + loginFromUser + "\"";
							}
							else
							{
								error_message = gettext("SQL syntax error");
								MESSAGE_ERROR("", action, error_message);
							}
						}
					}
					else
					{
						error_message = gettext("Login must contain alphabet characters");
						MESSAGE_DEBUG("", action, error_message);
					}
				}
				else
				{
					error_message = gettext("Login contains illegal characters");
					MESSAGE_DEBUG("", action, error_message);
				}
			}
			else
			{
				error_message = gettext(("Login must be at least " + to_string(MIN_LOGIN_LENGTH) + " characters").c_str());
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", "", "finish");
		}

		if(action == "AJAX_changeUserSex")
		{
			ostringstream   ostResult;
			string		    userSex;

			MESSAGE_DEBUG("", "", "start");

			ostResult.str("");
	/*		if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ": re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"re-login required\"}";
			}
			else
	*/
			{
				userSex = indexPage.GetVarsHandler()->Get("userSex");

				if((userSex == "male") || (userSex == "female"))
				{

					db.Query("UPDATE `users` SET `sex`=\"" + userSex + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(!db.isError())
					{
						ostResult << "{" 
								  << "\"result\":\"success\","
								  << "\"userLogin\":\"" << userSex << "\""
								  << "}";
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ":ERROR: updating users table [" + db.GetErrorMessage() + "]");
						}

						ostResult << "{" 
								  << "\"result\":\"error\","
								  << "\"description\":\"Внутренняя ошибка БД\""
								  << "}";
					}

				}
				else
				{
					
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ":ERROR: wrong userSex[" + userSex + "]");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"некорректный параметер\""
							  << "}";
				}

			} // --- if(user.GetLogin() == "Guest")


			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_changeUserSex.htmlt"))

			MESSAGE_DEBUG("", "", "finish");
		}

		if(action == "AJAX_changeUserBirthday")
		{
			ostringstream   ostResult;
			string		    userBirthday;

			MESSAGE_DEBUG("", "", "start");

			ostResult.str("");
	/*		if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ": re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"re-login required\"}";
			}
			else
	*/
			{
				userBirthday = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(std::regex_match(userBirthday, std::regex("^[[:digit:]]{1,2}/[[:digit:]]{1,2}/[[:digit:]]{2,4}$")))
				{

					db.Query("UPDATE `users` SET `birthday`=\"" + userBirthday + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(!db.isError())
					{
						ostResult << "{" 
								  << "\"result\":\"success\","
								  << "\"birthday\":\"" << userBirthday << "\""
								  << "}";
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ":ERROR: updating users table [" + db.GetErrorMessage() + "]");
						}

						ostResult << "{" 
								  << "\"result\":\"error\","
								  << "\"description\":\"Внутренняя ошибка БД\""
								  << "}";
					}

				}
				else
				{
					
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ":ERROR: wrong date format dd/mm/yy userBirthday[" + userBirthday + "]");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"некорректный формат даты\""
							  << "}";
				}

			} // --- if(user.GetLogin() == "Guest")


			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_changeUserBirthday.htmlt"))

			MESSAGE_DEBUG("", "", "finish");
		}

		if(action == "AJAX_updateCity")
		{
			ostringstream   ostResult;
			string		    city;

			MESSAGE_DEBUG("", "", "start");

			ostResult.str("");
	/*		if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ": re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"re-login required\"}";
			}
			else
	*/
			{
				string::size_type 	openBrace;
				string				cityID = "";

				city = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				openBrace = city.find("(");
				if(openBrace != string::npos)
				{
					city = city.substr(0, openBrace);
				}
				trim(city);

				cityID = GetGeoLocalityIDByCityAndRegion("", city, &db);
				if(cityID.empty()) cityID = "0";

				if(cityID.length())
				{

					db.Query("UPDATE `users` SET `geo_locality_id`=\"" + cityID + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(!db.isError())
					{
						ostResult << "{" 
								  << "\"result\":\"success\","
								  << "\"city\":\"" << city << "\""
								  << "}";
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ":ERROR: updating users table [" + db.GetErrorMessage() + "]");
						}

						ostResult << "{" 
								  << "\"result\":\"error\","
								  << "\"description\":\"Внутренняя ошибка БД\""
								  << "}";
					}

				}
				else
				{
					
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ": city is empty");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"город не указан\""
							  << "}";
				}

			} // --- if(user.GetLogin() == "Guest")


			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_updateCity.htmlt"))

			MESSAGE_DEBUG("", "", "finish");
		}

		if((action == "AJAX_editProfile_setBirthdayPrivate") || (action == "AJAX_editProfile_setBirthdayPublic"))
		{
			ostringstream   ostResult;
			string		    birthdayAccess = (action == "AJAX_editProfile_setBirthdayPrivate" ? "private" : "public");

			MESSAGE_DEBUG("", "", "start");

			ostResult.str("");
	/*		if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ": re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"re-login required\"}";
			}
			else
	*/
			{
				db.Query("UPDATE `users` SET `birthdayAccess`=\"" + birthdayAccess + "\" WHERE `id`=\"" + user.GetID() + "\";");
				if(!db.isError())
				{
					ostResult << "{" 
							  << "\"result\":\"success\","
							  << "\"birthday\":\"" << birthdayAccess << "\""
							  << "}";
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == " + action + ":ERROR: updating users table [" + db.GetErrorMessage() + "]");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"Внутренняя ошибка БД\""
							  << "}";
				}
			} // --- if(user.GetLogin() == "Guest")


			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_editProfile_setBirthdayPrivate.htmlt"))

			MESSAGE_DEBUG("", "", "finish");
		}

		if(action == "AJAX_updatePassportSeries")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `passport_series`=\"" + new_value + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updatePassportNumber")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `passport_number`=\"" + new_value + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateForeignPassportNumber")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `foreign_passport_number`=\"" + new_value + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updatePassportIssueDate")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `passport_issue_date`=STR_TO_DATE(\"" + new_value + "\",\"%d/%m/%Y\") WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateForeignPassportExpirationDate")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `foreign_passport_expiration_date`=STR_TO_DATE(\"" + new_value + "\",\"%d/%m/%Y\") WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updatePassportIssueAuthority")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `passport_issue_authority`=\"" + new_value + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateBirthday")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `birthday`=STR_TO_DATE(\"" + new_value + "\",\"%d/%m/%Y\") WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateFirstName_En")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `first_name_en`=\"" + new_value + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateLastName_En")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `last_name_en`=\"" + new_value + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateMiddleName_En")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `middle_name_en`=\"" + new_value + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateFirstName_Ru")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `name`=\"" + new_value + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateLastName_Ru")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `nameLast`=\"" + new_value + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateMiddleName_Ru")
		{
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			{
				string			template_name = "json_response.htmlt";
				string			error_message = "";
				string			new_value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

				if(new_value.length())
				{
					db.Query("UPDATE `users` SET `nameMiddle`=\"" + new_value + "\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", "", error_message);
					}
					if(error_message.empty())
					{
					}
					else
					{
						MESSAGE_DEBUG("", action, "unable to set new value (action/value = " + action + "/" + new_value + ")");
					}
				}
				else
				{
					error_message = "Поле не должно быть пустым";
					MESSAGE_DEBUG("", action, "user.id(" + user.GetID() + ") didn't set new_value");
				}

				if(error_message.empty())
				{
					ostResult << "{\"result\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "failed");
					ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_changeEmail")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;
			auto			email = CheckHTTPParam_Email(indexPage.GetVarsHandler()->Get("login"));


			if(email.length())
			{
				if(db.Query("SELECT `id` FROM `users` WHERE `email`=" + quoted(email) + ";"))
				{
					error_message = gettext("email already exists");
				}
				else
				{
					CMailLocal	mail;
					string		token = GetRandom(20);

					db.Query("DELETE FROM `email_change_tokens` WHERE `user_id`=" + quoted(user.GetID()) + ";");
					db.Query("INSERT INTO `email_change_tokens` SET `token`=\"" + token + "\", `new_email`=\"" + email + "\", `user_id`=\"" + user.GetID() + "\", `eventTimestamp`=NOW();");
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", action, "fail to insert activator for user.login(" + user.GetLogin() + ")");
					}
					else
					{
						indexPage.RegisterVariableForce("login", user.GetLogin());
						indexPage.RegisterVariableForce("name", user.GetName());
						indexPage.RegisterVariableForce("nameLast", user.GetNameLast());
						indexPage.RegisterVariableForce("token", token);
						mail.SendToEmail(email, user.GetLogin(), "change_email", indexPage.GetVarsHandler(), &db);
					}
				}
			}
			else
			{
				error_message = gettext("email is empty");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_activateEmailChangeToken")
		{

			MESSAGE_DEBUG("", action, "start");

			ostringstream	ostResult;
			auto			template_name = "json_response.htmlt"s;
			auto			error_message = ""s;
			auto			success_message = ""s;
			auto			token = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("token"));

			ostResult.str("");

			if(token.length())
			{
				if(db.Query("SELECT * FROM `email_change_tokens` WHERE `token`=" + quoted(token) + " AND `user_id`=" + quoted(user.GetID()) + ";"))
				{
					auto	new_email = db.Get(0, "new_email");

					if(db.Query("SELECT `email`,`login` FROM `users` WHERE `id`=" + quoted(user.GetID()) + ";"))
					{
						auto	old_email = db.Get(0, "email");
						auto	old_login = db.Get(0, "login");

						db.Query("UPDATE `users` SET `email`=" + quoted(new_email) + " WHERE `id`=" + quoted(user.GetID()) + ";");

						if(old_login == old_email)
							db.Query("UPDATE `users` SET `login`=" + quoted(new_email) + " WHERE `id`=" + quoted(user.GetID()) + ";");
						else
						{
							MESSAGE_DEBUG("", action, "login differ from email, don't update login");
						}

						db.Query("DELETE FROM `email_change_tokens` WHERE `user_id`=" + quoted(user.GetID()) + ";");

						success_message = ", \"email\": "s + quoted(new_email);
					}
					else
					{
						error_message = gettext("user not found");
						MESSAGE_DEBUG("", action, error_message);
					}
				}
				else
				{
					error_message = gettext("token not found");
					MESSAGE_DEBUG("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", action, error_message);
			}

			if(error_message.empty())
			{
				ostResult << "{\"result\":\"success\"" << success_message << "}";
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_sendPhoneConfirmationSMS")
		{
			auto			error_message = ""s;

			auto			country_code = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("country_code"));
			auto			phone_number = SymbolReplace_KeepDigitsOnly(CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("phone_number")));

			MESSAGE_DEBUG("", action, "start");

			if(country_code.length() && phone_number.length())
			{
				error_message = SendPhoneConfirmationCode(country_code, phone_number, indexPage.SessID_Get_FromHTTP(), &db, &user);
			}
			else
			{
				error_message = gettext("phone number is incorrect");
				MESSAGE_ERROR("", action, "country_code(" + country_code + ") or phone_number(" + phone_number + ") is empty");
			}

			AJAX_ResponseTemplate(&indexPage, "", error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_confirmPhoneNumber")
		{
			MESSAGE_DEBUG("", action, "start");

			auto	confirmation_code = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("confirmation_code"));
			vector<pair<string, string>>	error_message;

			if(db.Query("SELECT * FROM `phone_confirmation` WHERE `session`=\"" + indexPage.SessID_Get_FromHTTP() + "\";"))
			{
				auto	country_code = db.Get(0, "country_code");
				auto	phone_number = db.Get(0, "phone_number");

				error_message = CheckPhoneConfirmationCode(confirmation_code, indexPage.SessID_Get_FromHTTP(), &db, &user);

				if(error_message.size() == 0)
				{
					db.Query("UPDATE `users` SET `country_code`=\"" + country_code + "\", `phone`=\"" + phone_number + "\" , `is_phone_confirmed`=\"Y\" WHERE `id`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						error_message.push_back(make_pair("description", gettext("SQL syntax error")));
						MESSAGE_ERROR("", "", error_message[0].second);
					}
					else
					{
						// --- good2go
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "don't update phone number");
				}
			}
			else
			{
				error_message.push_back(make_pair("description", gettext("token expired")));
				MESSAGE_ERROR("", "", error_message[0].second);
			}

			AJAX_ResponseTemplate(&indexPage, "", error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		// --- helpdesk
		if(action == "AJAX_updateHelpdeskSMSSubscription")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			auto			new_value	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

			if(new_value.length())
			{
				auto	good2go = false;

				if(new_value == "Y")
				{
					if(db.Query("SELECT `is_phone_confirmed` FROM `users` WHERE `id`=" + quoted(user.GetID()) + ";"))
					{
						auto	is_phone_confirmed = db.Get(0, 0);

						if(is_phone_confirmed == "Y")
						{
							good2go = true;
						}
						else
						{
							error_message = gettext("You need to enter phone number in user profile");
							MESSAGE_DEBUG("", action, error_message);
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
					good2go = true;
				}

				if(good2go)
				{
					db.Query("UPDATE `users` SET "
										"`helpdesk_subscription_S1_sms`=" + quoted(new_value) + ","
										"`helpdesk_subscription_S2_sms`=" + quoted(new_value) + ","
										"`helpdesk_subscription_S3_sms`=" + quoted(new_value) + ","
										"`helpdesk_subscription_S4_sms`=" + quoted(new_value) + " "
										"WHERE `id`=" + quoted(user.GetID()) + ";");

					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", action, error_message);
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "previous errors prevent to change subscription mode");
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

		if(action == "AJAX_updateHelpdeskEmailSubscription")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			auto			new_value	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

			if(new_value.length())
			{
				db.Query("UPDATE `users` SET "
									"`helpdesk_subscription_S1_email`=" + quoted(new_value) + ","
									"`helpdesk_subscription_S2_email`=" + quoted(new_value) + ","
									"`helpdesk_subscription_S3_email`=" + quoted(new_value) + ","
									"`helpdesk_subscription_S4_email`=" + quoted(new_value) + " "
									"WHERE `id`=" + quoted(user.GetID()) + ";");

				if(db.isError())
				{
					error_message = gettext("SQL syntax error");
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

		if(action == "AJAX_updateTimecardNotifications")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			auto			new_value	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

			if(new_value.length())
			{
				db.Query("UPDATE `users` SET "
									"`pending_approval_notification_timecard`=" + quoted(new_value) + " "
									"WHERE `id`=" + quoted(user.GetID()) + ";");

				if(db.isError())
				{
					error_message = gettext("SQL syntax error");
					MESSAGE_ERROR("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("parameters incorrect");
				MESSAGE_ERROR("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateBTNotifications")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			auto			new_value	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

			if(new_value.length())
			{
				db.Query("UPDATE `users` SET "
									"`pending_approval_notification_bt`=" + quoted(new_value) + " "
									"WHERE `id`=" + quoted(user.GetID()) + ";");

				if(db.isError())
				{
					error_message = gettext("SQL syntax error");
					MESSAGE_ERROR("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("parameters incorrect");
				MESSAGE_ERROR("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}


		if(action == "AJAX_getFaq")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			success_message += quoted("faq"s) + ":[" + GetFAQInJSONFormat("SELECT * FROM `faq` WHERE `role`=" + quoted(user.GetType()) + ";", &db, &user) + "]";

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_sessionHandshake")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = ""s;
			auto			error_message = ""s;

			if(db.Query("SELECT `previous_sessid` FROM `sessions` WHERE `id`=\"" + indexPage.SessID_Get_FromHTTP() + "\";"))
			{
				auto	previous_sessid = db.Get(0, 0);

				db.Query("UPDATE `sessions` SET `remove_flag`=\"Y\", `remove_flag_timestamp`=UNIX_TIMESTAMP() WHERE `id`=" + quoted(previous_sessid) + ";");
				if(db.isError()) error_message = db.GetErrorMessage();
			}
			else
			{
				error_message = gettext("session not found");
				MESSAGE_ERROR("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		MESSAGE_DEBUG("", action, "finish condition")

		indexPage.OutTemplate();

	}
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
		log.Write(ERROR, string(__func__) + ": catch(exception& e): catch standard exception: ERROR  ", e.what());

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

