#include <string>
#include <iostream>
#include "ccgi.h"
#include "cmysql.h"
#include "cuser.h"
#include "cstatistics.h"
#include "c_float.h"
#include "utilities.h"
#include "utilities_common.h"
#include "localy.h"

using namespace std;

bool RunTest1()
{
	auto							result = true;
	vector<pair<string, double>>	arr = {
		make_pair(""s, 0),
		make_pair("0"s, 0),
		make_pair("0.1"s, 0.1),
		make_pair("0.2"s, 0.2),
		make_pair("0.12"s, 0.12),
		make_pair("0.123"s, 0.123),
		make_pair("0.124"s, 0.124),
		make_pair("0.125"s, 0.125),
		make_pair("0.126"s, 0.126),
		make_pair("0.127"s, 0.127),
		make_pair("0.128"s, 0.128),
		make_pair("0.129"s, 0.129),
		make_pair("0.001"s, 0.001),
		make_pair("0.004"s, 0.004),
		make_pair("0.005"s, 0.005),
		make_pair("0.0001"s, 0.0001),
		make_pair("0.0004"s, 0.0004),
		make_pair("0.0005"s, 0.0005),
		make_pair("0.000000001"s, 0.000000001),
		make_pair("00000000123456.123"s, 123456.123),
		make_pair("123456.123"s, 123456.123),
		make_pair("123456.124"s, 123456.124),
		make_pair("123456.125"s, 123456.125),
		make_pair("123456.126"s, 123456.126),
		make_pair("123456.127"s, 123456.127),
		make_pair("123456.128"s, 123456.128),
		make_pair("123456.129"s, 123456.129),
		make_pair("123456"s, 123456),
		make_pair("123456"s, 123456),
		make_pair("123456.001"s, 123456.001),
		make_pair("123456.004"s, 123456.004),
		make_pair("123456.005"s, 123456.005),
		make_pair("123456.0001"s, 123456.0001),
		make_pair("123456.0004"s, 123456.0004),
		make_pair("123456.0005"s, 123456.0005),
		make_pair("123456.000000001"s, 123456.000000001),
		make_pair("11123123123.125", 11123123123.125),

	};

	for(auto &item: arr)
	{
		c_float		num(item.first);

		if(num.Get() == item.second) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed due to (" << num.Get() << " != " << item.second << ")" << endl;
		}

	}

	return result;
}


bool RunTest2()
{
	auto							result = true;
	vector<pair<string, double>>	arr = {
		make_pair(""s, 0),
		make_pair("0"s, 0),
		make_pair("0.1"s, 0.1),
		make_pair("0.2"s, 0.2),
		make_pair("0.12"s, 0.12),
		make_pair("0.123"s, 0.123),
		make_pair("0.124"s, 0.124),
		make_pair("0.125"s, 0.125),
		make_pair("0.126"s, 0.126),
		make_pair("0.127"s, 0.127),
		make_pair("0.128"s, 0.128),
		make_pair("0.129"s, 0.129),
		make_pair("0.001"s, 0.001),
		make_pair("0.004"s, 0.004),
		make_pair("0.005"s, 0.005),
		make_pair("0.0001"s, 0.0001),
		make_pair("0.0004"s, 0.0004),
		make_pair("0.0005"s, 0.0005),
		make_pair("0.0006"s, 0.0006),
		make_pair("0.0007"s, 0.0007),
		make_pair("0.0008"s, 0.0008),
		make_pair("0.0009"s, 0.0009),
		make_pair("0.00099"s, 0.00099),
		make_pair("0.000000001"s, 0.000000001),
		make_pair("00000000123456.123"s, 123456.123),
		make_pair("123456.1233"s, 123456.1233),
		make_pair("123456.1244"s, 123456.1244),
		make_pair("123456.1255"s, 123456.1255),
		make_pair("123456.1266"s, 123456.1266),
		make_pair("123456.1277"s, 123456.1277),
		make_pair("123456.1288"s, 123456.1288),
		make_pair("123456.1299"s, 123456.1299),
		make_pair("123456"s, 123456),
		make_pair("123456"s, 123456),
		make_pair("123456.001"s, 123456.001),
		make_pair("123456.004"s, 123456.004),
		make_pair("123456.005"s, 123456.005),
		make_pair("123456.0001"s, 123456.0001),
		make_pair("123456.0004"s, 123456.0004),
		make_pair("123456.0005"s, 123456.0005),
		make_pair("123456.000000001"s, 123456.000000001),
		make_pair("11123123123.125", 11123123123.125),

	};

	for(auto &item: arr)
	{
		c_float		num(item.first, 3);

		if(num.Get() == item.second) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << num.Get() << " != " << item.second << ")" << endl;
		}

	}

	return result;
}

