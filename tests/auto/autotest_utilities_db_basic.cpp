#include <string>
#include <iostream>
#include "ccgi.h"
#include "cmysql.h"
#include "cuser.h"
#include "cstatistics.h"
#include "utilities.h"
#include "utilities_common.h"
#include "localy.h"

using namespace std;

bool Test1(CMysql *db)
{
	auto			result = true;

	setlocale(LC_ALL, "ru_RU.utf8");

	if(db->Query("SELECT NOW();")) {}
	else
	{
		result = false;
		cout << "failed on select from db" << endl;
	}

	return result;
}

int main(void)
{
	CStatistics		appStat;  // --- CStatistics must be firts statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	CMysql			db;
	auto			testing_success = true;

	{ MESSAGE_DEBUG("", "", __FILE__); }
	signal(SIGSEGV, crash_handler);

	if(db.Connect() < 0)
	{
		MESSAGE_ERROR("", "", "Can not connect to mysql database");
		throw CExceptionHTML("MySql connection");
	}

	if(!Test1(&db))
	{
		testing_success = false;
		cout << "Test1 failed" << endl;
	}

	if(testing_success)
		cout << "Test passed" << endl;

	return(0);
}

