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

	if((GetMinutesDeclension(0) == "минут") && (GetMinutesDeclension(10) == "минут") && (GetMinutesDeclension(11) == "минут") && (GetMinutesDeclension(19) == "минут") && (GetMinutesDeclension(100) == "минут"))
	{
		if((GetMinutesDeclension(1) == "минуту") && (GetMinutesDeclension(21) == "минуту") && (GetMinutesDeclension(121) == "минуту"))
		{
			if((GetMinutesDeclension(2) == "минуты") && (GetMinutesDeclension(22) == "минуты") && (GetMinutesDeclension(122) == "минуты") && (GetMinutesDeclension(1122) == "минуты"))
			{
				if((GetMinutesDeclension(3) == "минуты") && (GetMinutesDeclension(23) == "минуты") && (GetMinutesDeclension(123) == "минуты") && (GetMinutesDeclension(1123) == "минуты"))
				{
					if((GetMinutesDeclension(-3) == "") && (GetMinutesDeclension(-23) == "") && (GetMinutesDeclension(-123) == "") && (GetMinutesDeclension(-1123) == ""))
					{
						if((GetMinutesDeclension(1.9) == "минуту") && (GetMinutesDeclension(21.5) == "минуту") && (GetMinutesDeclension(122.5) == "минуты") && (GetMinutesDeclension(1123.1) == "минуты"))
						{
						}
						else
						{
							result = false;
							cout << "failed on -22.5" << endl;
						}
					}
					else
					{
						result = false;
						cout << "failed on -3" << endl;
					}
				}
				else
				{
					result = false;
					cout << "failed on 3" << endl;
				}
			}
			else
			{
				result = false;
				cout << "failed on 2" << endl;
			}
		}
		else
		{
			result = false;
			cout << "failed on 1" << endl;
		}
	}
	else
	{
		result = false;
		cout << "failed on 0" << endl;
	}

	return result;
}

bool Test2()
{
	auto result = true;
	auto error_message = ""s;

	if((GetHoursDeclension(0) == "часов") && (GetHoursDeclension(10) == "часов") && (GetHoursDeclension(11) == "часов") && (GetHoursDeclension(19) == "часов") && (GetHoursDeclension(100) == "часов"))
	{
		if((GetHoursDeclension(1) == "час") && (GetHoursDeclension(21) == "час") && (GetHoursDeclension(121) == "час"))
		{
			if((GetHoursDeclension(2) == "часа") && (GetHoursDeclension(22) == "часа") && (GetHoursDeclension(122) == "часа") && (GetHoursDeclension(1122) == "часа"))
			{
				if((GetHoursDeclension(3) == "часа") && (GetHoursDeclension(23) == "часа") && (GetHoursDeclension(123) == "часа") && (GetHoursDeclension(1123) == "часа"))
				{
					if((GetHoursDeclension(-3) == "") && (GetHoursDeclension(-23) == "") && (GetHoursDeclension(-123) == "") && (GetHoursDeclension(-1123) == ""))
					{
						if((GetHoursDeclension(1.9) == "час") && (GetHoursDeclension(21.5) == "час") && (GetHoursDeclension(122.5) == "часа") && (GetHoursDeclension(1123.1) == "часа"))
						{
						}
						else
						{
							result = false;
							cout << "failed on -22.5" << endl;
						}
					}
					else
					{
						result = false;
						cout << "failed on -3" << endl;
					}
				}
				else
				{
					result = false;
					cout << "failed on 3" << endl;
				}
			}
			else
			{
				result = false;
				cout << "failed on 2" << endl;
			}
		}
		else
		{
			result = false;
			cout << "failed on 1" << endl;
		}
	}
	else
	{
		result = false;
		cout << "failed on 0" << endl;
	}

	return result;
}

bool Test3()
{
	auto result = true;
	auto error_message = ""s;

	if((GetDaysDeclension(0) == "дней") && (GetDaysDeclension(10) == "дней") && (GetDaysDeclension(11) == "дней") && (GetDaysDeclension(19) == "дней") && (GetDaysDeclension(100) == "дней"))
	{
		if((GetDaysDeclension(1) == "день") && (GetDaysDeclension(21) == "день") && (GetDaysDeclension(121) == "день"))
		{
			if((GetDaysDeclension(2) == "дня") && (GetDaysDeclension(22) == "дня") && (GetDaysDeclension(122) == "дня") && (GetDaysDeclension(1122) == "дня"))
			{
				if((GetDaysDeclension(3) == "дня") && (GetDaysDeclension(23) == "дня") && (GetDaysDeclension(123) == "дня") && (GetDaysDeclension(1123) == "дня"))
				{
					if((GetDaysDeclension(-3) == "") && (GetDaysDeclension(-23) == "") && (GetDaysDeclension(-123) == "") && (GetDaysDeclension(-1123) == ""))
					{
						if((GetDaysDeclension(1.9) == "день") && (GetDaysDeclension(21.5) == "день") && (GetDaysDeclension(122.5) == "дня") && (GetDaysDeclension(1123.1) == "дня"))
						{
						}
						else
						{
							result = false;
							cout << "failed on -22.5" << endl;
						}
					}
					else
					{
						result = false;
						cout << "failed on -3" << endl;
					}
				}
				else
				{
					result = false;
					cout << "failed on 3" << endl;
				}
			}
			else
			{
				result = false;
				cout << "failed on 2" << endl;
			}
		}
		else
		{
			result = false;
			cout << "failed on 1" << endl;
		}
	}
	else
	{
		result = false;
		cout << "failed on 0" << endl;
	}

	return result;
}

