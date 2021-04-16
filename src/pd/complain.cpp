#include "complain.h"

int main()
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	c_config		config(CONFIG_DIR);
	string			action, partnerID;
	CMysql			db;
	struct timeval	tv;

	MESSAGE_DEBUG("", "", __FILE__);

	signal(SIGSEGV, crash_handler); 

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);    /* Flawfinder: ignore */

	try
	{

	indexPage.ParseURL();

	if(!indexPage.SetTemplate("index.htmlt"))
	{
		CLog	log;

		log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: template file was missing");
		throw CException("Template file was missing");
	}

	if(db.Connect(&config) < 0)
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

		action = GenerateSession(action, &config, &indexPage, &db, &user);
	}
	// ------------ end generate common parts

	MESSAGE_DEBUG("", "", "pre-condition if(action == \"" + action + "\")");

	if(action == "AJAX_SubmitImageComplain")
	{
		ostringstream   ostResult;
		string		  id, type;

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: action == AJAX_SubmitImageComplain: start");
		}

		ostResult.str("");
		if(user.GetLogin() == "Guest")
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_SubmitImageComplain: re-login required");
			}

			ostResult << "{\"result\":\"error\",\"description\":\"сессия закончилась, необходимо вновь зайти на сайт\"}";
		}
		else
		{
			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			type = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("type"));

			if(id.length() && type.length())
			{
				unsigned long	newComplainID;

				newComplainID = db.InsertQuery("INSERT INTO `users_complains` SET `userID`=\"" + user.GetID() + "\", `entityID`=\"" + id + "\", `type`=\"" + type + "\", `subtype`=\"image\", `openEventTimestamp`=UNIX_TIMESTAMP();");


				ostResult << "{" 
						  << "\"result\":\"success\","
						  << "\"complains\":[" << GetComplainListInJSONFormat("select * from `users_complains` where `id`=\"" + to_string(newComplainID) + "\";", &db) << "]"
						  << "}";
			}
			else
			{
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:action == AJAX_SubmitImageComplain:ERROR: mandatory parameters missed");
				}

				ostResult << "{" 
							  << "\"result\":\"error\","
							  << "\"description\":\"не указан обязательный параметр\""
							  << "}";
			}

		} // --- if(user.GetLogin() == "Guest")


		indexPage.RegisterVariableForce("result", ostResult.str());

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: template file json_response.htmlt was missing");
			throw CException("Template file json_response.htmlt was missing");
		}  // if(!indexPage.SetTemplate("AJAX_SubmitImageComplain.htmlt"))

		{
			CLog	log;
			log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]:action == AJAX_SubmitImageComplain: end");
		}
	}   // if(action == "AJAX_SubmitImageComplain")







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