bool RunTest3()
{
	auto							result = true;
	vector<pair<double, double>>	arr = {
		make_pair(0, 0),
		make_pair(0.1, 0.1),
		make_pair(0.2, 0.2),
		make_pair(0.12, 0.12),
		make_pair(0.123, 0.123),
		make_pair(0.124, 0.124),
		make_pair(0.125, 0.125),
		make_pair(0.126, 0.126),
		make_pair(0.127, 0.127),
		make_pair(0.128, 0.128),
		make_pair(0.129, 0.129),
		make_pair(0.001, 0.001),
		make_pair(0.004, 0.004),
		make_pair(0.005, 0.005),
		make_pair(0.0001, 0.0001),
		make_pair(0.0004, 0.0004),
		make_pair(0.0005, 0.0005),
		make_pair(0.000000001, 0.000000001),
		make_pair(00000000123456.123, 123456.123),
		make_pair(123456.123, 123456.123),
		make_pair(123456.124, 123456.124),
		make_pair(123456.125, 123456.125),
		make_pair(123456.126, 123456.126),
		make_pair(123456.127, 123456.127),
		make_pair(123456.128, 123456.128),
		make_pair(123456.129, 123456.129),
		make_pair(123456, 123456),
		make_pair(123456, 123456),
		make_pair(123456.001, 123456.001),
		make_pair(123456.004, 123456.004),
		make_pair(123456.005, 123456.005),
		make_pair(123456.0001, 123456.0001),
		make_pair(123456.0004, 123456.0004),
		make_pair(123456.0005, 123456.0005),
		make_pair(11123123123.1234, 11123123123.1234),
		make_pair(11123123123.125, 11123123123.125),
		make_pair(123456.000000001, 123456.000000001)

	};

	for(auto &item: arr)
	{
		c_float		num(item.first);

		if(num.Get() == item.second) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << num.Get() << " != " << item.second << ")" << endl;
		}
	}

	return result;
}

bool RunTest4()
{
	auto							result = true;
	vector<pair<double, double>>	arr = {
		make_pair(0, 0),
		make_pair(0.1, 0.1),
		make_pair(0.2, 0.2),
		make_pair(0.12, 0.12),
		make_pair(0.123, 0.123),
		make_pair(0.124, 0.124),
		make_pair(0.125, 0.125),
		make_pair(0.126, 0.126),
		make_pair(0.127, 0.127),
		make_pair(0.128, 0.128),
		make_pair(0.129, 0.129),
		make_pair(0.001, 0.001),
		make_pair(0.004, 0.004),
		make_pair(0.005, 0.005),
		make_pair(0.0001, 0.0001),
		make_pair(0.0004, 0.0004),
		make_pair(0.0005, 0.0005),
		make_pair(0.9995, 0.9995),
		make_pair(0.000000001, 0.000000001),
		make_pair(00000000123456.123, 123456.123),
		make_pair(123456.123, 123456.123),
		make_pair(123456.124, 123456.124),
		make_pair(123456.125, 123456.125),
		make_pair(123456.126, 123456.126),
		make_pair(123456.127, 123456.127),
		make_pair(123456.128, 123456.128),
		make_pair(123456.129, 123456.129),
		make_pair(123456, 123456),
		make_pair(123456, 123456),
		make_pair(123456.001, 123456.001),
		make_pair(123456.004, 123456.004),
		make_pair(123456.005, 123456.005),
		make_pair(123456.0001, 123456.0001),
		make_pair(123456.0004, 123456.0004),
		make_pair(123456.0005, 123456.0005),
		make_pair(123456.000000001, 123456.000000001),
		make_pair(25000000, 25000000),
		make_pair(25000000.123, 25000000.123),
		make_pair(125000000.1234, 125000000.1234),
		make_pair(123123123.1234, 123123123.1234),
		make_pair(223123123.1234, 223123123.1234),
		make_pair(323123123.1234, 323123123.1234),
		make_pair(423123123.1234, 423123123.1234),
		make_pair(523123123.1234, 523123123.1234),
		make_pair(623123123.1234, 623123123.1234),
		make_pair(723123123.1234, 723123123.1234),
		make_pair(823123123.1234, 823123123.1234),
		make_pair(923123123.1234, 923123123.1234),
		make_pair(1123123123.1234, 1123123123.1234),
		make_pair(2123123123.1234, 2123123123.1234),
		make_pair(3123123123.1234, 3123123123.1234),
		make_pair(4123123123.1234, 4123123123.1234),
		make_pair(5123123123.1234, 5123123123.1234),
		make_pair(6123123123.1234, 6123123123.1234),
		make_pair(7123123123.1234, 7123123123.1234),
		make_pair(8123123123.1234, 8123123123.1234),
		make_pair(9123123123.1234, 9123123123.1234),
		make_pair(10123123123.1234, 10123123123.1234),
		make_pair(11123123123.1234, 11123123123.1234),
		// make_pair(123456123456789.000000001, 123456123456789)

	};

	for(auto &item: arr)
	{
		c_float		num(item.first, 3);

		if(num.Get() == item.second) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << num.Get() << " != " << item.second << ")" << endl;
		}
	}

	return result;
}