bool Test4()
{
	auto result = true;
	auto error_message = ""s;

	if((GetMonthsDeclension(0) == "месяцев") && (GetMonthsDeclension(10) == "месяцев") && (GetMonthsDeclension(11) == "месяцев") && (GetMonthsDeclension(19) == "месяцев") && (GetMonthsDeclension(100) == "месяцев"))
	{
		if((GetMonthsDeclension(1) == "месяц") && (GetMonthsDeclension(21) == "месяц") && (GetMonthsDeclension(121) == "месяц"))
		{
			if((GetMonthsDeclension(2) == "месяца") && (GetMonthsDeclension(22) == "месяца") && (GetMonthsDeclension(122) == "месяца") && (GetMonthsDeclension(1122) == "месяца"))
			{
				if((GetMonthsDeclension(3) == "месяца") && (GetMonthsDeclension(23) == "месяца") && (GetMonthsDeclension(123) == "месяца") && (GetMonthsDeclension(1123) == "месяца"))
				{
					if((GetMonthsDeclension(-3) == "") && (GetMonthsDeclension(-23) == "") && (GetMonthsDeclension(-123) == "") && (GetMonthsDeclension(-1123) == ""))
					{
						if((GetMonthsDeclension(1.9) == "месяц") && (GetMonthsDeclension(21.5) == "месяц") && (GetMonthsDeclension(122.5) == "месяца") && (GetMonthsDeclension(1123.1) == "месяца"))
						{
						}
						else
						{
							result = false;
							cout << "failed on -22.5" << endl;
						}
					}
					else
					{
						result = false;
						cout << "failed on -3" << endl;
					}
				}
				else
				{
					result = false;
					cout << "failed on 3" << endl;
				}
			}
			else
			{
				result = false;
				cout << "failed on 2" << endl;
			}
		}
		else
		{
			result = false;
			cout << "failed on 1" << endl;
		}
	}
	else
	{
		result = false;
		cout << "failed on 0" << endl;
	}

	return result;
}

bool Test5()
{
	auto result = true;
	auto error_message = ""s;

	if((GetYearsDeclension(0) == "лет") && (GetYearsDeclension(10) == "лет") && (GetYearsDeclension(11) == "лет") && (GetYearsDeclension(19) == "лет") && (GetYearsDeclension(100) == "лет"))
	{
		if((GetYearsDeclension(1) == "год") && (GetYearsDeclension(21) == "год") && (GetYearsDeclension(121) == "год"))
		{
			if((GetYearsDeclension(2) == "года") && (GetYearsDeclension(22) == "года") && (GetYearsDeclension(122) == "года") && (GetYearsDeclension(1122) == "года"))
			{
				if((GetYearsDeclension(3) == "года") && (GetYearsDeclension(23) == "года") && (GetYearsDeclension(123) == "года") && (GetYearsDeclension(1123) == "года"))
				{
					if((GetYearsDeclension(-3) == "") && (GetYearsDeclension(-23) == "") && (GetYearsDeclension(-123) == "") && (GetYearsDeclension(-1123) == ""))
					{
						if((GetYearsDeclension(1.9) == "год") && (GetYearsDeclension(21.5) == "год") && (GetYearsDeclension(122.5) == "года") && (GetYearsDeclension(1123.1) == "года"))
						{
						}
						else
						{
							result = false;
							cout << "failed on -22.5" << endl;
						}
					}
					else
					{
						result = false;
						cout << "failed on -3" << endl;
					}
				}
				else
				{
					result = false;
					cout << "failed on 3" << endl;
				}
			}
			else
			{
				result = false;
				cout << "failed on 2" << endl;
			}
		}
		else
		{
			result = false;
			cout << "failed on 1" << endl;
		}
	}
	else
	{
		result = false;
		cout << "failed on 0" << endl;
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
	if(!Test5())
	{
		testing_success = false;
		cout << "Test5 failed" << endl;
	}

	if(testing_success)
		cout << "Test passed" << endl;

	return(0);
}

