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

bool Test1()
{
	auto result = true;
	auto error_message = ""s;
	struct tm	tm1 = GetTMObject("1/1/18");
	mktime(&tm1);
	if(((tm1.tm_year + 1900) == 2018) && (tm1.tm_mon == 0) && (tm1.tm_mday == 1))
	{
		struct tm	tm2 = GetTMObject("32/1/2018");
		mktime(&tm2);
		if(((tm2.tm_year + 1900) == 2018) && (tm2.tm_mon == 1) && (tm2.tm_mday == 1))
		{
			struct tm	tm3 = GetTMObject("15/10/18");
			mktime(&tm3);
			if(((tm3.tm_year + 1900) == 2018) && (tm3.tm_mon == 9) && (tm3.tm_mday == 15))
			{
				struct tm	tm4 = GetTMObject("29/2/20");
				mktime(&tm4);
				if(((tm4.tm_year + 1900) == 2020) && (tm4.tm_mon == 1) && (tm4.tm_mday == 29))
				{
					struct tm	tm5 = GetTMObject("30/2/20");
					mktime(&tm5);
					if(((tm5.tm_year + 1900) == 2020) && (tm5.tm_mon == 2) && (tm5.tm_mday == 1))
					{
					}
					else
					{
						result = false;
						cout << "failed on 30/2/20" << endl;
					}
				}
				else
				{
					result = false;
					cout << "failed on 29/2/20" << endl;
				}
			}
			else
			{
				result = false;
				cout << "failed on 15/10/18" << endl;
			}
		}
		else
		{
			result = false;
			cout << "failed on 32/1/18 (" << tm2.tm_mday << "/" << tm2.tm_mon << "/" << tm2.tm_year << ")" << endl;
		}
	}
	else
	{
		result = false;
		cout << "failed on 31/1/18" << endl;
	}

	return result;
}

bool Test2()
{
	auto result = true;
	auto error_message = ""s;
	struct tm	tm1 = GetTMObject("18-1-1");

	mktime(&tm1);
	if(((tm1.tm_year + 1900) == 2018) && (tm1.tm_mon == 0) && (tm1.tm_mday == 1))
	{
		struct tm	tm2 = GetTMObject("2018-1-32");
		mktime(&tm2);
		if(((tm2.tm_year + 1900) == 2018) && (tm2.tm_mon == 1) && (tm2.tm_mday == 1))
		{
			struct tm	tm3 = GetTMObject("18-10-15");
			mktime(&tm3);
			if(((tm3.tm_year + 1900) == 2018) && (tm3.tm_mon == 9) && (tm3.tm_mday == 15))
			{
				struct tm	tm4 = GetTMObject("20-2-29");
				mktime(&tm4);
				if(((tm4.tm_year + 1900) == 2020) && (tm4.tm_mon == 1) && (tm4.tm_mday == 29))
				{
					struct tm	tm5 = GetTMObject("20-2-30");
					mktime(&tm5);
					if(((tm5.tm_year + 1900) == 2020) && (tm5.tm_mon == 2) && (tm5.tm_mday == 1))
					{
					}
					else
					{
						result = false;
						cout << "failed on 18-1-1" << endl;
					}
				}
				else
				{
					result = false;
					cout << "failed on 2018-1-32" << endl;
				}
			}
			else
			{
				result = false;
				cout << "failed on 18-10-15" << endl;
			}
		}
		else
		{
			result = false;
			cout << "failed on 20-2-29" << endl;
		}
	}
	else
	{
		result = false;
		cout << "failed on 20-2-30" << endl;
	}

	return result;
}

bool Test3()
{
	auto result = true;
	auto error_message = ""s;
	struct tm	tm1 = GetTMObject("1-1-1");

	if((tm1.tm_year == 70))
	{
		struct tm	tm2 = GetTMObject("20184-1-32");
		if((tm2.tm_year == 70))
		{
			struct tm	tm3 = GetTMObject("18-a0-15");
			if((tm3.tm_year == 70))
			{
				struct tm	tm4 = GetTMObject("1/1/19999");
				if((tm4.tm_year == 70))
				{
					struct tm	tm5 = GetTMObject("--");
					if((tm5.tm_year == 70))
					{
					}
					else
					{
						result = false;
						cout << "failed on --" << endl;
					}
				}
				else
				{
					result = false;
					cout << "failed on 1/1/19999" << endl;
				}
			}
			else
			{
				result = false;
				cout << "failed on 18-a-15" << endl;
			}
		}
		else
		{
			result = false;
			cout << "failed on 20184-1-32" << endl;
		}
	}
	else
	{
		result = false;
		cout << "failed on 1-1-1" << endl;
	}

	return result;
}

bool Test4()
{
	auto result = true;
	auto error_message = ""s;

	if((GetTMObject("1/1/18") <= GetTMObject("32/12/18")))
	{
		if(GetTMObject("1/1/18") == GetTMObject("32/12/17"))
		{
			if(GetTMObject("1/1/18") < GetTMObject("02/01/2018"))
			{
				if(GetTMObject("29/2/16") < GetTMObject("01/03/2016"))
				{
					if(GetTMObject("30/2/16") >= GetTMObject("01/03/2016"))
					{
					}
					else
					{
						result = false;
						cout << "failed on GetTMObject(\"30/2/16\") >= GetTMObject(\"01/03/2016\")" << endl;
					}
				}
				else
				{
					result = false;
					cout << "failed on GetTMObject(\"29/2/16\") < GetTMObject(\"01/03/2016\")" << endl;
				}
			}
			else
			{
				result = false;
				cout << "failed on GetTMObject(\"1/1/18\") < GetTMObject(\"02/1/2018\")" << endl;
			}
		}
		else
		{
			result = false;
			cout << "failed on GetTMObject(\"1/1/18\") == GetTMObject(\"32/12/18\")" << endl;
		}
	}
	else
	{
		result = false;
		cout << "failed on GetTMObject(\"1/1/18\").tm_year == GetTMObject(\"32/12/18\").tm_year" << endl;
	}

	MESSAGE_DEBUG("", "", "finish");

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

	if(!Test1())
	{
		testing_success = false;
		cout << "Test1 failed" << endl;
	}
	if(!Test2())
	{
		testing_success = false;
		cout << "Test2 failed" << endl;
	}
	if(!Test3())
	{
		testing_success = false;
		cout << "Test3 failed" << endl;
	}
	if(!Test4())
	{
		testing_success = false;
		cout << "Test4 failed" << endl;
	}

	if(testing_success)
		cout << "Test passed" << endl;

	return(0);
}