auto RunTest5()
{	auto							result = true;
	vector<pair<string, string>>	arr = {
		make_pair("sdlkcm", "0"),
		make_pair("", "0"),
		make_pair("0.000", "0"),
		make_pair("0000.000", "0"),
		make_pair("1E+2", "100"),
		make_pair("1E+1", "10"),
		make_pair("1E+0", "1"),
		make_pair("2E+0", "2"),
		make_pair("2E-0", "2"),
		make_pair("1E-0", "1"),
		make_pair("1E-1", "0.1"),
		make_pair("1E-2", "0.01"),
		make_pair("1E-3", "0"),
		make_pair("-1E+2", "-100"),
		make_pair("-1E+1", "-10"),
		make_pair("-1E+0", "-1"),
		make_pair("-2E+0", "-2"),
		make_pair("-2E-0", "-2"),
		make_pair("-1E-0", "-1"),
		make_pair("-1E-1", "-0.1"),
		make_pair("-1E-2", "-0.01"),
		make_pair("-1E-3", "-0"),
		make_pair("12", "12"),
		make_pair("12.34", "12.34"),
		make_pair("12.345", "12.35"),
		make_pair("11123123123.125", "11123123123.13"),
	};

	for(auto &item: arr)
	{
		c_float		num(item.first);

		if(string(num) == item.second) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << num.Get() << " != " << item.second << ")" << endl;
		}
	}

	return result;
}

auto RunTest6()
{
	auto	result = true;
	vector<tuple<string, string, double>>	arr = {
		make_tuple<string, string, double>("", "1.125", 1.125),
		make_tuple<string, string, double>("001.100", "01.10", 2.2),
		make_tuple<string, string, double>("001.100", "-01.10", 0),
		make_tuple<string, string, double>("1.985", "0.005", 1.99),
		make_tuple<string, string, double>("1.000000000001", "1.0000000001", 2.000000000101),
		make_tuple<string, string, double>("101.1", "0.01", 101.11),
		make_tuple<string, string, double>("asd", "cxv", 0),
		make_tuple<string, string, double>("1.9", "1.1", 3),
		make_tuple<string, string, double>("-1.9", "1.1", -0.8),
		make_tuple<string, string, double>("22000111.1", "22000111.1", 44000222.2),
		make_tuple<string, string, double>("122000111.005", "122000111.0051", 244000222.0101),
	};

	for(auto &item: arr)
	{
		c_float		num_1(get<0>(item));
		c_float		num_2(get<1>(item));

		// --- it failing -1.9 + 1.1 == -0.8 test (difference is 2.2e-16)
		if((num_1 + num_2).Get() - get<2>(item) < 1.0e-13) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << get<0>(item) << " + " << get<1>(item) << " != " << (num_1 + num_2).Get() << " (expected: " << get<2>(item) << ", diff = " << (num_1 + num_2).Get() - get<2>(item)<< "))" << endl;
		}
	}


	return result;
}

