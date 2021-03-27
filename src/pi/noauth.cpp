#include "noauth.h"

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

		if(action == "AJAX_getGeoCountryList")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			success_message = "\"countries\":[" + GetGeoCountryListInJSONFormat("SELECT * FROM `geo_country`;", &db, &user) + "]";

			AJAX_ResponseTemplate(&indexPage, success_message, "");

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
				if(GetValueFromDB("SELECT `id` FROM `users` WHERE `country_code`=" + quoted(country_code) + " AND `phone`=" + quoted(phone_number) + ";", &db).length())
				{
					error_message = SendPhoneConfirmationCode(country_code, phone_number, indexPage.SessID_Get_FromHTTP(), &db, &user);
				}
				else
				{
					error_message = gettext("phone not registered");
					MESSAGE_DEBUG("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("phone number is incorrect");
				MESSAGE_ERROR("", action, "country_code(" + country_code + ") or phone_number(" + phone_number + ") is empty");
			}

			AJAX_ResponseTemplate(&indexPage, "", error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

	/*
		if(action == "AJAX_checkPhoneConfirmationCode")
		{
			MESSAGE_DEBUG("", action, "start");

			auto	confirmation_code = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("confirmation_code"));
			auto	error_message = CheckPhoneConfirmationCode(confirmation_code, indexPage.SessID_Get_FromHTTP(), &db, &user);

			AJAX_ResponseTemplate(&indexPage, "", error_message);

			MESSAGE_DEBUG("", action, "finish");
		}
	*/
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

