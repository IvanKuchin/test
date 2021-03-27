#include <string>
#include <iostream>
#include "ccgi.h"
#include "cmysql.h"
#include "cuser.h"
#include "cstatistics.h"
// #include "utilities_timecard.h"
#include "utilities_common.h"
#include "localy.h"

using namespace std;

auto repeat(string src, int times)
{
	auto result = src;

	for(auto i = 0; i < times - 1; ++i) result += src;

	return result;
}

bool Test1()
{
	auto			result = true;
	auto			error_message = ""s;
	auto			translation = ""s;
	// char			buffer[1024];
	vector<string>	number_test_fail = {"123.12", "0x123", "", "1E-1", "-1", "-1.234", "-0", "123ABC", "0x7"};
	vector<string>	number_test_success = {"12312", "0000000000000000000000000", "", "1234567890965432123457908642", "1234567890", "007"};
	vector<string>	float_test_fail = {"1231..2", ".12312", "12312.", "12.3.12", "0x1.23", "", ".1E-1", "-1", "-1.234", "-0", "123ABC.", "0x7", "007A", "007scdlkm", "007.sdkjcnsdc", "007,98797", "007!"};
	vector<string>	float_test_success = {"1231.2", "1.2312", "123.12", "0000000000000000000000000.00000000000000000", "", "1.234567890965432123457908642", "12345.67890", "0.07", "12312", "0000000000000000000000000", "", "1234567890965432123457908642", "1234567890", "007.234098", "007"};
	vector<string>	date_test_fail = {"0/0/00", "0/1/11", "1/0/11", "1/1/1899", "1/1/2109", "31/2/2019", "30/2/2019", "0-0-0000", "12312.", "12.3.12", "0x1.23", "", ".1E-1", "-1", "-1.234", "-0", "123ABC.", "0x7", "007A", "007scdlkm", "007.sdkjcnsdc", "007,98797", "007!"};
	vector<string>	date_test_success = {"1/2/00", "1/2/11", "1/2/2018", "11/12/2000", "28/2/2019", "29/2/2020", "29/2/2020", "31/12/20"};
	vector<string>	email_test_fail = {"0/@0/00", "@0111", "@0111.123", "1@1.1" "user@domain.longdomain", "U_S_E_R@Domaincom", "my@.mail.com", "my@-mail.com", "my@mail-.com", "my@mail..com", "my@mail,com", "my,name@mail.com", "my!name@mail.com", "my.name@my!mail.com", "my#name@mail.com", "my.name@my#mail.com", "my_super_long_first_Name_and_super_long_last_Name@my-super-long-grand-domain.my-super-long-sub-domain.my-super-long-domain.com.edu"};
	vector<string>	email_test_success = {"a@ab.com", "user@mail.mail", "1@24.11", "USER@DOMAIN.NET", "U_S_E_R@domain.com", "my.@mail.com", ".my@mail.com", "my_@mail.com", "_my@mail.com", "U_S_E_R@my-domain.com", "first_name.last_name@secondary.my-domain.com"};
	vector<string>	timeentry_test_fail = {"8.8.8.8.8", "8,8.009", "1,a,1", "a", "0.10", "0,", "00", "000000000.00000000000,0", "8,8.084", "1.124,1,12", "1-E10,1230", "01,01,01,01", "1-E10,1230","12,123,123.00,24.24", "0", "0.0", "1.125,1,12"};
	vector<string>	timeentry_test_success = {"8", "", "8,-1,8", ",", "", "8,8.8", "8,8.08", "8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8"};
	auto			rus_alphabet = "Аа Бб Вв Гг Дд Ее Ёё Жж Зз Ии Йй Кк Лл Мм Нн Оо Пп Рр Сс Тт Уу Фф Хх Цц Чч Шш Щщ Ъъ Ыы Ьь Ээ Юю Яя"s;
	auto			us_alphabet = "Aa Bb Cc Dd Ee Ff Gg Hh Ii Jj Kk Ll Mm Nn Oo Pp Qq Rr Ss Tt Uu Vv Ww Xx Yy Zz"s;

	if(CheckHTTPParam_Text(rus_alphabet) == rus_alphabet) {}
	else
	{
		result = false;
		cout << "failed on basic RUS alphabet" << endl;
	}

	if(CheckHTTPParam_Text(repeat(rus_alphabet, 150)).length() == CheckHTTPParam_Text(repeat(rus_alphabet, 150)).length()) {}
	else
	{
		result = false;
		cout << "failed on RUS overflow" << endl;
	}

	if(CheckHTTPParam_Text(us_alphabet) == us_alphabet) {}
	else
	{
		result = false;
		cout << "failed on basic US alphabet" << endl;
	}

	if(CheckHTTPParam_Text(repeat(us_alphabet, 350)).length() == CheckHTTPParam_Text(repeat(us_alphabet, 350)).length()) {}
	else
	{
		result = false;
		cout << "failed on US-overflow" << endl;
	}

	if(CheckHTTPParam_Text("un\"ed") == "un&quot;ed") {}
	else
	{
		result = false;
		cout << "failed on quoted test" << endl;
	}

	if(CheckHTTPParam_Text("t		a		b		e		d") == "t  a  b  e  d") {}
	else
	{
		result = false;
		cout << "failed on tab test" << endl;
	}

	if(CheckHTTPParam_Text("\\slashed") == "&#92;slashed") {}
	else
	{
		result = false;
		cout << "failed on slash test" << endl;
	}

	if(CheckHTTPParam_Text("<vw>i</vw>") == "&lt;vw&gt;i&lt;/vw&gt;") {}
	else
	{
		result = false;
		cout << "failed on <> test" << endl;
	}

	if(CheckHTTPParam_Text("line<br>line<BR>line<Br>line<bR>") == "line&lt;br&gt;line&lt;BR&gt;line&lt;Br&gt;line&lt;bR&gt;") {}
	else
	{
		result = false;
		cout << "failed on <br> test" << endl;
	}

	if((translation = RemoveAllNonAlphabetSymbols("/\\	<>\"-;':,%N.`:'apo';`,")) == "apo") {}
	else
	{
		result = false;
		cout << "failed on RemoveAllNonAlphabetSymbols(.`:'apo';`)=" << translation << ", test" << endl;
	}

	if(RemoveSpecialHTMLSymbols("<tag>\"value—value\"</tag>") == "&lt;tag&gt;&quot;value—value&quot;&lt;/tag&gt;") {}
	else
	{
		result = false;
		cout << "failed on RemoveSpecialHTMLSymbols(<tag>\"value—value\"</tag>), test" << endl;
	}

	if((translation = RemoveSpecialHTMLSymbols("<tag>\"chanel№5\"</tag>")) == "&lt;tag&gt;&quot;chanel№5&quot;&lt;/tag&gt;") {}
	else
	{
		result = false;
		cout << "failed on RemoveSpecialHTMLSymbols(<tag>\"chanel№5\"</tag>)=" << translation << ", test" << endl;
	}

	if((translation = RemoveSpecialHTMLSymbols("<tag>\"chanel№5\"</tag>\\\\\\")) == "&lt;tag&gt;&quot;chanel№5&quot;&lt;/tag&gt;") {}
	else
	{
		result = false;
		cout << "failed on RemoveSpecialHTMLSymbols(\"<tag>\"chanel№5\"</tag>\\\\\\\")=" << translation << ", test" << endl;
	}

	if((translation = ReplaceDoubleQuoteToQuote("Zdes bwl \"Vasya\"")) == "Zdes bwl 'Vasya'") {}
	else
	{
		result = false;
		cout << "failed on ReplaceDoubleQuoteToQuote(\"Zdes bwl \"Vasya\"\")=" << translation << ", test" << endl;
	}

	if((translation = ReplaceCRtoHTML("Zdes bwl \n\"Vasya\"")) == "Zdes bwl <bR>\"Vasya\"") {}
	else
	{
		result = false;
		cout << "failed on ReplaceCRtoHTML(\"Zdes bwl \n\"Vasya\"\")=" << translation << ", test" << endl;
	}
	if((translation = ReplaceCRtoHTML("Zdes bwl \n\"Vasya\"\r")) == "Zdes bwl <bR>\"Vasya\"<Br>") {}
	else
	{
		result = false;
		cout << "failed on ReplaceCRtoHTML(\"Zdes bwl \n\"Vasya\"\")=" << translation << ", test" << endl;
	}
	if((translation = ReplaceCRtoHTML("Zdes bwl \n\"Vasya\"\r\n")) == "Zdes bwl <bR>\"Vasya\"<br>") {}
	else
	{
		result = false;
		cout << "failed on ReplaceCRtoHTML(\"Zdes bwl \n\"Vasya\"\r\n\")=" << translation << ", test" << endl;
	}


	if((translation = SymbolReplace_KeepDigitsOnly("Zdes bwl \"Vasya\"")) == "") {}
	else
	{
		result = false;
		cout << "failed on SymbolReplace_KeepDigitsOnly(\"Zdes bwl \"Vasya\"\")=" << translation << ", test" << endl;
	}

	if((translation = SymbolReplace_KeepDigitsOnly("ЙЦУКЕНГШЩЗ1234567890йцукенгшщз1234567890qwertyuiop")) == "12345678901234567890") {}
	else
	{
		result = false;
		cout << "failed on SymbolReplace_KeepDigitsOnly(\"1234567890йцукенгшщз1234567890qwertyuiop\")=" << translation << ", test" << endl;
	}

	if((translation = RemoveSpecialSymbols("\\ 123	chanel№5	——— \\\\")) == " 123 chanel№5 ——— ") {}
	else
	{
		result = false;
		cout << "failed on RemoveSpecialSymbols(\"\\ 123	chanel№5	——— \\\\\")=" << translation << ", test" << endl;
	}

	if((translation = RemoveAllNonAlphabetSymbols("\\&lt;tag&gt; 123	chanel№5	——— &lt;/tag&gt;\\\\")) == "tag123chanel5tag") {}
	else
	{
		result = false;
		cout << "failed on RemoveAllNonAlphabetSymbols(\"\\&lt;tag&gt; 123	chanel№5	——— &lt;/tag&gt;\\\\\")=" << translation << ", test" << endl;
	}

	if((translation = RemoveAllNonAlphabetSymbols("\\<tag> 123	chaNel№5	——— </tag>then , after party ...\\\\")) == "tag123chael5tagthenafterparty") {}
	else
	{
		result = false;
		cout << "failed on RemoveAllNonAlphabetSymbols(\"\\<tag> 123	chaNel№5	——— </tag>then , after party ...\\\\\")=" << translation << ", test" << endl;
	}

	if((translation = RemoveAllNonAlphabetSymbols(" \\ / 	< > \"'; : ` . , % - N &lt;&gt;&quot;&#92;")) == "") {}
	else
	{
		result = false;
		cout << "failed on RemoveAllNonAlphabetSymbols(\" \\ / 	< > \"'; : ` . , % - N &lt;&gt;&quot;&#92;\")=" << translation << ", test" << endl;
	}

	if((translation = ConvertHTMLToText("&lt;&gt;&quot;&#92;")) == "<>\"&#92;") {}
	else
	{
		result = false;
		cout << "failed on ConverHTMLToText(\"&lt;&gt;&quot;&#92;\")=" << translation << ", test" << endl;
	}

	for(auto str: number_test_fail)
	{
		if(CheckHTTPParam_Number(str) == "") {}
		else
		{
			result = false;
			cout << "failure number test failed: " << str << endl;
		}
	}

	for(auto str: number_test_success)
	{
		if(CheckHTTPParam_Number(str) == str) {}
		else
		{
			result = false;
			cout << "success number test failed: " << str << endl;
		}
	}

	for(auto str: float_test_fail)
	{
		if(CheckHTTPParam_Float(str) == "") {}
		else
		{
			result = false;
			cout << "failure float test failed: " << str << endl;
		}
	}

	for(auto str: float_test_success)
	{
		if(CheckHTTPParam_Float(str) == str) {}
		else
		{
			result = false;
			cout << "success float test failed: " << str << endl;
		}
	}

	for(auto str: date_test_fail)
	{
		if(CheckHTTPParam_Date(str) == "") {}
		else
		{
			result = false;
			cout << "failure date test failed: " << str << endl;
		}
	}

	for(auto str: date_test_success)
	{
		if(CheckHTTPParam_Date(str) == str) {}
		else
		{
			result = false;
			cout << "success date test failed: " << str << endl;
		}
	}

	for(auto str: email_test_fail)
	{
		if(CheckHTTPParam_Email(str) == "") {}
		else
		{
			result = false;
			cout << "failure email test failed: " << str << endl;
		}
	}

	for(auto str: email_test_success)
	{
		if(CheckHTTPParam_Email(str) == str) {}
		else
		{
			result = false;
			cout << "success email test failed: " << str << endl;
		}
	}

	return result;
}

bool Test2()
{
	auto	result = true;
	auto	error_message = ""s;

	if(split(",2,,4,5,,7,,9,,,12,", ',').size() == 6) {}
	else
	{
		result = false;
		cout << "fail to split" << endl;
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

	if(testing_success)
		cout << "Test passed" << endl;

	return(0);
}