auto RunTest7()
{
	auto	result = true;
	vector<tuple<string, string, double>>	arr = {
		make_tuple<string, string, double>("", "1.125", -1.125),
		make_tuple<string, string, double>("001.100", "01.10", 0),
		make_tuple<string, string, double>("1.985", "0.005", 1.98),
		make_tuple<string, string, double>("1.000000000001", "1.0000000001", -9.89999e-11),
		make_tuple<string, string, double>("101.1", "0.01", 101.09),
		make_tuple<string, string, double>("asd", "cxv", 0),
		make_tuple<string, string, double>("1.9", "1.1", 0.8),
		make_tuple<string, string, double>("22000111.1", "22000111.1", 0),
		make_tuple<string, string, double>("122000111.005", "122000111.004", 0.01),
	};

	for(auto &item: arr)
	{
		c_float		num_1(get<0>(item));
		c_float		num_2(get<1>(item));

		if((num_1 - num_2).Get() - get<2>(item) < 1e-10) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << get<0>(item) << " - " << get<1>(item) << " != " << (num_1 - num_2).Get() << ")" << endl;
		}
	}


	return result;
}

auto RunTest8()
{
	auto	result = true;
	vector<tuple<string, string, double>>	arr = {
		make_tuple<string, string, double>("2", "-2.125", 2.0*(-2.125)),
		make_tuple<string, string, double>("001.100", "01.10", 1.1*1.1),
		make_tuple<string, string, double>("1.985", "0.005",1.985*0.005),
		make_tuple<string, string, double>("1.000000000001", "1.0000000001", 1.000000000001*1.0000000001),
		make_tuple<string, string, double>("101.1", "0.01", 101.1 * 0.01),
		make_tuple<string, string, double>("asd", "cxv", 0),
		make_tuple<string, string, double>("1.9", "1.1", 1.9*1.1),
		make_tuple<string, string, double>("22000111.1", "2", 22000111.1*2),
		make_tuple<string, string, double>("122000111.005", "5.1", 122000111.005*5.1),
	};

	for(auto &item: arr)
	{
		c_float		num_1(get<0>(item));
		c_float		num_2(get<1>(item));
		c_float		op_result(get<2>(item));

		if((num_1 * num_2) == op_result) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << get<0>(item) << " * " << get<1>(item) << " == " << get<2>(item) << ")" << endl;
			cout << "must be failed on (" << string(num_1) << " * " << string(num_2) << " = " << string(op_result)  << " == " << string((num_1 * num_2)) << ")" << endl;
		}
	}

	return result;
}

auto RunTest9()
{
	auto	result = true;
	vector<tuple<string, string, double>>	arr = {
		make_tuple<string, string, double>("2", "-2.125", 2.0/(-2.125)),
		make_tuple<string, string, double>("001.100", "01.10", 1.1/1.1),
		make_tuple<string, string, double>("1.985", "0.005",1.985/0.005),
		make_tuple<string, string, double>("1.000000000001", "1.0000000001", 1.000000000001/1.0000000001),
		make_tuple<string, string, double>("101.1", "0.01", 101.1 / 0.01),
		make_tuple<string, string, double>("asd", "1", 0),
		make_tuple<string, string, double>("1.9", "1.1", 1.9/1.1),
		make_tuple<string, string, double>("22000111.1", "2", 22000111.1/2),
		make_tuple<string, string, double>("12000111.01", "5", 12000111.01/5),
	};

	for(auto &item: arr)
	{
		c_float		num_1(get<0>(item));
		c_float		num_2(get<1>(item));
		c_float		op_result(get<2>(item));

		if((num_1 / num_2) == op_result) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << get<0>(item) << " / " << get<1>(item) << " == " << get<2>(item) << ")" << endl;
			cout << "must be failed on (" << string(num_1) << " / " << (num_2) << " = " << (op_result)  << " == " << string((num_1 / num_2)) << ", diff " << op_result - num_1 / num_2 << ")" << endl;
		}
	}


	return result;
}

auto RunTest10()
{
	auto	result = true;
	vector<tuple<string, string>>	arr = {
		make_tuple<string, string>("20", "20.0"),
		make_tuple<string, string>("20.095", "20.095"),
		make_tuple<string, string>("", "0000.0"),
		make_tuple<string, string>("abc", "def"),
		make_tuple<string, string>("-10.009", "-10.009"),
	};

	for(auto &item: arr)
	{
		c_float		num_1(get<0>(item));
		c_float		num_2(get<1>(item));

		if((num_1 == num_2)) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << get<0>(item) << " == " << get<1>(item) << ")" << endl;
			cout << "must be failed on (" << string(num_1) << " == " << string(num_2) << ")" << endl;
		}
	}


	return result;
}

