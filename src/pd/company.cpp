#include "company.h"

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

		if((action == "view_company_profile") || (action == "getCompanyWall"))
		{
			ostringstream	ost;
			string			sessid, id, link;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == view_company_profile: start");
			}

/*
			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == view_company_profile re-login required");
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
					if(db.Query("SELECT `id` FROM `company` WHERE `link`=\"" + link + "\";"))
					{
						id = db.Get(0, "id");
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == view_company_profile:ERROR: company.link(" + link + ") not found");
						}
					}
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == view_company_profile:ERROR: id and link are empty");
					}

				}
			}

			if(!link.length())
			{
				if(id.length())
				{
					if(db.Query("SELECT `link` FROM `company` WHERE `id`=\"" + id + "\";"))
					{
						link = db.Get(0, "link");
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == view_company_profile:ERROR: company.id(" + id + ") not found");
						}
					}
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == view_company_profile:ERROR: id and link are empty");
					}

				}
			}

			if(id.length()) indexPage.RegisterVariableForce("id", id);
			if(link.length()) indexPage.RegisterVariableForce("link", link);

			if(!indexPage.SetTemplate("view_company_profile.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + string("]:template file view_company_profile.htmlt was missing"));
				throw CException("Template file was missing");
			}
		}

	    if(action == "AJAX_getCompanyWall")
	    {
			MESSAGE_DEBUG("", action, "start");

	        int     currPage = 0, newsOnSinglePage = 0;

			auto	companies_i_own_vector	= GetValuesFromDB("SELECT `id` FROM `company` WHERE `isBlocked`=\"N\" AND `admin_userID`=\"" + user.GetID() + "\";", &db);
	        auto	strNewsOnSinglePage 	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("NewsOnSinglePage"));
	        auto	strPageToGet        	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("page"));
	        auto	companyLink				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("link"));
	        auto	companyID				= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
	        auto	result					= ""s;
	        auto	success_message			= ""s;
	        auto	error_message			= ""s;

	        if(strPageToGet.empty()) strPageToGet = "0";

	        {
	            CLog    log;
	            log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_getCompanyWall: page " + strPageToGet + " requested");
	        }

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
	                log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_getCompanyWall: re-login required");
	            }

	            ost.str("");
	            ost << "/?rand=" << GetRandom(10);
	            indexPage.Redirect(ost.str());
	        }
	*/
			if(companyLink.length() && db.Query("SELECT * FROM `company` WHERE `link`=\"" + companyLink + "\" AND `isblocked`=\"N\";"))
				result = GetNewsFeedInJSONFormat(" `feed`.`srcType`=\"company\" AND `feed`.`userId` IN (SELECT `id` FROM `company` WHERE `link`=\"" + companyLink + "\" AND `isBlocked`=\"N\") ", currPage, newsOnSinglePage, &user, &db);
			else if(companyID.length() && db.Query("SELECT * FROM `company` WHERE `id`=\"" + companyID + "\" AND `isblocked`=\"N\";"))
				result = GetNewsFeedInJSONFormat(" `feed`.`srcType`=\"company\" AND `feed`.`userId` IN (SELECT `id` FROM `company` WHERE `id`=\"" + companyID + "\" AND `isBlocked`=\"N\") ", currPage, newsOnSinglePage, &user, &db);
			else
			{
                MESSAGE_ERROR("", action, "company link(" + companyLink + ")/id(" + companyID + ") not found");
			}

			success_message =
								"\"my_companies\":[" + join(companies_i_own_vector) + "],"
								"\"feed\":[" + result + "]"
								;

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);
			
			MESSAGE_DEBUG("", action, "finish");
	    }

		if(action == "JSON_getCompanyProfile")
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == JSON_getCompanyProfile: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == JSON_getCompanyProfile: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		companyID;

				companyID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				if(companyID.length())
				{
					ostringstream		ost;

					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`=\"" << companyID << "\" and `isBlocked`='N';";

					ostResult << "{\"result\":\"success\",\"companies\":[" << GetCompanyListInJSONFormat(ost.str(), &db, &user, false) << "]}";
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == JSON_getCompanyProfile:ERROR: in companyID [", companyID, "]");

					ostResult << "{\"result\":\"error\",\"description\":\"ERROR in companyID\",\"companies\":[]}";
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
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == JSON_getCompanyProfile: start");
			}
		}

		if(action == "JSON_getCompanyProfileAndMyVacancies")
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == JSON_getCompanyProfileAndMyVacancies: start");
			}

			ostResult.str("");
/*			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == JSON_getCompanyProfileAndMyVacancies: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
*/
			{
				string		companyID;

				companyID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				if(companyID.length())
				{
					ostringstream		ost;

					ost.str("");
					ost << "SELECT * FROM `company` WHERE `id`=\"" << companyID << "\" and `isBlocked`='N';";

					ostResult << "{\"result\":\"success\","
								 "\"companies\":[" << GetCompanyListInJSONFormat(ost.str(), &db, &user, false) << "],"
						  		 "\"myvacancies\":[" << GetCandidatesListAppliedToVacancyInJSONFormat("SELECT * FROM `company_candidates` WHERE (`vacancy_id` in (SELECT `id` FROM `company_vacancy` WHERE `company_id`=\"" + companyID + "\")) AND `user_id`=\"" + user.GetID() + "\";", &db) << "],"
								 "\"users\":[" << GetUserListInJSONFormat("SELECT * FROM `users` WHERE `id`=\"" + user.GetID() + "\" AND `isblocked`=\"N\";", &db, &user) << "]"
								 "}";
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == JSON_getCompanyProfileAndMyVacancies:ERROR: in companyID [", companyID, "]");

					ostResult << "{\"result\":\"error\",\"description\":\"ERROR in companyID\",\"companies\":[]}";
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
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == JSON_getCompanyProfileAndMyVacancies: start");
			}
		}


		if(action == "AJAX_removeOpenVacancy")
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_removeOpenVacancy: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_removeOpenVacancy: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		id;

				id = indexPage.GetVarsHandler()->Get("id");
				try { 
					id = to_string(stol(id.c_str()));
				} catch(...) {
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_removeOpenVacancy:ERROR: can't convert OpenVacancyID to number");
					}
				}

				if(db.Query("SELECT * FROM `company_vacancy` WHERE `id`=\"" + id + "\";"))
				{
					string 		companyID = db.Get(0, "company_id");

					if(db.Query("SELECT * FROM `company` WHERE `admin_userID`=\"" + user.GetID() + "\" AND `id`=\"" + companyID + "\";"))
					{
						// --- IMPORTANT !!! Don't change order.
						// --- delete user notifications
						db.Query("DELETE FROM `users_notification` WHERE (`actionTypeId`=\"59\") AND (`actionID` in (SELECT `id` FROM `company_candidates` WHERE `vacancy_id`=\"" + id + "\"));");
						// --- delete applied/rejected candidates
						db.Query("DELETE FROM `company_candidates` WHERE `vacancy_id`=\"" + id + "\";");
						// --- delete vacancy
						db.Query("DELETE FROM `company_vacancy` WHERE `id`=\"" + id + "\";");

						ostResult << "{\"result\":\"success\"}";
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_removeOpenVacancy:ERROR: userID[" + user.GetID() + "] not allowed remove openVacancyID[" + id + "], due to he is not company[" + companyID + "] admin");
						}

						ostResult << "{\"result\":\"error\",\"description\":\"Вам нельзя удалить эту вакансию\"}";
					}
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_removeOpenVacancy:ERROR: openVacancyID[" + id + "] not found in DB");
					}
					ostResult << "{\"result\":\"error\",\"description\":\"Вакансия не найдена\"}";
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
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_removeOpenVacancy: start");
			}
		}



		if(action == "AJAX_precreateNewOpenVacancy")
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_precreateNewOpenVacancy: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string			company_position_id;
				string			title					= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
				string			geo_locality			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("geo_locality"));
				string			employmenttype			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("employmenttype"));
				string			question1				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question1"));
				string			question1answers		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question1answers"));
				string			question1answer1		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question1answer1"));
				string			question1answer2		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question1answer2"));
				string			question1answer3		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question1answer3"));
				string			question2				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question2"));
				string			question2answers		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question2answers"));
				string			question2answer1		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question2answer1"));
				string			question2answer2		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question2answer2"));
				string			question2answer3		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question2answer3"));
				string			question3				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question3"));
				string			question3answers		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question3answers"));
				string			question3answer1		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question3answer1"));
				string			question3answer2		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question3answer2"));
				string			question3answer3		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question3answer3"));
				string			language1				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("language1"));
				string			language2				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("language2"));
				string			language3				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("language3"));
				string			skill1					= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("skill1"));
				string			skill2					= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("skill2"));
				string			skill3					= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("skill3"));
				string			description				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("description"));
				string			publish_period			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("publish"));
				string			company_id				= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("company_id"));
				string			minsalary				= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("minsalary"));
				string			maxsalary				= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("maxsalary"));
				string			month					= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("month"));
				string			geo_locality_id			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("geo_locality_id"));
				string			language1_id = "0";
				string			language2_id = "0";
				string			language3_id = "0";
				string			skill1_id = "0";
				string			skill2_id = "0";
				string			skill3_id = "0";
				string			publish_finish;


				if(!minsalary.length()) minsalary = "0";
				if(!maxsalary.length()) maxsalary = "0";

				if(publish_period == "week")
				{
					publish_finish = "(NOW() + INTERVAL 7 DAY)";
				}
				else if(publish_period == "month")
				{
					publish_finish = "(NOW() + INTERVAL 31 DAY)";
				}
				else
				{
					publish_finish = "2000-01-01";
				}

				if((geo_locality_id == "0") || (geo_locality_id.length() == 0))
				{
					smatch			matchResult;
					string			regionName, cityName;
					regex			regexGeoLocality("(.*)[[:s:]]*\\((.*)\\)");

					if(regex_search(geo_locality, matchResult, regexGeoLocality))
					{
			            cityName = matchResult[1];
			            regionName = matchResult[2];
					}
					else
						cityName = geo_locality;

					trim(cityName);
					trim(regionName);

		            if(cityName.length())
						geo_locality_id = GetGeoLocalityIDByCityAndRegion(regionName, cityName, &db);
				}

				company_position_id = GetCompanyPositionIDByTitle(title, &db);
				language1_id = GetLanguageIDByTitle(language1, &db);
				language2_id = GetLanguageIDByTitle(language2, &db);
				language3_id = GetLanguageIDByTitle(language3, &db);
				skill1_id = GetSkillIDByTitle(skill1, &db);
				skill2_id = GetSkillIDByTitle(skill2, &db);
				skill3_id = GetSkillIDByTitle(skill3, &db);

				if(!company_id.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: companyid is empty");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо указать компанию-работодателя\""
							  << "}";
				}
				else if(!company_position_id.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: title is empty");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить должность\""
							  << "}";
				}
				else if(!month.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: month is empty");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать месяц выхода на работу\""
							  << "}";
				}
				else if(!geo_locality_id.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: geo_locality_id is empty");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать город работы выхода на работу\""
							  << "}";
				}
				else if(!employmenttype.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: employmenttype is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить тип трудоустройства\""
							  << "}";
				}
				else if(!question1.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question1 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить квалификационный вопрос 1\""
							  << "}";
				}
				else if(!question1answers.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question1answers is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать правильный ответ на первый вопрос\""
							  << "}";
				}
				else if(!question1answer1.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question1answer1 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на первый вопрос\""
							  << "}";
				}
				else if(!question1answer2.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question1answer2 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на первый вопрос\""
							  << "}";
				}
				else if(!question1answer3.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question1answer3 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на первый вопрос\""
							  << "}";
				}
				else if(!question2.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question2 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить квалификационный вопрос 2\""
							  << "}";
				}
				else if(!question2answers.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question2answers is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать правильный ответ на второй вопрос\""
							  << "}";
				}
				else if(!question2answer1.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question2answer1 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на второй вопрос\""
							  << "}";
				}
				else if(!question2answer2.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question2answer2 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на второй вопрос\""
							  << "}";
				}
				else if(!question2answer3.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question2answer3 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на второй вопрос\""
							  << "}";
				}
				else if(!question3.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question3 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить квалификационный вопрос 3\""
							  << "}";
				}
				else if(!question3answers.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question3answers is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать правильный ответ на третий вопрос\""
							  << "}";
				}
				else if(!question3answer1.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question3answer1 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на третий вопрос\""
							  << "}";
				}
				else if(!question3answer2.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question3answer2 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на третий вопрос\""
							  << "}";
				}
				else if(!question3answer3.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: question3answer3 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на третий вопрос\""
							  << "}";
				}
				else if(!publish_period.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: publish is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать длительность публикации\""
							  << "}";
				}
				else
				{
					if(db.Query("SELECT `id` FROM `company` WHERE `id`=\"" + company_id + "\" AND `admin_userID`=\"" + user.GetID() + "\";"))
					{
						long int	openVacancyID = 0;

						openVacancyID = db.InsertQuery("INSERT INTO `company_vacancy` SET "
														"`company_id`=\"" + company_id + "\","
														"`company_position_id`=\"" + company_position_id + "\","
														"`geo_locality_id`=\"" + geo_locality_id + "\","
														"`salary_min`=\"" + minsalary + "\","
														"`salary_max`=\"" + maxsalary + "\","
														"`start_month`=\"" + month + "\","
														"`work_format`=\"" + employmenttype + "\","
														"`description`=\"" + description + "\","
														"`question1`=\"" + question1 + "\","
														"`answer11`=\"" + question1answer1 + "\","
														"`answer12`=\"" + question1answer2 + "\","
														"`answer13`=\"" + question1answer3 + "\","
														"`correct_answer1`=\"" + question1answers + "\","
														"`question2`=\"" + question2 + "\","
														"`answer21`=\"" + question2answer1 + "\","
														"`answer22`=\"" + question2answer2 + "\","
														"`answer23`=\"" + question2answer3 + "\","
														"`correct_answer2`=\"" + question2answers + "\","
														"`question3`=\"" + question3 + "\","
														"`answer31`=\"" + question3answer1 + "\","
														"`answer32`=\"" + question3answer2 + "\","
														"`answer33`=\"" + question3answer3 + "\","
														"`correct_answer3`=\"" + question3answers + "\","
														"`language1_id`=\"" + language1_id + "\","
														"`language2_id`=\"" + language2_id + "\","
														"`language3_id`=\"" + language3_id + "\","
														"`skill1_id`=\"" + skill1_id + "\","
														"`skill2_id`=\"" + skill2_id + "\","
														"`skill3_id`=\"" + skill3_id + "\","
														"`publish_finish`=" + publish_finish + ","
														"`publish_period`=\"" + publish_period + "\""
														";");
						if(db.isError())
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: inserting users_notification table");
							}

							ostResult.str("");
							ostResult << "{" 
									  << "\"result\":\"error\","
									  << "\"description\":\"Ошибка БД.\""
									  << "}";
						}
						else
						{
							ostResult.str("");
							ostResult << "{" 
									  << "\"result\":\"success\","
									  << "\"id\":\"" + to_string(openVacancyID) + "\","
									  << "\"open_vacancies\":[" + GetOpenVacanciesInJSONFormat("SELECT * FROM `company_vacancy` WHERE `company_id`=\"" + company_id + "\";", &db) + "]"
									  << "}";
						}
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy:ERROR: user[" + user.GetID() + "] not allowed create vacancy in companyID[" + company_id + "]");
						}

						ostResult.str("");
						ostResult << "{" 
								  << "\"result\":\"error\","
								  << "\"description\":\"Вы не можете создавать вакансии.\""
								  << "}";
					}
				}
			} // --- if(user.GetLogin() == "Guest")


			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_precreateNewOpenVacancy.htmlt"))

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_precreateNewOpenVacancy: end");
			}
		}   // if(action == "AJAX_precreateNewOpenVacancy")

		if(action == "AJAX_rejectCandidate")
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_rejectCandidate: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_rejectCandidate: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string			company_candidate_id	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
				string			vacancy_id = "";
				string			company_id = "";
				string			user_id = "";

				if(db.Query("SELECT `user_id`,`vacancy_id` FROM `company_candidates` WHERE `id`=\"" + company_candidate_id + "\";"))
				{
					vacancy_id = db.Get(0, "vacancy_id");
					user_id = db.Get(0, "user_id");

					if(db.Query("SELECT `company_id` FROM `company_vacancy` WHERE `id`=\"" + vacancy_id + "\";"))
					{
						company_id = db.Get(0, "company_id");

						if(db.Query("SELECT `id` FROM `company` WHERE `id`=\"" + company_id + "\" AND `admin_userID`=\"" + user.GetID() + "\";"))
						{

							db.Query("UPDATE `company_candidates` SET `status`=\"rejected\",`eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + company_candidate_id + "\";");

							if(db.isError())
							{
								{
									CLog	log;
									log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_rejectCandidate:ERROR: updating company_candidates table");
								}

								ostResult.str("");
								ostResult << "{" 
										  << "\"result\":\"error\","
										  << "\"description\":\"Ошибка БД.\""
										  << "}";
							}
							else
							{
								ostResult.str("");
								ostResult << "{" 
										  << "\"result\":\"success\","
										  << "\"open_vacancies\":[" + GetOpenVacanciesInJSONFormat("SELECT * FROM `company_vacancy` WHERE `company_id`=\"" + company_id + "\";", &db) + "]"
										  << "}";
							}

							if(db.InsertQuery("INSERT INTO `users_notification` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user_id + "\", \"59\", \"" + vacancy_id + "\", UNIX_TIMESTAMP());"))
							{
							}
							else
							{
								{
									CLog	log;
									log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_rejectCandidate:ERROR: inserting into user notification");
								}
							}

						}
						else
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_rejectCandidate:ERROR: user[" + user.GetID() + "] not allowed reject candidate[" + company_candidate_id + "]");
							}

							ostResult.str("");
							ostResult << "{" 
									  << "\"result\":\"error\","
									  << "\"description\":\"Вы не можете отказать кандидату.\""
									  << "}";
						}

					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_rejectCandidate:ERROR: user[" + user.GetID() + "] company_vacancy not found by id[" + vacancy_id + "]");
						}

						ostResult.str("");
						ostResult << "{" 
								  << "\"result\":\"error\","
								  << "\"description\":\"Вакансия не найдена.\""
								  << "}";
					}

				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_rejectCandidate:ERROR: user[" + user.GetID() + "] company_candidate not found by id[" + company_candidate_id + "]");
					}

					ostResult.str("");
					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"Такой кандидат не аплаился на позицию.\""
							  << "}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_rejectCandidate.htmlt"))

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_rejectCandidate: end");
			}
		}   // if(action == "AJAX_rejectCandidate")


		if(action == "AJAX_getCandidatesAppliedToPosition")
		{
			ostringstream   ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_getCandidatesAppliedToPosition: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string			open_vacancy_id			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				// --- only company admin can get candidate list
				if(db.Query("SELECT `company_id` FROM `company_vacancy` WHERE `id`=\"" + open_vacancy_id + "\";"))
				{
					string		company_id = db.Get(0, "company_id");

					if(db.Query("SELECT `id` FROM `company` WHERE `id`=\"" + company_id + "\" AND `admin_userID`=\"" + user.GetID() + "\";"))
					{
						ostResult.str("");
						ostResult << "{" 
								  << "\"result\":\"success\","
								  << "\"candidates\":[" << GetCandidatesListAppliedToVacancyInJSONFormat("SELECT * FROM `company_candidates` WHERE `vacancy_id`=\"" + open_vacancy_id + "\";", &db) << "]"
								  << "}";
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: user[" + user.GetID() + "] not allowed to get candidates list applied to company_vacancy.id[" + open_vacancy_id + "]");
						}

						ostResult.str("");
						ostResult << "{" 
								  << "\"result\":\"error\","
								  << "\"description\":\"Не хватает прав доступа\""
								  << "}";
					}
				}
				else
				{
					{
						CLog	log;

						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: company_vacancy.id[" + open_vacancy_id + "] not found");
					}

					ostResult.str("");
					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"вакансия не найдена\""
							  << "}";
				}

			}


			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			} 


			MESSAGE_DEBUG("", action, "finish");
		}  

		if(action == "AJAX_amIAppliedToVacancy")
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_amIAppliedToVacancy: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_amIAppliedToVacancy: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string			open_vacancy_id			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				ostResult.str("");
				ostResult << "{" 
						  << "\"result\":\"success\","
						  << "\"candidates\":[" << GetCandidatesListAppliedToVacancyInJSONFormat("SELECT * FROM `company_candidates` WHERE `vacancy_id`=\"" + open_vacancy_id + "\" AND `user_id`=\"" + user.GetID() + "\";", &db) << "],"
						  << "\"user\":[" << GetUserListInJSONFormat("SELECT * FROM `users` WHERE `id`=\"" + user.GetID() + "\";", &db, &user) << "]"
						  << "}";

			} // --- if(user.GetLogin() == "Guest")


			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_amIAppliedToVacancy.htmlt"))

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_amIAppliedToVacancy: end");
			}
		}   // if(action == "AJAX_amIAppliedToVacancy")


		if(action == "AJAX_updateOpenVacancy")
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_updateOpenVacancy: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string			company_position_id;
				string			company_id = "";
				string			title 				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
				string			geo_locality		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("geo_locality"));
				string			employmenttype		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("employmenttype"));
				string			question1			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question1"));
				string			question1answers	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question1answers"));
				string			question1answer1	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question1answer1"));
				string			question1answer2	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question1answer2"));
				string			question1answer3	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question1answer3"));
				string			question2			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question2"));
				string			question2answers	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question2answers"));
				string			question2answer1	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question2answer1"));
				string			question2answer2	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question2answer2"));
				string			question2answer3	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question2answer3"));
				string			question3			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question3"));
				string			question3answers	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question3answers"));
				string			question3answer1	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question3answer1"));
				string			question3answer2	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question3answer2"));
				string			question3answer3	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("question3answer3"));
				string			language1			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("language1"));
				string			language2			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("language2"));
				string			language3			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("language3"));
				string			skill1				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("skill1"));
				string			skill2				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("skill2"));
				string			skill3				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("skill3"));
				string			description			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("description"));
				string			publish_period		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("publish"));
				string			openVacancyID		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
				string			minsalary			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("minsalary"));
				string			maxsalary			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("maxsalary"));
				string			month				= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("month"));
				string			geo_locality_id		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("geo_locality_id"));
				string			language1_id = "0";
				string			language2_id = "0";
				string			language3_id = "0";
				string			skill1_id = "0";
				string			skill2_id = "0";
				string			skill3_id = "0";
				string			publish_finish;

				if(db.Query("SELECT `company_id` FROM `company_vacancy` WHERE `id`=\"" + openVacancyID + "\";"))
					company_id = db.Get(0, "company_id");
				else
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy: openVacancyID[" + openVacancyID + "] not found in DB.company_vacancy table");
					}

					ostResult << "{\"result\":\"error\",\"description\":\"ОШИБКА. вакансия не найдена в БД.\"}";
				}

				if(!minsalary.length()) minsalary = "0";
				if(!maxsalary.length()) maxsalary = "0";

				if(publish_period == "week")
				{
					publish_finish = "(NOW() + INTERVAL 7 DAY)";
				}
				else if(publish_period == "month")
				{
					publish_finish = "(NOW() + INTERVAL 31 DAY)";
				}
				else
				{
					publish_finish = "\"2000-01-01\"";
				}

				if((geo_locality_id == "0") || (geo_locality_id.length() == 0))
				{
					string			regionName, cityName;
					smatch			matchResult;
					regex			regexGeoLocality("(.*)[[:s:]]*\\((.*)\\)");

					if(regex_search(geo_locality, matchResult, regexGeoLocality))
					{
			            cityName = matchResult[1];
			            regionName = matchResult[2];
					}
					else
						cityName = geo_locality;

					trim(cityName);
					trim(regionName);

		            if(cityName.length())
						geo_locality_id = GetGeoLocalityIDByCityAndRegion(regionName, cityName, &db);
				}

				company_position_id = GetCompanyPositionIDByTitle(title, &db);
				language1_id = GetLanguageIDByTitle(language1, &db);
				language2_id = GetLanguageIDByTitle(language2, &db);
				language3_id = GetLanguageIDByTitle(language3, &db);
				skill1_id = GetSkillIDByTitle(skill1, &db);
				skill2_id = GetSkillIDByTitle(skill2, &db);
				skill3_id = GetSkillIDByTitle(skill3, &db);

				if(!company_id.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: companyid is empty");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо указать компанию-работодателя\""
							  << "}";
				}
				else if(!company_position_id.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: title is empty");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить должность\""
							  << "}";
				}
				else if(!month.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: month is empty");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать месяц выхода на работу\""
							  << "}";
				}
				else if(!geo_locality_id.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: geo_locality_id is empty");
					}

					ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать город работы выхода на работу\""
							  << "}";
				}
				else if(!employmenttype.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: employmenttype is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить тип трудоустройства\""
							  << "}";
				}
				else if(!question1.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question1 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить квалификационный вопрос 1\""
							  << "}";
				}
				else if(!question1answers.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question1answers is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать правильный ответ на первый вопрос\""
							  << "}";
				}
				else if(!question1answer1.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question1answer1 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на первый вопрос\""
							  << "}";
				}
				else if(!question1answer2.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question1answer2 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на первый вопрос\""
							  << "}";
				}
				else if(!question1answer3.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question1answer3 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на первый вопрос\""
							  << "}";
				}
				else if(!question2.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question2 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить квалификационный вопрос 2\""
							  << "}";
				}
				else if(!question2answers.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question2answers is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать правильный ответ на второй вопрос\""
							  << "}";
				}
				else if(!question2answer1.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question2answer1 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на второй вопрос\""
							  << "}";
				}
				else if(!question2answer2.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question2answer2 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на второй вопрос\""
							  << "}";
				}
				else if(!question2answer3.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question2answer3 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на второй вопрос\""
							  << "}";
				}
				else if(!question3.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question3 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить квалификационный вопрос 3\""
							  << "}";
				}
				else if(!question3answers.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question3answers is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать правильный ответ на третий вопрос\""
							  << "}";
				}
				else if(!question3answer1.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question3answer1 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на третий вопрос\""
							  << "}";
				}
				else if(!question3answer2.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question3answer2 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на третий вопрос\""
							  << "}";
				}
				else if(!question3answer3.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: question3answer3 is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо заполнить все ответы на третий вопрос\""
							  << "}";
				}
				else if(!publish_period.length())
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: publish is empty");
					}
					ostResult << "{"
							  << "\"result\":\"error\","
							  << "\"description\":\"Необходимо выбрать длительность публикации\""
							  << "}";
				}
				else
				{
					if(db.Query("SELECT `id` FROM `company` WHERE `id`=\"" + company_id + "\" AND `admin_userID`=\"" + user.GetID() + "\";"))
					{
						openVacancyID = db.Query("UPDATE `company_vacancy` SET "
														"`company_id`=\"" + company_id + "\", "
														"`company_position_id`=\"" + company_position_id + "\", "
														"`geo_locality_id`=\"" + geo_locality_id + "\", "
														"`salary_min`=\"" + minsalary + "\", "
														"`salary_max`=\"" + maxsalary + "\", "
														"`start_month`=\"" + month + "\", "
														"`work_format`=\"" + employmenttype + "\", "
														"`description`=\"" + description + "\", "
														"`question1`=\"" + question1 + "\", "
														"`answer11`=\"" + question1answer1 + "\", "
														"`answer12`=\"" + question1answer2 + "\", "
														"`answer13`=\"" + question1answer3 + "\", "
														"`correct_answer1`=\"" + question1answers + "\", "
														"`question2`=\"" + question2 + "\", "
														"`answer21`=\"" + question2answer1 + "\", "
														"`answer22`=\"" + question2answer2 + "\", "
														"`answer23`=\"" + question2answer3 + "\", "
														"`correct_answer2`=\"" + question2answers + "\", "
														"`question3`=\"" + question3 + "\", "
														"`answer31`=\"" + question3answer1 + "\", "
														"`answer32`=\"" + question3answer2 + "\", "
														"`answer33`=\"" + question3answer3 + "\", "
														"`correct_answer3`=\"" + question3answers + "\", "
														"`language1_id`=\"" + language1_id + "\", "
														"`language2_id`=\"" + language2_id + "\", "
														"`language3_id`=\"" + language3_id + "\", "
														"`skill1_id`=\"" + skill1_id + "\", "
														"`skill2_id`=\"" + skill2_id + "\", "
														"`skill3_id`=\"" + skill3_id + "\", "
														"`publish_finish`=" + publish_finish + ", "
														"`publish_period`=\"" + publish_period + "\" "
														"WHERE `id`=\"" + openVacancyID + "\";");
						if(db.isError())
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: updating users_notification table");
							}

							ostResult.str("");
							ostResult << "{" 
									  << "\"result\":\"error\","
									  << "\"description\":\"Ошибка БД.\""
									  << "}";
						}
						else
						{
							ostResult.str("");
							ostResult << "{" 
									  << "\"result\":\"success\","
									  << "\"open_vacancies\":[" + GetOpenVacanciesInJSONFormat("SELECT * FROM `company_vacancy` WHERE `company_id`=\"" + company_id + "\";", &db, &user) + "]"
									  << "}";
						}
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy:ERROR: user[" + user.GetID() + "] not allowed create vacancy in companyID[" + company_id + "]");
						}

						ostResult.str("");
						ostResult << "{" 
								  << "\"result\":\"error\","
								  << "\"description\":\"Вы не можете редактировать вакансии.\""
								  << "}";
					}
				}
			} // --- if(user.GetLogin() == "Guest")


			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_updateOpenVacancy.htmlt"))

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_updateOpenVacancy: end");
			}
		}   // if(action == "AJAX_updateOpenVacancy")


		if(action == "AJAX_applyToVacancy")
		{
			ostringstream   ostResult;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_applyToVacancy: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_applyToVacancy: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string			company_position_id;
				string			openVacancyID		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
				string			question1answers	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("question1answers"));
				string			question2answers	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("question2answers"));
				string			question3answers	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("question3answers"));
				string			language1			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("language1"));
				string			language2			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("language2"));
				string			language3			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("language3"));
				string			skill1				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("skill1"));
				string			skill2				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("skill2"));
				string			skill3				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("skill3"));
				string			description			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("description"));

				if(db.Query("SELECT `company_id` FROM `company_vacancy` WHERE `id`=\"" + openVacancyID + "\";"))
				{
					string		company_id = db.Get(0, "company_id");

					if(db.Query("SELECT `id` FROM `company_candidates` WHERE `vacancy_id`=\"" + openVacancyID + "\" AND `user_id`=\"" + user.GetID() + "\" AND `status`=\"rejected\";"))
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_applyToVacancy:ERROR: no allowed to apply to rejected vacancy (company_candidates.id = " + db.Get(0, "id") + ")");
						}
						ostResult << "{"
								  << "\"result\":\"error\","
								  << "\"description\":\"Нельзя регистрироваться на позицию, где Вам отказали\""
								  << "}";
					}
					else
					{

						if(!question1answers.length())
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_applyToVacancy:ERROR: question1answers is empty");
							}
							ostResult << "{"
									  << "\"result\":\"error\","
									  << "\"description\":\"Необходимо выбрать правильный ответ на первый вопрос\""
									  << "}";
						}
						else if(!question2answers.length())
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_applyToVacancy:ERROR: question2answers is empty");
							}
							ostResult << "{"
									  << "\"result\":\"error\","
									  << "\"description\":\"Необходимо выбрать правильный ответ на второй вопрос\""
									  << "}";
						}
						else if(!question3answers.length())
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_applyToVacancy:ERROR: question3answers is empty");
							}
							ostResult << "{"
									  << "\"result\":\"error\","
									  << "\"description\":\"Необходимо выбрать правильный ответ на третий вопрос\""
									  << "}";
						}
						else
						{
							string		sqlQuery = 
													"`vacancy_id`=\"" + openVacancyID + "\", "
													"`user_id`=\"" + user.GetID() + "\", "
													"`answer1`=\"" + question1answers + "\", "
													"`answer2`=\"" + question2answers + "\", "
													"`answer3`=\"" + question3answers + "\", "
													"`language1`=\"" + language1 + "\", "
													"`language2`=\"" + language2 + "\", "
													"`language3`=\"" + language3 + "\", "
													"`skill1`=\"" + skill1 + "\", "
													"`skill2`=\"" + skill2 + "\", "
													"`skill3`=\"" + skill3 + "\", "
													"`description`=\"" + description + "\", "
													"`eventTimestamp`=UNIX_TIMESTAMP() ";

							if(db.Query("SELECT `id` FROM `company_candidates` WHERE `vacancy_id`=\"" + openVacancyID + "\" AND `user_id`=\"" + user.GetID() + "\";"))
								sqlQuery = "UPDATE `company_candidates` SET " + sqlQuery + " WHERE `id`=\"" + db.Get(0, "id") + "\";";
							else
								sqlQuery = "INSERT INTO `company_candidates` SET " + sqlQuery;

							db.Query(sqlQuery);

							if(db.isError())
							{
								{
									CLog	log;
									log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_applyToVacancy:ERROR: updating users_notification table");
								}

								ostResult.str("");
								ostResult << "{" 
										  << "\"result\":\"error\","
										  << "\"description\":\"Ошибка БД.\""
										  << "}";
							}
							else
							{
								ostResult.str("");
								ostResult << "{" 
										  	 "\"result\":\"success\","
									  		 "\"myvacancies\":[" << GetCandidatesListAppliedToVacancyInJSONFormat("SELECT * FROM `company_candidates` WHERE (`vacancy_id` in (SELECT `id` FROM `company_vacancy` WHERE `company_id`=\"" + company_id + "\")) AND `user_id`=\"" + user.GetID() + "\";", &db) << "]"
										  	 "}";
							}
						}

					}

				}
				else
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_applyToVacancy: openVacancyID[" + openVacancyID + "] not found in DB.company_vacancy table");
					}

					ostResult << "{\"result\":\"error\",\"description\":\"ОШИБКА. вакансия не найдена в БД.\"}";
				}

			} // --- if(user.GetLogin() == "Guest")


			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
				throw CException("Template file json_response.htmlt was missing");
			}  // if(!indexPage.SetTemplate("AJAX_applyToVacancy.htmlt"))

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_applyToVacancy: end");
			}
		}   // if(action == "AJAX_applyToVacancy")

		if(action == "AJAX_companyTakeOwnership")
		{
			ostringstream   ostResult;
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_companyTakeOwnership: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_companyTakeOwnership: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		  companyID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				if(companyID.length())
				{

					// --- check if company available
					if(db.Query("SELECT `admin_userID` FROM `company` WHERE `id`=\"" + companyID + "\" AND `isBlocked`=\"N\";"))
					{
						string		admin_userID = db.Get(0, "admin_userID");
						
						if((admin_userID == "0") || (admin_userID == user.GetID()))
						{
							db.Query("UPDATE `company` SET `admin_userID`=\"" + user.GetID() + "\", `ownerChangeCounter`=`ownerChangeCounter`+1,`isConfirmed`=\"N\",`lastActivity`=UNIX_TIMESTAMP() WHERE `id`=\"" + companyID + "\";");

							if(db.isError())
							{
								{
									CLog	log;
									log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_applyToVacancy:ERROR: updating users_notification table");
								}

								ostResult.str("");
								ostResult << "{" 
										  << "\"result\":\"error\","
										  << "\"description\":\"Ошибка БД.\""
										  << "}";
							}
							else
							{
								ostResult.str("");
								ostResult << "{" 
										  	 "\"result\":\"success\""
										  	 "}";

								if(!db.Query("SELECT `id` FROM `users_subscriptions` WHERE `user_id`=\"" + user.GetID() + "\" AND `entity_type`=\"company\" AND `entity_id`=\"" + companyID + "\";"))
									db.Query("INSERT INTO `users_subscriptions` SET `user_id`=\"" + user.GetID() + "\",`entity_type`=\"company\",`entity_id`=\"" + companyID + "\",`eventTimestamp`=UNIX_TIMESTAMP();");
							}

						}
						else
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyTakeOwnership: ERROR: company[" + companyID + "] can't be assigned to user[" + user.GetID() + "] due to current owner exists");
							}

							ostResult << "{\"result\": \"error\", \"description\": \"Компания не может быть переданна\"}";
						}
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyTakeOwnership: ERROR: company[" + companyID + "] doesn't exists");
						}

						ostResult << "{\"result\": \"error\", \"description\": \"Компания не существует\"}";
					}
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_companyTakeOwnership: ERROR: mandatory parameter missed or empty in HTML request [companyID]");
					}

					ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: недостаточно параметров\"}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyTakeOwnership:ERROR: can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_companyTakeOwnership: finish");
			}
		}

		if(action == "AJAX_companyPossessionRequest")
		{
			ostringstream   ostResult;
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		companyID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
				string		description = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("description"));
				string		changeRate = "0";

				if(companyID.length())
				{
					if(db.Query("SELECT `id` FROM `company_possession_request` WHERE "
									"`requested_company_id`=\"" + companyID + "\" AND "
									"`requester_user_id`=\"" + user.GetID() + "\" AND"
									"`eventTimestamp` >= UNIX_TIMESTAMP() - 86400*30;")) // --- ratelimiter once per month
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest: possession request already exists [id = " + db.Get(0, "id") + "]");
						}

						ostResult << "{\"result\": \"error\", \"description\": \"Такой запрос уже существует\"}";
					}
					else
					{
						if(db.Query("SELECT `id` FROM `company_possession_request` WHERE "
									"`requested_company_id`=\"" + companyID + "\" AND "
									"`requester_user_id`=\"" + user.GetID() + "\";"))
						{
							string		requestID = db.Get(0, "id");

							db.Query("DELETE FROM `company_possession_request` WHERE `id`\"" + requestID + "\";");
							db.Query("DELETE FROM `user_notification` WHERE `actionTypeId`=\"60\" AND `id`\"" + requestID + "\";");
						}
						// --- check if company available
						if(db.Query("SELECT `admin_userID` FROM `company` WHERE `id`=\"" + companyID + "\" AND `isBlocked`=\"N\";"))
						{
							string		admin_userID = db.Get(0, "admin_userID");
							
							if((admin_userID != "0") && (admin_userID != user.GetID()))
							{
								long  int	requestID = 0;

								requestID = db.InsertQuery("INSERT INTO `company_possession_request` SET "
																		"`requested_company_id`=\"" + companyID + "\","
																		"`requester_user_id`=\"" + user.GetID() + "\","
																		"`status`=\"requested\","
																		"`description`=\"" + description + "\","
																		"`eventTimestamp`=UNIX_TIMESTAMP();"
																		);

								if(requestID)
								{

									ostResult.str("");
									ostResult << "{" 
											  << "\"result\":\"success\""
											  << "}";
									
									if(db.InsertQuery("INSERT INTO `users_notification` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + admin_userID + "\", \"60\", \"" + to_string(requestID) + "\", UNIX_TIMESTAMP());"))
									{
									}
									else
									{
										{
											CLog	log;
											log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest: ERROR: inserting to `user_notification` table");
										}
									}
								}
								else
								{
									{
										CLog	log;
										log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest: ERROR: inserting possession request to `company_possession_request` table");
									}

									ostResult << "{\"result\": \"error\", \"description\": \"Ошибка БД\"}";
								}

							}
							else
							{
								{
									CLog	log;
									log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest: ERROR: company[" + companyID + "] can't be assigned to user[" + user.GetID() + "] due to current owner exists");
								}

								ostResult << "{\"result\": \"error\", \"description\": \"Компания не может быть переданна\"}";
							}
						}
						else
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest: ERROR: company[" + companyID + "] doesn't exists");
							}

							ostResult << "{\"result\": \"error\", \"description\": \"Компания не существует\"}";
						}
					}
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_companyPossessionRequest: ERROR: mandatory parameter missed or empty in HTML request [companyID]");
					}

					ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: недостаточно параметров\"}";
				}

			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest:ERROR: can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest: finish");
			}
		}

		if(action == "AJAX_grantPossessionRequest")
		{
			ostringstream   ostResult;
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_grantPossessionRequest: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_grantPossessionRequest: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		  notificationID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				if(db.Query("SELECT `actionId` FROM `users_notification` WHERE `id`=\"" + notificationID + "\" AND `userId`=\"" + user.GetID() + "\" AND `actionTypeId`=\"60\";"))
				{
					string		possessionRequestID = db.Get(0, "actionId");

					if(db.Query("SELECT * FROM `company_possession_request` WHERE `id`=\"" + possessionRequestID + "\";"))
					{
						string	requested_company_id = db.Get(0, "requested_company_id");
						string	requester_user_id = db.Get(0, "requester_user_id");
						string	status = db.Get(0, "status");

						if(status == "requested")
						{
							if(requested_company_id.length())
							{

								// --- check if company available
								if(db.Query("SELECT `admin_userID` FROM `company` WHERE `id`=\"" + requested_company_id + "\" AND `isBlocked`=\"N\" AND `admin_userID`=\"" + user.GetID() + "\";"))
								{
									string		admin_userID = db.Get(0, "admin_userID");
									
									if(admin_userID == user.GetID())
									{
										db.Query("UPDATE `company` SET `admin_userID`=\"" + requester_user_id + "\", `ownerChangeCounter`=`ownerChangeCounter`+1, `isConfirmed`=\"N\", `lastActivity`=UNIX_TIMESTAMP() WHERE `id`=\"" + requested_company_id + "\";");

										// --- reassign notifications to new admin "likes / dislikes" and notifications
										db.Query("UPDATE `users_notification` SET `userId`=\"" + requester_user_id + "\" WHERE `actionTypeId`=\"50\" and `actionId` IN (SELECT `actionId` FROM `feed` WHERE `actionTypeId`=\"11\" AND `srcType`=\"company\" AND `userId`=\"" + requested_company_id + "\");");
										db.Query("UPDATE `users_notification` SET `userId`=\"" + requester_user_id + "\" WHERE `actionTypeId`=\"49\" and `actionId` IN (SELECT `id` FROM `feed_message_params` WHERE `messageID` IN (SELECT `actionId` FROM `feed` WHERE `actionTypeId`=\"11\" AND `srcType`=\"company\" AND `userId`=\"" + requested_company_id + "\") and `parameter`=\"like\");");

										// --- reassign notifications to new admin "comments and notifications"
										db.Query("UPDATE `users_notification` SET `userId`=\"" + requester_user_id + "\" WHERE `actionTypeId`=\"19\" and `actionId` IN (SELECT `id` FROM `feed_message_comment` WHERE `messageID` IN (SELECT `actionId` FROM `feed` WHERE `actionTypeId`=\"11\" AND `srcType`=\"company\" AND `userId`=\"" + requested_company_id + "\") and `type`=\"message\");");

										if(db.isError())
										{
											{
												CLog	log;
												log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_applyToVacancy:ERROR: updating users_notification table");
											}

											ostResult.str("");
											ostResult << "{" 
													  << "\"result\":\"error\","
													  << "\"description\":\"Ошибка БД.\""
													  << "}";
										}
										else
										{
											ostResult.str("");
											ostResult << "{" 
													  	 "\"result\":\"success\""
													  	 "}";

											db.Query("UPDATE `company_possession_request` SET `status`=\"approved\", `eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + possessionRequestID + "\";");
											db.Query("DELETE FROM `users_notification` WHERE `id`=\"" + notificationID + "\";");
											if(db.InsertQuery("INSERT INTO `users_notification` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + requester_user_id + "\", \"61\", \"" + requested_company_id + "\", UNIX_TIMESTAMP());"))
											{
											}
											else
											{
												{
													CLog	log;
													log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest: ERROR: inserting to `user_notification` table");
												}
											}

										}

									}
									else
									{
										{
											CLog	log;
											log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_grantPossessionRequest: ERROR: company[" + requested_company_id + "] doesn't belongs to you [userid: " + user.GetID() + "]");
										}

										ostResult << "{\"result\": \"error\", \"description\": \"Компания не может быть передана\"}";
									}
								}
								else
								{
									{
										CLog	log;
										log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_grantPossessionRequest: ERROR: company[" + requested_company_id + "] doesn't exists or not belongs to you [id: " + user.GetID() + "]");
									}

									ostResult << "{\"result\": \"error\", \"description\": \"Компания не существует или не ваша\"}";
								}
							}
							else
							{
								{
									CLog	log;
									log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_grantPossessionRequest: ERROR: mandatory parameter empty [requested_company_id]");
								}

								ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: недостаточно параметров\"}";
							}
						}
						else
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_grantPossessionRequest: ERROR: possession request in wrong state [state: " + status + "], must be in `requested` state");
							}

							ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: невозможно найти запрос на владение\"}";
						}
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_grantPossessionRequest: ERROR: possession request [id: " + possessionRequestID + "] not found");
						}

						ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: невозможно найти запрос на владение\"}";
					}
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_grantPossessionRequest: ERROR: notification [id: " + notificationID + ", actionTypeId: 60] doesn't exists or not yours [userid: " + user.GetID() + "]");
					}

					ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: невозможно найти ваш notification\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_grantPossessionRequest:ERROR: can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_grantPossessionRequest: finish");
			}
		}

		if(action == "AJAX_dropCompanyPossession")
		{
			ostringstream   ostResult;
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_dropCompanyPossession: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_dropCompanyPossession: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		  companyID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				// --- check if company available
				if(db.Query("SELECT `admin_userID` FROM `company` WHERE `id`=\"" + companyID + "\" AND `isBlocked`=\"N\" AND `admin_userID`=\"" + user.GetID() + "\";"))
				{
						db.Query("UPDATE `company` SET `admin_userID`=\"0\", `ownerChangeCounter`=`ownerChangeCounter`+1, `isConfirmed`=\"N\", `lastActivity`=UNIX_TIMESTAMP() WHERE `id`=\"" + companyID + "\";");

						if(db.isError())
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_applyToVacancy:ERROR: updating users_notification table");
							}

							ostResult.str("");
							ostResult << "{" 
									  << "\"result\":\"error\","
									  << "\"description\":\"Ошибка БД.\""
									  << "}";
						}
						else
						{
							ostResult.str("");
							ostResult << "{" 
									  	 "\"result\":\"success\""
									  	 "}";
						}
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_dropCompanyPossession: ERROR: company[" + companyID + "] doesn't exists or not belongs to you [id: " + user.GetID() + "]");
					}

					ostResult << "{\"result\": \"error\", \"description\": \"Компания не существует или не ваша\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_dropCompanyPossession:ERROR: can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_dropCompanyPossession: finish");
			}
		}

		if(action == "AJAX_rejectPossessionRequest")
		{
			ostringstream   ostResult;
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_rejectPossessionRequest: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_rejectPossessionRequest: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		  notificationID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				if(db.Query("SELECT `actionId` FROM `users_notification` WHERE `id`=\"" + notificationID + "\" AND `userId`=\"" + user.GetID() + "\" AND `actionTypeId`=\"60\";"))
				{
					string		possessionRequestID = db.Get(0, "actionId");

					if(db.Query("SELECT * FROM `company_possession_request` WHERE `id`=\"" + possessionRequestID + "\";"))
					{
						string	requested_company_id = db.Get(0, "requested_company_id");
						string	requester_user_id = db.Get(0, "requester_user_id");
						string	status = db.Get(0, "status");

						if(status == "requested")
						{
							if(requested_company_id.length())
							{

								// --- check if company available
								if(db.Query("SELECT `admin_userID` FROM `company` WHERE `id`=\"" + requested_company_id + "\" AND `isBlocked`=\"N\" AND `admin_userID`=\"" + user.GetID() + "\";"))
								{
									string		admin_userID = db.Get(0, "admin_userID");
									
									if(admin_userID == user.GetID())
									{
										db.Query("UPDATE `company_possession_request` SET `status`=\"rejected\", `eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + possessionRequestID + "\";");

										if(db.isError())
										{
											{
												CLog	log;
												log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_applyToVacancy:ERROR: updating users_notification table");
											}

											ostResult.str("");
											ostResult << "{" 
													  << "\"result\":\"error\","
													  << "\"description\":\"Ошибка БД.\""
													  << "}";
										}
										else
										{
											ostResult.str("");
											ostResult << "{" 
													  	 "\"result\":\"success\""
													  	 "}";

											db.Query("DELETE FROM `users_notification` WHERE `id`=\"" + notificationID + "\";");
											if(db.InsertQuery("INSERT INTO `users_notification` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + requester_user_id + "\", \"62\", \"" + requested_company_id + "\", UNIX_TIMESTAMP());"))
											{
											}
											else
											{
												{
													CLog	log;
													log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_companyPossessionRequest: ERROR: inserting to `user_notification` table");
												}
											}

										}

									}
									else
									{
										{
											CLog	log;
											log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_rejectPossessionRequest: ERROR: company[" + requested_company_id + "] doesn't belongs to you [userid: " + user.GetID() + "]");
										}

										ostResult << "{\"result\": \"error\", \"description\": \"Компания не может быть передана\"}";
									}
								}
								else
								{
									{
										CLog	log;
										log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_rejectPossessionRequest: ERROR: company[" + requested_company_id + "] doesn't exists or not belongs to you [id: " + user.GetID() + "]");
									}

									ostResult << "{\"result\": \"error\", \"description\": \"Компания не существует или не ваша\"}";
								}
							}
							else
							{
								{
									CLog	log;
									log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_rejectPossessionRequest: ERROR: mandatory parameter empty [requested_company_id]");
								}

								ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: недостаточно параметров\"}";
							}
						}
						else
						{
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_rejectPossessionRequest: ERROR: possession request in wrong state [state: " + status + "], must be in `requested` state");
							}

							ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: невозможно найти запрос на владение\"}";
						}
					}
					else
					{
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_rejectPossessionRequest: ERROR: possession request [id: " + possessionRequestID + "] not found");
						}

						ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: невозможно найти запрос на владение\"}";
					}
				}
				else
				{
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]::AJAX_rejectPossessionRequest: ERROR: notification [id: " + notificationID + ", actionTypeId: 60] doesn't exists or not yours [userid: " + user.GetID() + "]");
					}

					ostResult << "{\"result\": \"error\", \"description\": \"ОШИБКА: невозможно найти ваш notification\"}";
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_rejectPossessionRequest:ERROR: can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_rejectPossessionRequest: finish");
			}
		}



		// --- AJAX friend list for autocomplete
		if((action == "AJAX_getFindCompaniesListAutocomplete") || (action == "AJAX_getFindCompaniesList") || (action == "AJAX_getFindCompanyByID") || (action == "AJAX_getMyCompaniesList"))
		{
			ostringstream	ost, ostFinal;
			string			sessid, lookForKey, companiesList;
			vector<string>	searchWords;

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + string("]:") + ": action == AJAX_getFindCompaniesListAutocomplete: start");
			}

			// --- Initialization
			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == AJAX_getFindCompaniesListAutocomplete: re-login required");
				}

				ost.str("");
				ost << "/?rand=" << GetRandom(10);
				indexPage.Redirect(ost.str());
			}

			lookForKey = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("lookForKey"));

			if( (lookForKey.length() >= 3) || ((action == "AJAX_getFindCompanyByID") && lookForKey.length())  || (action == "AJAX_getMyCompaniesList")) 
			{
				ostringstream	ost;

				// --- Looking through company name
				ost.str("");
				if(action == "AJAX_getFindCompanyByID")
					ost << "SELECT * FROM `company` WHERE `isBlocked`='N' AND (`id`=\"" << lookForKey << "\");";
				else if (action == "AJAX_getMyCompaniesList")
					ost << "SELECT * FROM `company` WHERE `isBlocked`='N' AND "
														"((`admin_userID`=\"" << user.GetID() << "\") "
														" OR "
														"(`id` IN (SELECT `entity_id` FROM `users_subscriptions` WHERE `entity_type`=\"company\" AND `user_id`=\"" + user.GetID() + "\"))) "
														"ORDER BY `name`;";
				else
					ost << "SELECT * FROM `company` WHERE `isBlocked`='N' AND (`name` like \"%" << lookForKey << "%\");";

				ostFinal << "{\"status\":\"success\",\"companies\":[" << GetCompanyListInJSONFormat(ost.str(), &db, &user) << "]}";

			}
			else
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]::action == AJAX_getFindCompaniesListAutocomplete: searching key is empty");
				}
				ostFinal << "{\"status\":\"error\",\"description\":\"searching key is empty or less then 3\", \"companies\":[]}";
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;

				log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + string("]:ERROR: template file json_response.htmlt was missing"));
				throw CException("Template file was missing");
			}

			{
				CLog	log;

				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + string("]:") + ":action == AJAX_getFindCompaniesListAutocomplete: final response [length = " + to_string(ostFinal.str().length()) + "]");
			}
		}

		if((action == "AJAX_SubscribeOnCompany") || (action == "AJAX_UnsubscribeFromCompany"))
		{
			ostringstream   ostResult;
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_SubscribeOnCompany: start");
			}

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_SubscribeOnCompany: re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
			}
			else
			{
				string		  companyID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));

				ostResult	<< "{" << (action == "AJAX_SubscribeOnCompany" ? SubscribeToCompany(companyID, &user, &db) : UnsubscribeFromCompany(companyID, &user, &db)) << ",";
				ostResult	<< "\"subscriptions\":[" << GetUserSubscriptionsInJSONFormat("SELECT * FROM `users_subscriptions` WHERE `user_id`=\"" + user.GetID() + "\";", &db) << "]";
				ostResult	<< "}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				CLog	log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_SubscribeOnCompany:ERROR: can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			}

			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]: action == AJAX_SubscribeOnCompany: finish");
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

