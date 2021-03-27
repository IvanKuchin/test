#include "cron_minute_pi.h"

auto CleanupActivators(CMysql *db)
{
	auto		error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	db->Query("DELETE FROM `activators` WHERE  `date`<=(now() - INTERVAL " + to_string(ACTIVATOR_SESSION_LEN) + " MINUTE);");
	
	MESSAGE_DEBUG("", "", "finish");

	return error_message;
}

auto	CleanUPData(CMysql *db)
{
	auto		error_message = ""s;

	MESSAGE_DEBUG("", "", "start");
	
	db->Query("DELETE FROM `captcha` WHERE `timestamp`<=(NOW()-INTERVAL " + to_string(SESSION_LEN) + " MINUTE)");
	db->Query("DELETE FROM `sessions` WHERE `time`<=(NOW()-INTERVAL `expire` SECOND) AND `expire`>'0'");
	db->Query("DELETE FROM `phone_confirmation` WHERE `eventTimestamp`<(UNIX_TIMESTAMP() - " + to_string(SMSC_EXPIRATION) + ");");

	MESSAGE_DEBUG("", "", "finish (error_message.length = " + to_string(error_message.length()) + ")");

	return error_message;
}

auto CleanupNotActivatedUsers(CMysql *db)
{
	auto		result = ""s;

	MESSAGE_DEBUG("", "", "start");

	if(db->Query("SELECT `id` FROM `users` WHERE `isactivated`='N' AND `activator_sent` <= (now() - INTERVAL " + to_string(ACTIVATOR_SESSION_LEN) + " MINUTE);"))
	{
		db->Query("DELETE FROM `users_passwd` WHERE `userID` IN (SELECT `id` FROM `users` WHERE `isactivated`='N' AND `activator_sent` <= (now() - INTERVAL " + to_string(ACTIVATOR_SESSION_LEN) + " MINUTE));");
		db->Query("DELETE FROM `users` WHERE `isactivated`='N' AND `activator_sent` <= (now() - INTERVAL " + to_string(ACTIVATOR_SESSION_LEN) + " MINUTE);");
	}
	
	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto	CleanUPEmailChange(CMysql *db)
{
	auto		error_message = ""s;

	MESSAGE_DEBUG("", "", "start");
	
	db->Query("DELETE FROM `email_change_tokens` WHERE `eventTimestamp` < DATE_SUB(NOW(), INTERVAL " + to_string(ACTIVATOR_EMAIL_CHANGE) + " MINUTE)");

	MESSAGE_DEBUG("", "", "finish (error_message.length = " + to_string(error_message.length()) + ")");

	return error_message;
}

int main()
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
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
		auto	error_message = ""s;

		if(db.Connect() < 0)
		{
			MESSAGE_ERROR("", "", "can't connect to DB");
			throw CExceptionHTML("MySql connection");
		}

		//--- start of minute cron main functionality

		if((error_message = CleanUPData(&db))							.length()) MESSAGE_ERROR("", "", error_message);
		if((error_message = CleanupNotActivatedUsers(&db))				.length()) MESSAGE_ERROR("", "", error_message);
		if((error_message = CleanUPEmailChange(&db))					.length()) MESSAGE_ERROR("", "", error_message);
		if((error_message = CleanupActivators(&db))						.length()) MESSAGE_ERROR("", "", error_message);


		//--- end of minute cron main functionality
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