auto RunTest11()
{
	auto	result = true;
	vector<tuple<string, string>>	arr = {
		make_tuple<string, string>("20", "20.01"),
		make_tuple<string, string>("20.09", "20.1"),
		make_tuple<string, string>("-0.005", "0000.0"),
		make_tuple<string, string>("-10000000", "def"),
		make_tuple<string, string>("-10.09", "-10.01"),
	};

	for(auto &item: arr)
	{
		c_float		num_1(get<0>(item));
		c_float		num_2(get<1>(item));

		if((num_1 < num_2)) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << get<0>(item) << " <= " << get<1>(item) << ")" << endl;
		}
	}


	return result;
}

auto RunTest12()
{
	auto	result = true;
	vector<tuple<string, string>>	arr = {
		make_tuple<string, string>("20", "20.01"),
		make_tuple<string, string>("20.09", "20.1"),
		make_tuple<string, string>("-0.005", "0000.0"),
		make_tuple<string, string>("-10000000", "def"),
		make_tuple<string, string>("-10.09", "-10.01"),
	};

	for(auto &item: arr)
	{
		c_float		num_1(get<0>(item));
		c_float		num_2(get<1>(item));

		if((num_2 > num_1)) {}
		else
		{
			result = false;
			cout << "[" << __LINE__ << "] failed on (" << get<1>(item) << " > " << get<0>(item) << ")" << endl;
		}
	}


	return result;
}

auto RunTest13()
{
	auto	result = true;


	for(auto i1 = 0; i1 < 100; ++i1)
	{
		for(auto i2 = 0; i2 < 100; ++i2)
		{
			c_float	num(to_string(i1) + "." + to_string(i2));
			string	target = CutTrailingZeroes(to_string(i1) + "." + to_string(i2));

			if(string(num) == target) {}
			else
			{
				result = false;
				cout << "[" << __LINE__ << "] failed (string(" << num << ") != " << i1 << "." << i2 << ")" << endl;
			}
		}
	}

	return result;
}

auto RunTest14()
{
	auto	result = true;

	for(auto i1 = 0; i1 < 100; ++i1)
	{
		for(auto i = 0; i < 10000; ++i)
		{
			c_float	num(to_string(i1) + "." + to_string(i), 4);
			string	target = CutTrailingZeroes(to_string(i1) + "." + to_string(i));

			if(string(num) == target) {}
			else
			{
				result = false;
				cout << "[" << __LINE__ << "] failed (string(" << num << ") != " << i1 << "." << i << ")" << endl;
			}
		}
	}

	return result;
}

auto RunTest15()
{
	auto	result = true;

	for(auto i1 = 0; i1 < 100; ++i1)
	{
		for(auto i = 0; i < 100; ++i)
		{
			c_float	num(i1 + i/100);

			if(num.Get() == i1 + i/100) {}
			else
			{
				result = false;
				cout << "[" << __LINE__ << "] failed on (" << i1 << "." << i << ")" << endl;
			}
		}
	}

	return result;
}

auto RunTest16()
{
	auto	result = true;

	for(auto i1 = 0; i1 < 100; ++i1)
	{
		for(auto i = 0; i < 10000; ++i)
		{
			c_float	num(i1 + i/10000, 4);

			if(num.Get() == i1 + i/10000) {}
			else
			{
				result = false;
				cout << "[" << __LINE__ << "] failed on (" << i1 << "." << i << ")" << endl;
			}
		}
	}

	return result;
}

vector<bool (*)(void)>	func_v = {
	RunTest1,
	RunTest2,
	RunTest3,
	RunTest4,
	RunTest5,
	RunTest6,
	RunTest7,
	RunTest8,
	RunTest9,
	RunTest10,
	RunTest11,
	RunTest12,
	RunTest13,
	RunTest14,
	RunTest15,
};


bool Test_ru_RU(CMysql *db)
{
	auto	result = true;

	setlocale(LC_ALL, "ru_RU.utf8");

	for(auto f: func_v)
	{
		result &= f();
	}

	return result;
}

bool Test_en_US(CMysql *db)
{
	auto	result = true;

	setlocale(LC_ALL, "en_US.utf8");

	for(auto f: func_v)
	{
		result &= f();
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

	if(!Test_ru_RU(&db))
	{
		testing_success = false;
		cout << "Test_ru_RU failed" << endl;
	}

	if(!Test_en_US(&db))
	{
		testing_success = false;
		cout << "Test_en_US failed" << endl;
	}

	if(testing_success)
		cout << "Test passed" << endl;

	return(0);
}

