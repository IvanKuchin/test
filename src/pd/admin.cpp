#include "admin.h"

int main()
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	c_config		config(CONFIG_DIR);
	string			act, id, content;
	struct timeval	tv;

	MESSAGE_DEBUG("", "", __FILE__);

	signal(SIGSEGV, crash_handler); 

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);    /* Flawfinder: ignore */
	appStat.SetLogFile(ADMIN_LOG_FILE_NAME);

	try
	{

		indexPage.ParseURL();

		if(!indexPage.SetTemplateFile("templates/admin_index.htmlt"))
		{
			CLog	log(ADMIN_LOG_FILE_NAME);

			log.Write(ERROR, string(__func__) + ": ERROR: template file was missing");
			throw CException("Template file was missing");
		}

 
		CMysql	db;
		if(db.Connect(&config) < 0)
		{
			CLog	log(ADMIN_LOG_FILE_NAME);

			log.Write(ERROR, string(__func__) + ": ERROR: Can not connect to mysql database");
			return(1);
		}

		act = indexPage.GetVarsHandler()->Get("act");

		indexPage.RegisterVariable("rand", GetRandom(10).c_str());

		{
			CLog	log(ADMIN_LOG_FILE_NAME);
			log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act ==", act);
		}

		if(act == "admin_chat_get_cnx_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_chat_get_cnx_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_chat_get_cnx_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_chat_get_cnx_list: end");
			}
		}
		if(act == "admin_chat_get_presence_cache")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_chat_get_presence_cache: begin");
			}

			indexPage.SetTemplateFile("templates/admin_chat_get_presence_cache.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_chat_get_presence_cache: end");
			}
		}
		if(act == "admin_get_user_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_user_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_user_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_user_list: end");
			}
		}
		if(act == "admin_get_certificates_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_certificates_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_certificates_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_certificates_list: end");
			}
		}
		if(act == "admin_get_courses_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_courses_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_courses_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_courses_list: end");
			}
		}
		if(act == "admin_get_languages_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_languages_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_languages_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_languages_list: end");
			}
		}
		if(act == "admin_get_universities_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_universities_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_universities_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_universities_list: end");
			}
		}
		if(act == "admin_get_schools_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_schools_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_schools_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_schools_list: end");
			}
		}
		if(act == "admin_get_book_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_book_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_book_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_book_list: end");
			}
		}
		if(act == "admin_get_book_complains_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_book_complains_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_book_complains_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_book_complains_list: end");
			}
		}
		if(act == "admin_get_complains_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_complains_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_complains_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_complains_list: end");
			}
		}
		if(act == "admin_get_companies_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_companies_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_companies_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_companies_list: end");
			}
		}
		if(act == "admin_get_occupations_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_occupations_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_occupations_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_occupations_list: end");
			}
		}
		if(act == "admin_get_regions_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_regions_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_regions_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_regions_list: end");
			}
		}
		if(act == "admin_get_localities_list")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_localities_list: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_localities_list.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_localities_list: end");
			}
		}
		if(act == "admin_get_companies_duplicates")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_companies_duplicates: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_companies_duplicates.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_companies_duplicates: end");
			}
		}
		if(act == "admin_get_pictures_lostindb")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_pictures_lostindb: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_pictures_lostindb.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_pictures_lostindb: end");
			}
		}

		if(act == "admin_get_recommendation_adverse")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_recommendation_adverse: begin");
			}

			indexPage.SetTemplateFile("templates/admin_get_recommendation_adverse.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == admin_get_recommendation_adverse: end");
			}
		}

		if(act == "AJAX_getUserList")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getUserList: begin");
			}

			indexPage.RegisterVariableForce("result", "[" + GetUserListInJSONFormat("select * from `users` ORDER BY `users`.`id` ASC;", &db, &user) + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getUserList: end");
			}

		}

		if(act == "AJAX_getUserByID")
		{
			string			userID;
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getUserByID: begin");
			}

			ostResult.str("");
			userID = indexPage.GetVarsHandler()->Get("userID");
			if(userID.length())
			{
				ostResult << "{\"result\":\"success\", \"users\": [" + GetUserListInJSONFormat("select * from `users` WHERE `id`=\"" + userID + "\";", &db, NULL) + "]}";
			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getUserByID:ERROR: userID empty");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"userID is empty\"}";
			}


			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getUserByID: end");
			}
		}

		if(act == "AJAX_getBookByField")
		{
			string			name, value;
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getBookByTitle: begin");
			}

			ostResult.str("");
			name = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("name"));
			value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

			if(name.length() && value.length())
			{
				ostResult << "{\"result\":\"success\", \"books\": [" + GetBookListInJSONFormat("select * from `book` WHERE `" + name + "`=\"" + value + "\";", &db) + "]}";
			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getBookByTitle:ERROR: name or value is empty");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"name or value is empty\"}";
			}


			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getBookByTitle: end");
			}
		}

		if(act == "AJAX_setBookComplainResolveComment")
		{
			string			complainID, resolveComment;
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setBookComplainResolveComment: begin");
			}

			complainID = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("complainID"));
			resolveComment = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("resolveComment"));

			if(complainID.length() && resolveComment.length())
			{
				db.Query("UPDATE `book_complains` SET `resolveComment`=\"" + resolveComment + "\" WHERE `id`=\"" + complainID + "\";");

				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setBookComplainResolveComment:ERROR: complainID empty");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"complainID is empty\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setBookComplainResolveComment: end");
			}

		}

		if(act == "AJAX_setComplainResolveComment")
		{
			string			complainID, resolveComment;
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setComplainResolveComment: begin");
			}

			complainID = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("complainID"));
			resolveComment = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("resolveComment"));

			if(complainID.length() && resolveComment.length())
			{
				db.Query("UPDATE `users_complains` SET `resolveComment`=\"" + resolveComment + "\" WHERE `id`=\"" + complainID + "\";");

				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setComplainResolveComment:ERROR: complainID empty");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"complainID is empty\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setComplainResolveComment: end");
			}

		}
		
		
		if(act == "AJAX_setComplainState")
		{
			string			complainID, state;
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setComplainState: begin");
			}

			complainID = indexPage.GetVarsHandler()->Get("complainID");
			state = indexPage.GetVarsHandler()->Get("state");
			if(complainID.length() && state.length())
			{
				db.Query("UPDATE `users_complains` SET `state`=\"" + state + "\", `closeEventTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + complainID + "\";");

				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setComplainState:ERROR: complainID empty");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"complainID is empty\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setComplainState: end");
			}

		}
		
		if(act == "AJAX_setBookComplainState")
		{
			string			complainID, state;
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setBookComplainState: begin");
			}

			complainID = indexPage.GetVarsHandler()->Get("complainID");
			state = indexPage.GetVarsHandler()->Get("state");
			if(complainID.length() && state.length())
			{
				db.Query("UPDATE `book_complains` SET `state`=\"" + state + "\" WHERE `id`=\"" + complainID + "\";");

				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setBookComplainState:ERROR: complainID empty");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"complainID is empty\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setBookComplainState: end");
			}

		}
		
		if(act == "AJAX_getBookComplainsList")
		{
			CUser			user;
			ostringstream	ostResult;
			int				numberComplains;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getBookComplainsList: begin");
			}

			numberComplains = db.Query("SELECT * FROM `book_complains`;");
			for(int i = 0; i < numberComplains; ++i)
			{
				if(ostResult.str().length()) ostResult << ",";

				ostResult << "{"
						  << "\"id\": \""  					  	<< db.Get(i, "id") << "\", "
						  << "\"authorName\": \""		 		<< db.Get(i, "authorName") << "\","
						  << "\"title\": \"" 				   	<< db.Get(i, "title") << "\", "
						  << "\"isbn10\": \"" 					<< db.Get(i, "isbn10") << "\","
						  << "\"isbn13\": \"" 					<< db.Get(i, "isbn13") << "\","
						  << "\"cover\": \""		   			<< db.Get(i, "cover") << "\","
						  << "\"complainedUserID\": \"" 	  	<< db.Get(i, "userID") << "\","
						  << "\"complain\": \"" 				<< db.Get(i, "complain") << "\","
						  << "\"resolveComment\": \"" 			<< db.Get(i, "resolveComment") << "\","
						  << "\"state\": \"" 					<< db.Get(i, "state") << "\","
						  << "\"eventTimestamp\": \"" 			<< db.Get(i, "eventTimestamp") << "\""
						  << "}";
			}


			indexPage.RegisterVariableForce("result", "[" + ostResult.str() + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getBookComplainsList: end");
			}

		}

		if(act == "AJAX_getComplainsList")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getComplainList: begin");
			}

			indexPage.RegisterVariableForce("result", "{\"complains\":[" + GetComplainListInJSONFormat("select * from `users_complains` ORDER BY `id` ASC;", &db, true) + "]}");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getComplainList: end");
			}

		}

		if(act == "AJAX_getBookList")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getBookList: begin");
			}

			indexPage.RegisterVariableForce("result", "[" + GetBookListInJSONFormat("select * from `book` ORDER BY `id` ASC;", &db, true) + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getBookList: end");
			}

		}
		if(act == "AJAX_getCertificateList")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getCertificateList: begin");
			}

			indexPage.RegisterVariableForce("result", "[" + GetCertificationListInJSONFormat("select * from `certification_tracks` ORDER BY `vendor_id` ASC;", &db, true) + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getCertificateList: end");
			}

		}
		if(act == "AJAX_getCourseList")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getCourseList: begin");
			}

			indexPage.RegisterVariableForce("result", "[" + GetCourseListInJSONFormat("select * from `certification_tracks` ORDER BY `vendor_id` ASC;", &db, true) + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getCourseList: end");
			}

		}
		if(act == "AJAX_getLanguageList")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getLanguageList: begin");
			}

			indexPage.RegisterVariableForce("result", "[" + GetLanguageListInJSONFormat("select * from `language` ORDER BY `language`.`title` ASC;", &db, true) + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getLanguageList: end");
			}

		}
		if(act == "AJAX_getUniversityList")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getUniversityList: begin");
			}

			indexPage.RegisterVariableForce("result", "[" + GetUniversityListInJSONFormat("select * from `university` ORDER BY `id` ASC;", &db, true) + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getUniversityList: end");
			}

		}
		if(act == "AJAX_getSchoolList")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getSchoolList: begin");
			}

			indexPage.RegisterVariableForce("result", "[" + GetSchoolListInJSONFormat("select * from `school` ORDER BY `geo_locality_id` ASC;", &db, true) + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getSchoolList: end");
			}

		}

		if(act == "AJAX_getOccupationsList")
		{
			string		result = "";			

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getOccupationsList: begin");
			}

			{
				struct OccupationClass {
					string	id, title;
					string	employedUsersList;
					string	openVacanciesList;
				};
				vector<OccupationClass>		occupationList;
				int							affected;

				affected = db.Query("select * from `company_position` ORDER BY `company_position`.`id` DESC");
				for(int i = 0; i < affected; ++i)
				{
					OccupationClass		item;

					item.id = db.Get(i, "id");
					item.title = db.Get(i, "title");

					occupationList.push_back(item);
				}

				for(auto &item : occupationList)
				{
					string		temp = "";
					long int	affected = db.Query("select `user_id` from `users_company` where `position_title_id`=\"" + item.id + "\";");

					for(int i = 0; i < affected; ++i)
					{
						if(temp.length()) temp += ",";
						temp += db.Get(i, "user_id");						
					}

					item.employedUsersList = temp;
				}

				for(auto &item : occupationList)
				{
					string		temp = "";
					long int	affected = db.Query("select `company_id` from `company_vacancy` where `company_position_id`=\"" + item.id + "\";");

					for(int i = 0; i < affected; ++i)
					{
						if(temp.length()) temp += ",";
						temp += db.Get(i, "company_id");						
					}

					item.openVacanciesList = temp;
				}


				for(auto &item : occupationList)
				{
					if(result.length()) result += ",";
					result += "{";
					result += "\"occupationID\": \"" + item.id + "\",";
					result += "\"occupationTitle\": \"" + item.title + "\",";
					result += "\"occupationEmployedUsersList\": [" + item.employedUsersList + "],";
					result += "\"occupationOpenVacanciesList\": [" + item.openVacanciesList + "]";
					result += "}";
				}
			}

			indexPage.RegisterVariableForce("result", "[" + result + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getOccupationsList: end");
			}

		}

		if(act == "AJAX_getLocalitiesList")
		{
			string		result = "";			

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getLocalitiesList: begin");
			}

			{
				struct LocalityClass {
					string	id, title;
					string	schoolList;
					string	openVacanciesList;
				};
				vector<LocalityClass>		localityList;
				int							affected;

				affected = db.Query("SELECT `geo_locality`.`id` as geo_locality_id, `geo_locality`.`title` as geo_locality_title, `geo_region`.`title` as geo_region_title FROM `geo_locality` inner JOIN `geo_region` on `geo_locality`.`geo_region_id`=`geo_region`.`id` ORDER BY `geo_locality`.`id` DESC");
				for(int i = 0; i < affected; ++i)
				{
					LocalityClass		item;

					item.id = db.Get(i, "geo_locality_id");
					item.title = string(db.Get(i, "geo_locality_title")) + " (" + string(db.Get(i, "geo_region_title")) + ")";

					localityList.push_back(item);
				}

				for(auto &item : localityList)
				{
					string		temp = "";
					long int	affected = db.Query("select `id` from `school` where `geo_locality_id`=\"" + item.id + "\";");

					for(int i = 0; i < affected; ++i)
					{
						if(temp.length()) temp += ",";
						temp += db.Get(i, "id");						
					}

					item.schoolList = temp;
				}

				for(auto &item : localityList)
				{
					string		temp = "";
					long int	affected = db.Query("select `company_id` from `company_vacancy` where `geo_locality_id`=\"" + item.id + "\";");

					for(int i = 0; i < affected; ++i)
					{
						if(temp.length()) temp += ",";
						temp += db.Get(i, "company_id");						
					}

					item.openVacanciesList = temp;
				}


				for(auto &item : localityList)
				{
					if(result.length()) result += ",";
					result += "{";
					result += "\"id\": \"" + item.id + "\",";
					result += "\"title\": \"" + item.title + "\",";
					result += "\"schoolsList\": [" + item.schoolList + "],";
					result += "\"openVacanciesList\": [" + item.openVacanciesList + "]";
					result += "}";
				}
			}

			indexPage.RegisterVariableForce("result", "[" + result + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getLocalitiesList: end");
			}

		}

		if(act == "AJAX_getRegionsList")
		{
			string		result = "";			

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getRegionsList: begin");
			}

			{
				struct LocalityClass {
					string	id, title;
					string	universityList;
					string	localityList;
				};
				vector<LocalityClass>		regionList;
				int							affected;

				affected = db.Query("SELECT `geo_region`.`id` as geo_region_id, `geo_region`.`title` as geo_region_title FROM `geo_region`  ORDER BY `geo_region`.`id` DESC");
				for(int i = 0; i < affected; ++i)
				{
					LocalityClass		item;

					item.id = db.Get(i, "geo_region_id");
					item.title = db.Get(i, "geo_region_title");

					regionList.push_back(item);
				}

				for(auto &item : regionList)
				{
					string		temp = "";
					long int	affected = db.Query("select `id` from `university` where `geo_region_id`=\"" + item.id + "\";");

					for(int i = 0; i < affected; ++i)
					{
						if(temp.length()) temp += ",";
						temp += db.Get(i, "id");						
					}

					item.universityList = temp;
				}

				for(auto &item : regionList)
				{
					string		temp = "";
					long int	affected = db.Query("select `id` from `geo_locality` where `geo_region_id`=\"" + item.id + "\";");

					for(int i = 0; i < affected; ++i)
					{
						if(temp.length()) temp += ",";
						temp += db.Get(i, "id");						
					}

					item.localityList = temp;
				}


				for(auto &item : regionList)
				{
					if(result.length()) result += ",";
					result += "{";
					result += "\"id\": \"" + item.id + "\",";
					result += "\"title\": \"" + item.title + "\",";
					result += "\"universityList\": [" + item.universityList + "],";
					result += "\"localityList\": [" + item.localityList + "]";
					result += "}";
				}
			}

			indexPage.RegisterVariableForce("result", "[" + result + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getRegionsList: end");
			}

		}



		if(act == "AJAX_getCompaniesList")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getCompaniesList: begin");
			}

			indexPage.RegisterVariableForce("result", "[" + GetCompanyListInJSONFormat("select * from `company`", &db, NULL, true, true) + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getCompaniesList: end");
			}

		}

		if(act == "AJAX_getCompaniesDuplicates")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getCompaniesDuplicates: begin");
			}

			indexPage.RegisterVariableForce("result", "[" + GetCompanyDuplicates(&db) + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getCompaniesDuplicates: end");
			}

		}

		if(act == "AJAX_getPicturesLostindb")
		{
			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getPicturesLostindb: begin");
			}

			indexPage.RegisterVariableForce("lostWithZeroSetID", GetPicturesWithEmptySet(&db));
			indexPage.RegisterVariableForce("lostWithUnknownMessage", GetPicturesWithUnknownMessage(&db));
			indexPage.RegisterVariableForce("lostWithUnknownUser", GetPicturesWithUnknownUser(&db));
			indexPage.SetTemplateFile("templates/json_lostpictures.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getPicturesLostindb: end");
			}

		}

		if(act == "AJAX_removeMessagePicture")
		{
			string			imageID = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_removeMessagePicture: begin");
			}

			imageID = indexPage.GetVarsHandler()->Get("imageid");
			if(imageID.length())
			{
				string 	whereStatement = "`id`=\"" + imageID + "\"";
				RemoveMessageImages(whereStatement, &db);
				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_removeMessagePicture:ERROR: imageid empty");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"imageid is empty\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_removeMessagePicture: end");
			}

		}

		if(act == "AJAX_removeSpecifiedCover")
		{
			string			itemID = "";
			string			itemType = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_removeSpecifiedCover: begin");
			}

			itemID = indexPage.GetVarsHandler()->Get("id");
			itemType = indexPage.GetVarsHandler()->Get("type");

			ostResult.str("");
			if(itemID.length() && itemType.length())
			{
				if(RemoveSpecifiedCover(itemID, itemType, &config, &db)) ostResult << "{\"result\":\"success\"}";
				else ostResult << "{\"result\":\"error\",\"description\":\"ERROR removing specified cover\"}";
			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_removeSpecifiedCover:ERROR: imageid empty");
				}
				ostResult << "{\"result\":\"error\", \"description\":\"imageid is empty\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_removeSpecifiedCover: end");
			}

		}

		if(act == "AJAX_removeCertificate")
		{
			string			itemID = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeCertificate: begin");
			}

			itemID = indexPage.GetVarsHandler()->Get("id");
			if(itemID.length())
			{
				if(db.Query("select * from `certification_tracks` where `id`=\"" + itemID + "\";"))
				{

					if(db.Query("select * from `users_certifications` where `track_id`=\"" + itemID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"user assigned to this certificate\"}";
					}
					else
					{
						RemoveSpecifiedCover(itemID, "certification", &config, &db);
						db.Query("delete from `certification_tracks` where `id`=\"" + itemID + "\";");
						ostResult << "{\"result\":\"success\"}";
					}
				}
				else
				{
					ostResult << "{\"result\":\"error\", \"description\":\"certificate not found\"}";
				}

			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeCertificate:ERROR: itemID not found");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"itemID not found\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeCertificate: end");
			}

		}

		if(act == "AJAX_removeCourse")
		{
			string			itemID = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeCourse: begin");
			}

			itemID = indexPage.GetVarsHandler()->Get("id");
			if(itemID.length())
			{
				if(db.Query("select * from `certification_tracks` where `id`=\"" + itemID + "\";"))
				{

					if(db.Query("select * from `users_courses` where `track_id`=\"" + itemID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"user assigned to this course\"}";
					}
					else
					{
						RemoveSpecifiedCover(itemID, "course", &config, &db);
						db.Query("delete from `certification_tracks` where `id`=\"" + itemID + "\";");
						ostResult << "{\"result\":\"success\"}";
					}
				}
				else
				{
					ostResult << "{\"result\":\"error\", \"description\":\"course not found\"}";
				}

			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeCourse:ERROR: itemID not found");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"itemID not found\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeCourse: end");
			}

		}

		if(act == "AJAX_removeUniversity")
		{
			string			itemID = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeUniversity: begin");
			}

			itemID = indexPage.GetVarsHandler()->Get("id");
			if(itemID.length())
			{
				if(db.Query("select * from `university` where `id`=\"" + itemID + "\";"))
				{

					if(db.Query("select * from `users_university` where `university_id`=\"" + itemID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"user assigned to this university\"}";
					}
					else
					{
						RemoveSpecifiedCover(itemID, "university", &config, &db);
						db.Query("delete from `university` where `id`=\"" + itemID + "\";");
						ostResult << "{\"result\":\"success\"}";
					}
				}
				else
				{
					ostResult << "{\"result\":\"error\", \"description\":\"university not found\"}";
				}

			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeUniversity:ERROR: itemID is empty");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"itemID is empty\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeUniversity: end");
			}
		}

		if(act == "AJAX_removeSchool")
		{
			string			itemID = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeSchool: begin");
			}

			itemID = indexPage.GetVarsHandler()->Get("id");
			if(itemID.length())
			{
				if(db.Query("select * from `school` where `id`=\"" + itemID + "\";"))
				{

					if(db.Query("select * from `users_school` where `school_id`=\"" + itemID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"user assigned to this school\"}";
					}
					else
					{
						RemoveSpecifiedCover(itemID, "school", &config, &db);
						db.Query("delete from `school` where `id`=\"" + itemID + "\";");
						ostResult << "{\"result\":\"success\"}";
					}
				}
				else
				{
					ostResult << "{\"result\":\"error\", \"description\":\"school not found\"}";
				}

			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeSchool:ERROR: itemID is empty");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"itemID is empty\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeSchool: end");
			}

		}

		if(act == "AJAX_removeLanguage")
		{
			string			itemID = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeLanguage: begin");
			}

			itemID = indexPage.GetVarsHandler()->Get("id");
			if(itemID.length())
			{
				if(db.Query("select * from `language` where `id`=\"" + itemID + "\";"))
				{

					if(db.Query("select * from `users_language` where `language_id`=\"" + itemID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"user assigned to this language\"}";
					}
					else
					{
						RemoveSpecifiedCover(itemID, "language", &config, &db);
						db.Query("delete from `language` where `id`=\"" + itemID + "\";");
						ostResult << "{\"result\":\"success\"}";
					}
				}
				else
				{
					ostResult << "{\"result\":\"error\", \"description\":\"language not found\"}";
				}

			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeLanguage:ERROR: itemID is empty");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"itemID is empty\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeLanguage: end");
			}

		}


		if(act == "AJAX_removeBook")
		{
			string			bookID = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeBook: begin");
			}

			bookID = indexPage.GetVarsHandler()->Get("bookID");
			if(bookID.length())
			{
				string 	whereStatement = "`id`=\"" + bookID + "\"";

				if(db.Query("select * from `book` where `id`=\"" + bookID + "\";"))
				{

					if(db.Query("select * from `users_books` where `bookID`=\"" + bookID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"reader assigned to this book\"}";
					}
					else
					{
						string 	whereStatement = "`id`=\"" + bookID + "\"";
						RemoveBookCover(whereStatement, &config, &db);
						db.Query("delete from `book` where `id`=\"" + bookID + "\";");
						ostResult << "{\"result\":\"success\"}";
					}
				}
				else
				{
					ostResult << "{\"result\":\"error\", \"description\":\"book not found\"}";
				}

			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeBook:ERROR: bookID not found");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"bookID not found\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeBook: end");
			}

		}

		if(act == "AJAX_removeOccupation")
		{
			string			occupationID = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeOccupation: begin");
			}

			occupationID = indexPage.GetVarsHandler()->Get("occupationID");
			if(occupationID.length())
			{
				string 	whereStatement = "`id`=\"" + occupationID + "\"";

				if(db.Query("select * from `company_position` where `id`=\"" + occupationID + "\";"))
				{

					if(db.Query("select * from `users_company` where `position_title_id`=\"" + occupationID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"user assigned to this occupation\"}";
					}
					else if(db.Query("select * from `company_vacancy` where `company_position_id`=\"" + occupationID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"open vacancy assigned to this occupation\"}";
					}
					else
					{
						db.Query("delete from `company_position` where `id`=\"" + occupationID + "\";");
						ostResult << "{\"result\":\"success\"}";
					}
				}
				else
				{
					ostResult << "{\"result\":\"error\", \"description\":\"occupation not found\"}";
				}

			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeOccupation:ERROR: occupationID not found");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"occupationID not found\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeOccupation: end");
			}
		}

		if(act == "AJAX_removeLocality")
		{
			string			localityID = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeLocality: begin");
			}

			localityID = indexPage.GetVarsHandler()->Get("id");
			if(localityID.length())
			{
				string 	whereStatement = "`id`=\"" + localityID + "\"";

				if(db.Query("select * from `geo_locality` where `id`=\"" + localityID + "\";"))
				{

					if(db.Query("select * from `school` where `geo_locality_id`=\"" + localityID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"school assigned to this locality\"}";
					}
					else if(db.Query("select * from `company_vacancy` where `geo_locality_id`=\"" + localityID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"open vacancy assigned to this locality\"}";
					}
					else
					{
						db.Query("delete from `geo_locality` where `id`=\"" + localityID + "\";");
						ostResult << "{\"result\":\"success\"}";
					}
				}
				else
				{
					ostResult << "{\"result\":\"error\", \"description\":\"locality not found\"}";
				}

			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeLocality:ERROR: localityID not found");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"localityID not found\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeLocality: end");
			}
		}

		if(act == "AJAX_removeRegion")
		{
			string			regionID = "";
			ostringstream	ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeRegion: begin");
			}

			regionID = indexPage.GetVarsHandler()->Get("id");
			if(regionID.length())
			{
				string 	whereStatement = "`id`=\"" + regionID + "\"";

				if(db.Query("select * from `geo_region` where `id`=\"" + regionID + "\";"))
				{

					if(db.Query("select * from `university` where `geo_region_id`=\"" + regionID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"university assigned to this locality\"}";
					}
					else if(db.Query("select * from `geo_locality` where `geo_region_id`=\"" + regionID + "\";"))
					{
						ostResult << "{\"result\":\"error\", \"description\":\"locality assigned to this locality\"}";
					}
					else
					{
						db.Query("delete from `geo_region` where `id`=\"" + regionID + "\";");
						ostResult << "{\"result\":\"success\"}";
					}
				}
				else
				{
					ostResult << "{\"result\":\"error\", \"description\":\"region not found\"}";
				}

			}
			else
			{
				{
					CLog	log(ADMIN_LOG_FILE_NAME);
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeRegion:ERROR: regionID not found");
				}
				ostResult.str("");
				ostResult << "{\"result\":\"error\", \"description\":\"regionID not found\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: act == AJAX_removeRegion: end");
			}
		}



		if(act == "AJAX_getRecommendationAdverse")
		{
			CUser	user;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getRecommendationAdverse: begin");
			}

			indexPage.RegisterVariableForce("result", "[" + GetRecommendationAdverse(&db) + "]");
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_getRecommendationAdverse: end");
			}

		}

		if(act == "AJAX_setRecommendationPotentialAdverse")
		{
			string			recommendationID = indexPage.GetVarsHandler()->Get("id");
			ostringstream	ost, ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setRecommendationPotentialAdverse: begin");
			}

			ostResult.str("");
			ost.str("");
			ost << "update `users_recommendation` set `state`=\"potentially adverse\" where `id`=\"" << recommendationID << "\";";
			db.Query(ost.str());
			if(!db.isError())
			{
				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				CLog	log;
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setRecommendationPotentialAdverse: ERROR:", db.GetErrorMessage());

				ostResult << "{\"result\":\"success\", \"description\":\"" << db.GetErrorMessage() << "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setRecommendationPotentialAdverse: end");
			}

		}

		if(act == "AJAX_setRecommendationClean")
		{
			string			recommendationID = indexPage.GetVarsHandler()->Get("id");
			ostringstream	ost, ostResult;

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setRecommendationClean: begin");
			}

			ostResult.str("");
			ost.str("");
			ost << "update `users_recommendation` set `state`=\"clean\" where `id`=\"" << recommendationID << "\";";
			db.Query(ost.str());
			if(!db.isError())
			{
				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				CLog	log;
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setRecommendationClean: ERROR:", db.GetErrorMessage());

				ostResult << "{\"result\":\"success\", \"description\":\"" << db.GetErrorMessage() << "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());
			indexPage.SetTemplateFile("templates/json_response.htmlt");

			{
				CLog	log(ADMIN_LOG_FILE_NAME);
				log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: act == AJAX_setRecommendationClean: end");
			}

		}

			
		indexPage.RegisterVariableForce("rand", GetRandom(10).c_str());
		indexPage.OutTemplate();


	}
	catch(CException c)
	{
		indexPage.SetTemplateFile("templates/error.htmlt");
		indexPage.RegisterVariable("content", c.GetReason().c_str());
		indexPage.OutTemplate();
	return(-1);
	}

	return(0);
}

