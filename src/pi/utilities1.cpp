#include "utilities.h"

void crash_handler(int sig)
{
	void	   *array[10];
	size_t		nptrs;
	char	   **strings;

	// get void*'s for all entries on the stack
	nptrs = backtrace(array, 10);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, nptrs, STDERR_FILENO);


	// --- try to print out to the CLog
	strings = backtrace_symbols(array, nptrs);
	if(strings)
	{
		for(unsigned int i = 0; i < nptrs; i++)
		{
			MESSAGE_ERROR("", "", string(strings[i]));
		}

		free(strings);
	}

	exit(1);
}

auto GetLocale()
{
	auto	result = ""s;

	// MESSAGE_DEBUG("", "", "start");

	result = setlocale(LC_ALL, NULL);

	if(result.empty()) MESSAGE_ERROR("", "", "fail to define locale");

	// MESSAGE_DEBUG("", "", "finish(result length is " + to_string(result.length()) + ")");

	return result;
}

std::string rtrim(std::string& str)
{
	str.erase(str.find_last_not_of(' ')+1);		 //suffixing spaces
	return str;
}

std::string ltrim(std::string& str)
{
	str.erase(0, str.find_first_not_of(' '));	   //prefixing spaces
	return str;
}

std::string trim(std::string& str)
{
	rtrim(str);
	ltrim(str);
	return str;
}

string	quoted(string src)
{
	return '"' + src + '"';
}

vector<string>	quoted(const vector<string> &src)
{
	vector<string>	result;
	result.reserve(src.size()); // --- reduce probability of memory reallocation

	for(const string &item: src)
	{
		result.push_back(quoted(item));
	}

	return result;
}

auto toUpper(const string &src) -> string
{
	auto	result(""s);
	auto	locale_str = GetLocale();

	MESSAGE_DEBUG("", "", "start");

	if(locale_str.length())
	{
		auto	wtemp = multibyte_to_wide(src);
		locale	loc(locale_str.c_str());

		for(auto &c: wtemp) c = toupper(c, loc);

		result = wide_to_multibyte(wtemp);
	}
	else
	{
		MESSAGE_ERROR("", "", "fail to define locale. Impossible run toUpper")
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto toLower(const string &src) -> string
{
	auto	result(""s);
	auto	locale_str = GetLocale();

	MESSAGE_DEBUG("", "", "start");

	if(locale_str.length())
	{
		auto	wtemp = multibyte_to_wide(src);
		locale	loc(locale_str.c_str());

		for(auto &c: wtemp) c = tolower(c, loc);

		result = wide_to_multibyte(wtemp);
	}
	else
	{
		MESSAGE_ERROR("", "", "fail to define locale. Impossible run toLower")
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

string	GetPasswordNounsList(CMysql *db)
{
	MESSAGE_DEBUG("", "", "start");

	auto 	result = ""s;
	auto	affected = db->Query("SELECT COUNT(*) as `total` FROM `password_dictionary_nouns`;");

	if(affected)
	{
		auto		total_number_of_words = stoi(db->Get(0, "total")) - 1;

		affected = db->Query("SELECT * FROM `password_dictionary_nouns` WHERE `id` in (round(rand()*" + to_string(total_number_of_words) + ") + 1, round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1)");
		for(auto i = 0; i < affected; ++i)
		{
			if(result.length()) result += ",";
			result += string("\"") + db->Get(i, "word") + "\"";
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "password_dictionary_nouns is empty");
	}

	MESSAGE_DEBUG("", "", "finish (result length is " + to_string(result.length()) + ")");

	return result;
}

string	GetPasswordCharacteristicsList(CMysql *db)
{
	MESSAGE_DEBUG("", "", "start");

	auto 	result = ""s;
	auto	affected = db->Query("SELECT COUNT(*) as `total` FROM `password_dictionary_characteristics`;");

	if(affected)
	{
		auto		total_number_of_words = stoi(db->Get(0, "total"));

		affected = db->Query("SELECT * FROM `password_dictionary_characteristics` WHERE `id` in (round(rand()*" + to_string(total_number_of_words) + ") + 1, round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1)");
		for(auto i = 0; i < affected; ++i)
		{
			if(result.length()) result += ",";
			result += string("\"") + db->Get(i, "word") + "\"";
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "password_dictionary_characteristics is empty");
	}

	MESSAGE_DEBUG("", "", "finish (result length is " + to_string(result.length()) + ")");

	return result;
}

string	GetPasswordAdjectivesList(CMysql *db)
{
	MESSAGE_DEBUG("", "", "start");

	auto 	result = ""s;
	auto	affected = db->Query("SELECT COUNT(*) as `total` FROM `password_dictionary_adjectives`;");

	if(affected)
	{
		auto		total_number_of_words = stoi(db->Get(0, "total"));

		affected = db->Query("SELECT * FROM `password_dictionary_adjectives` WHERE `id` in (round(rand()*" + to_string(total_number_of_words) + ") + 1, round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1)");
		for(auto i = 0; i < affected; ++i)
		{
			if(result.length()) result += ",";
			result += string("\"") + db->Get(i, "word") + "\"";
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "password_dictionary_adjectives is empty");
	}

	MESSAGE_DEBUG("", "", "finish (result length is " + to_string(result.length()) + ")");

	return result;
}

string GetRandom(int len)
{
	auto	result = ""s;

	for(auto i = 0; i < len; i++)
	{
		result += (char)('0' + (int)(rand()/(RAND_MAX + 1.0) * 10));
	}

	return result;
}

string DeleteHTML(string src, bool removeBR /* = true*/)
{
	auto			  	result = src;
	string::size_type   firstPos, lastPos;

	MESSAGE_DEBUG("", "", "start (src.len = " + to_string(src.length()) + ")");

	firstPos = result.find("<");
	if(firstPos != string::npos)
	{
		lastPos = result.find(">", firstPos);
		if(lastPos == string::npos) lastPos = result.length();

		while(firstPos != string::npos)
		{
			if(removeBR)
				result.erase(firstPos, lastPos - firstPos + 1);
			else
			{
				string  htmlTag;

				// --- this will run in case "keep BR"
				htmlTag = result.substr(firstPos + 1, lastPos-firstPos-1);
				transform(htmlTag.begin(), htmlTag.end(),htmlTag.begin(), ::tolower);

				if(htmlTag != "br")
				{
					result.erase(firstPos, lastPos - firstPos + 1);
				}
				else
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "(" + src + ")" + "[" + to_string(__LINE__) + "]: keep <br> at pos: " + to_string(firstPos));
					}
				}
			}

			firstPos = result.find("<", firstPos + 1);
			if(firstPos == string::npos) break;
			lastPos = result.find(">", firstPos);
			if(lastPos == string::npos) lastPos = result.length();
		}
	} // --- if "<" found in srcStr

	MESSAGE_DEBUG("", "", "finish(result.len = " + to_string(result.length()) + ")");

	return result;
}

/*
	Delete symbol " from string src
*/
string RemoveQuotas(string src)
{
	auto				result = src;
	string::size_type	pos = 0;

	MESSAGE_DEBUG("", "", "start (src = " + src + ")");

	while((pos = result.find("\"", pos)) != string::npos)
	{
		result.replace(pos, 1, "\\\"");
		pos += 2;
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

/*
	Delete special symbols like \t \\ \<
*/
auto RemoveSpecialSymbols(wstring src) -> wstring
{
	auto					result = src;
	map<wstring, wstring>	map_replacement_1 = {
		{L"\\", L""},
		{L"\t", L" "}
	};


	MESSAGE_DEBUG("", "", "start");

	while ((result.length()) && (result.at(result.length() - 1) == L'\\')) result.replace(result.length() - 1, 1, L"");

	result = ReplaceWstringAccordingToMap(result, map_replacement_1);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto RemoveSpecialSymbols(string src) -> string
{
	return(wide_to_multibyte(RemoveSpecialSymbols(multibyte_to_wide(src))));
}

/*
	Delete special symbols like \t \\ \<
*/
auto RemoveSpecialHTMLSymbols(const wstring &src) -> wstring
{
	auto					result(src);
	wstring::size_type		pos = 0;
	map<wstring, wstring>	map_replacement = {
		{L"\\", L"&#92;"},
		{L"\t", L" "},
		{L"<", L"&lt;"},
		{L">", L"&gt;"},
		{L"\"", L"&quot;"}
	};

	MESSAGE_DEBUG("", "", "start");

	while ((result.length()) && (result.at(result.length() - 1) == L'\\')) result.replace(result.length() - 1, 1, L"");

	result = ReplaceWstringAccordingToMap(result, map_replacement);

	MESSAGE_DEBUG("", "", "finish (result.length = " + to_string(result.length()) + ")");

	return result;
}

auto RemoveSpecialHTMLSymbols(const string &src) -> string
{
	return(wide_to_multibyte(RemoveSpecialHTMLSymbols(multibyte_to_wide(src))));
}

/*
	Change " symbol to " from string src
*/
string ReplaceDoubleQuoteToQuote(string src)
{
	auto				result = src;
	string::size_type	pos = 0;

	while((pos = result.find("\"", pos)) != string::npos)
	{
		result.replace(pos, 1, "'");
		// pos += 2;
	}

	return result;
}


/*
	Change CR/CRLF symbol to <BR> from string src
*/
auto ReplaceCRtoHTML(wstring src) -> wstring
{

	auto					result = src;
	map<wstring, wstring>	map_replacement_1 = {
		{L"\r\n", L"<br>"}
	};
	map<wstring, wstring>	map_replacement_2 = {
		{L"\n", L"<bR>"},
		{L"\r", L"<Br>"}
	};


	MESSAGE_DEBUG("", "", "start");

	result = ReplaceWstringAccordingToMap(result, map_replacement_1);
	result = ReplaceWstringAccordingToMap(result, map_replacement_2);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto ReplaceCRtoHTML(string src) -> string
{
	return(wide_to_multibyte(ReplaceCRtoHTML(multibyte_to_wide(src))));
}

string CleanUPText(const string messageBody, bool removeBR/* = true*/)
{
	auto	  result = messageBody;

	MESSAGE_DEBUG("", "", "start");

	result = DeleteHTML(result, removeBR);
	result = ReplaceDoubleQuoteToQuote(result);
	result = ReplaceCRtoHTML(result);
	result = RemoveSpecialSymbols(result);
	trim(result);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

/*
	Delete any special symbols
	ATTENTION !!!
	use it carefully
	symbol(N) replaced to ""
	Used only for matching duplicates
*/
auto RemoveAllNonAlphabetSymbols(const wstring &src) -> wstring
{
	auto					result(src);
	map<wstring, wstring>	map_replacement_1 = {
		{L"&lt;", L""},
		{L"&gt;", L""},
		{L"&quot;", L""},
		{L"&#92;", L""},
	};
	map<wstring, wstring>	map_replacement_2 = {
		{L" ", L""},
		{L"\\", L""},
		{L"/", L""},
		{L"\t", L""},
		{L"<", L""},
		{L">", L""},
		{L"№", L""},
		{L"—", L""},
		{L"\"", L""},
		{L"'", L""},
		{L";", L""},
		{L":", L""},
		{L"`", L""},
		{L".", L""},
		{L",", L""},
		{L"%", L""},
		{L"-", L""},
		{L"N", L""},
	};


	MESSAGE_DEBUG("", "", "start");

	while ((result.length()) && (result.at(result.length() - 1) == L'\\')) result.replace(result.length() - 1, 1, L"");
	result = ReplaceWstringAccordingToMap(result, map_replacement_1);
	result = ReplaceWstringAccordingToMap(result, map_replacement_2);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto RemoveAllNonAlphabetSymbols(const string &src) -> string
{
	return(wide_to_multibyte(RemoveAllNonAlphabetSymbols(multibyte_to_wide(src))));
}


string ConvertTextToHTML(const string &messageBody)
{
	auto 		result = messageBody;

	MESSAGE_DEBUG("", "", "start");

	result = RemoveSpecialHTMLSymbols(result);
	result = DeleteHTML(result);
	result = ReplaceCRtoHTML(result);
	trim(result);

	MESSAGE_DEBUG("", "", "finish ( result length = " + to_string(result.length()) + ")");

	return result;
}

auto CheckHTTPParam_Text(const string &srcText) -> string
{
	return	ConvertTextToHTML(srcText);
}

auto CheckHTTPParam_Number(const string &srcText) -> string
{
	auto	result = ""s;

	MESSAGE_DEBUG("", "", "start param(" + srcText + ")");

	if(srcText.length())
	{
		smatch	m;
		regex	r("[^0-9]");

		if(regex_search(srcText, m, r))
		{
			MESSAGE_ERROR("", "", "can't convert(" + srcText + ") to number");
		}
		else
		{
			result = srcText;
		}
	}

	MESSAGE_DEBUG("", "", "finish ( result length = " + to_string(result.length()) + ")");

	return	result;
}

string CheckHTTPParam_Float(const string &srcText)
{
	auto	result = ""s;

	MESSAGE_DEBUG("", "", "start param(" + srcText + ")");

	if(srcText.length())
	{
		regex	r("^[0-9]+(\\.[0-9]+)?$");

		if(regex_match(srcText, r))
		{
			result = srcText;
		}
		else
		{
			MESSAGE_ERROR("", "", "can't convert(" + srcText + ") to float");
		}
	}

	MESSAGE_DEBUG("", "", "finish ( result length = " + to_string(result.length()) + ")");

	return	result;
}

string CheckHTTPParam_Date(string srcText)
{
	auto	result = ""s;

	MESSAGE_DEBUG("", "", "start param(" + srcText + ")");

	if(srcText.length())
	{
		regex	r("^([[:digit:]]{1,2})\\/([[:digit:]]{1,2})\\/([[:digit:]]{2,4})$");
		smatch	sm;

		trim(srcText);

		if(regex_match(srcText, sm, r))
		{
			auto	date	= stoi(sm[1]);
			auto	month	= stoi(sm[2]);
			auto	year	= stoi(sm[3]);

			if(year < 100) year += 2000;

			if((1 <= date) && (date <= 31))
			{
				if((1 <= month) && (month <= 12))
				{
					if((1900 <= year) && (year <= 2100))
					{
						auto tm_obj = GetTMObject(srcText);

						mktime(&tm_obj);

						if((date == tm_obj.tm_mday) && (month == (tm_obj.tm_mon + 1)) && (year == (tm_obj.tm_year + 1900)))
						{
							result = srcText;
						}
						else
						{
							MESSAGE_ERROR("", "", "wrong date (" + srcText + " -> " + to_string(date) + "/" + to_string(month) + "/" + to_string(year) + " vs " + to_string(tm_obj.tm_mday) + "/" + to_string(tm_obj.tm_mon + 1) + "/" + to_string(tm_obj.tm_year + 1900) + ")");
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "year (" + to_string(year) + ") is out of range");
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "month (" + to_string(month) + ") is out of range");
				}
			}
			else
			{
				MESSAGE_ERROR("", "", "date (" + to_string(date) + ") is out of range");
			}
		}
		else
		{
			MESSAGE_ERROR("", "", "(" + srcText + ") doesn't match date regex");
		}
	}

	MESSAGE_DEBUG("", "", "finish (result length = " + result + ")");

	return	result;
}

string CheckHTTPParam_Email(const string &srcText)
{
	auto		result = ""s;

    regex       positionRegex(".*([+-][[:digit:]]+\\.[[:digit:]]+)([+-][[:digit:]]+\\.[[:digit:]]+)([+-][[:digit:]]+\\.[[:digit:]]+).*");
    smatch      matchResult;

	MESSAGE_DEBUG("", "", "start param(" + srcText + ")");

	result = ConvertTextToHTML(srcText);

	if(regex_match(srcText, regex("^[._[:alnum:]]+@[[:alnum:]][\\-.[:alnum:]]*[[:alnum:]]\\.[[:alnum:]]{2,5}$") ))
    {
    	if(result.length() > 128)
    	{
    		result = "";
    		MESSAGE_DEBUG("", "", "e-mail too long >128 symbols");
    	}
    	else
    	{
	    	// --- regex matched
    	}
    }
    else
    {
		MESSAGE_DEBUG("", "", "email doesn't match regex " + result);

    	result = "";
    }

	MESSAGE_DEBUG("", "", "finish ( result length = " + to_string(result.length()) + ")");

	return	result;
}

double GetSecondsSinceY2k()
{
	time_t	  timer;
	// struct tm   y2k = {0};
	double		seconds;
	double		secondsY2kUTC = 946684800;

	// y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	// y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	timer = time(NULL);  /* get current time; same as: timer = time(NULL)  */
	// seconds = difftime(timer,mktime(&y2k));
	seconds = timer - secondsY2kUTC;

	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetSecondsSinceY2k(): end (seconds since Y2k" << std::to_string(seconds) << ")";
		log.Write(DEBUG, ost.str());
	}

	return seconds;
}

string GetLocalFormattedTimestamp()
{
	time_t	  now_t;
	struct tm   *local_tm;
	char		buffer[80];
	auto		result = ""s;

	now_t = time(NULL);
	local_tm = localtime(&now_t);
	if(local_tm == NULL)
	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetLocalFormatedTimestamp(): ERROR in running localtime(&t)";
		log.Write(ERROR, ost.str());
	}

	memset(buffer, 0, 80);
	strftime(buffer,80,"%Y-%m-%02d %T", local_tm);
	result = buffer;


	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetLocalFormatedTimestamp(): end (" << result << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;

}

// --- input format: "2015-06-10 00:00:00"
// --- return: number of second difference from now
double GetTimeDifferenceFromNow(const string timeAgo)
{
	time_t	  now_t, checked_t;
	// char		utc_str[100];
	struct tm   *local_tm, check_tm;
	ostringstream	ost;
/*
	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): start";
		log.Write(DEBUG, ost.str());
	}
*/

	now_t = time(NULL);
	local_tm = localtime(&now_t);
	if(local_tm == NULL)
	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetTimeDifferenceFromNow(now): ERROR in running localtime(&t)";
		log.Write(ERROR, ost.str());
	}
/*
	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetTimeDifferenceFromNow(now): now_t = " << now_t;
		log.Write(DEBUG, ost.str());
	}
*/

	// now2_t = time(NULL);
	// check_tm = localtime(&now2_t);
	sscanf(timeAgo.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &check_tm.tm_year, &check_tm.tm_mon, &check_tm.tm_mday, &check_tm.tm_hour, &check_tm.tm_min, &check_tm.tm_sec);
	check_tm.tm_year -= 1900;
	check_tm.tm_mon -= 1;
	check_tm.tm_isdst = 0;	// --- "Summer time" is OFF. Be carefull with it.
							// --- Russia is not using Daylight saving
							// --- USA is

	// --- For testing purposes try to use the same Daylight saving as in local clock
	// --- Test starts on 9/11. If there is no side effect, comment-out previous line.
	if(local_tm) check_tm.tm_isdst = local_tm->tm_isdst;	// --- "Summer time" is the same as local clock.

/*
	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): checked year = " << check_tm.tm_year << " checked month = " << check_tm.tm_mon << " checked day = " << check_tm.tm_mday << " checked hour = " << check_tm.tm_hour << " checked min = " << check_tm.tm_min << " checked sec = " << check_tm.tm_sec;
		log.Write(DEBUG, ost.str());
	}
*/
	checked_t = mktime(&check_tm);

	{
		CLog	log;
		ostringstream	ost;
		char	buffer[80];

		ost.str("");
		strftime(buffer,80,"check_tm: date regenerated: %02d-%b-%Y %T %Z  %I:%M%p.", &check_tm);
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): " << buffer << "";
		log.Write(DEBUG, ost.str());

		memset(buffer, 0, 80);
		strftime(buffer,80,"local_tm: date regenerated: %02d-%b-%Y %T %Z  %I:%M%p.", local_tm);
		ost.str("");
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): " << buffer << "";
		log.Write(DEBUG, ost.str());

		ost.str("");
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): difftime( now_t=" << now_t << ", checked_t=" << checked_t << ")";
		log.Write(DEBUG, ost.str());

		ost.str("");
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): end (difference = " << difftime(now_t, checked_t) << ")";
		log.Write(DEBUG, ost.str());
	}

	return difftime(now_t, checked_t);
}

string GetMinutesDeclension(const int value)
{
	map<int, string> 	mapDeclension = {
		{1, "минуту"},
		{2, "минуты"},
		{3, "минут"}
	};

	auto				result = ""s;

	if(value % 10 == 0) 						{ result = mapDeclension.at(3); };
	if(value % 10 == 1) 						{ result = mapDeclension.at(1); };
	if((value % 10 >= 2) and (value % 10 <= 4))	{ result = mapDeclension.at(2); };
	if((value % 10 >= 5) and (value % 10 <= 9))	{ result = mapDeclension.at(3); };
	if((value >= 5) and (value <= 20))			{ result = mapDeclension.at(3); };
	return result;
}


string GetHoursDeclension(const int value)
{
	map<int, string> 	mapDeclension = {
		{1, "час"},
		{2, "часа"},
		{3, "часов"}
	};
	auto				result = ""s;

	if(value % 10 == 0) 						{ result = mapDeclension.at(3); };
	if(value % 10 == 1) 						{ result = mapDeclension.at(1); };
	if((value % 10 >= 2) and (value % 10 <= 4))	{ result = mapDeclension.at(2); };
	if((value % 10 >= 5) and (value % 10 <= 9))	{ result = mapDeclension.at(3); };
	if((value >= 5) and (value <= 20))			{ result = mapDeclension.at(3); };
	return result;
}

string GetDaysDeclension(const int value)
{
	map<int, string> 	mapDeclension = {
		{1, "день"},
		{2, "дня"},
		{3, "дней"}
	};
	auto				result = ""s;

	if(value % 10 == 0) 						{ result = mapDeclension.at(3); };
	if(value % 10 == 1) 						{ result = mapDeclension.at(1); };
	if((value % 10 >= 2) and (value % 10 <= 4))	{ result = mapDeclension.at(2); };
	if((value % 10 >= 5) and (value % 10 <= 9))	{ result = mapDeclension.at(3); };
	if((value >= 5) and (value <= 20))			{ result = mapDeclension.at(3); };
	return result;
}

string GetMonthsDeclension(const int value)
{
	map<int, string> 	mapDeclension = {
		{1, "месяц"},
		{2, "месяца"},
		{3, "месяцев"}
	};
	auto				result = ""s;

	if(value % 10 == 0) 						{ result = mapDeclension.at(3); };
	if(value % 10 == 1) 						{ result = mapDeclension.at(1); };
	if((value % 10 >= 2) and (value % 10 <= 4))	{ result = mapDeclension.at(2); };
	if((value % 10 >= 5) and (value % 10 <= 9))	{ result = mapDeclension.at(3); };
	if((value >= 5) and (value <= 20))			{ result = mapDeclension.at(3); };
	return result;
}

string GetYearsDeclension(const int value)
{
	map<int, string> 	mapDeclension = {
		{1, "год"},
		{2, "года"},
		{3, "лет"}
	};
	auto				result = ""s;

	if(value % 10 == 0) 						{ result = mapDeclension.at(3); };
	if(value % 10 == 1) 						{ result = mapDeclension.at(1); };
	if((value % 10 >= 2) and (value % 10 <= 4))	{ result = mapDeclension.at(2); };
	if((value % 10 >= 5) and (value % 10 <= 9))	{ result = mapDeclension.at(3); };
	if((value >= 5) and (value <= 20))			{ result = mapDeclension.at(3); };
	return result;
}

// --- input format: "2015-06-10 00:00:00"
// --- return: human readable format
string GetHumanReadableTimeDifferenceFromNow (const string timeAgo)
{
	double			seconds = GetTimeDifferenceFromNow(timeAgo);
	double			minutes = seconds / 60;
	double			hours = minutes / 60;
	double			days = hours / 24;
	double			months = days / 30;
	double			years = months / 12;
	ostringstream	ost;

	ost.str("");
	if(years >= 1)
	{
		ost << (int)years << " " << GetYearsDeclension(years);
	}
	else if(months >= 1)
	{
		ost << (int)months << " " << GetMonthsDeclension(months);
	}
	else if(days >= 1)
	{
		ost << (int)days << " " << GetDaysDeclension(days);
	}
	else if(hours >= 1)
	{
		ost << (int)hours << " " << GetHoursDeclension(hours);
	}
	else
	{
		ost << (int)minutes << " " << GetMinutesDeclension(minutes);
	}

	ost << " назад.";


	// --- commented to reduce logs flooding
	{
		CLog	log;
		ostringstream	ost1;

		ost1.str("");
		ost1 << "string GetHumanReadableTimeDifferenceFromNow (" << timeAgo << "): sec difference (" << seconds << ") human format (" << ost.str() << ")" ;
		log.Write(DEBUG, ost1.str());
	}


	return ost.str();
}

string SymbolReplace(const string where, const string src, const string dst)
{
	auto	  result(where);
	auto	  pos = result.find(src);

	while(pos != string::npos)
	{
		result.replace(pos, src.length(), dst);
		pos = result.find(src, pos + 1);
	}

	return result;
}

auto SymbolReplace_KeepDigitsOnly(const string &where) -> string
{
	auto	  	result = where;
	auto		i = 0u;

	while(i < result.length())
	{
		auto	currSymb = result.at(i);

		if((currSymb >= static_cast<char>('0')) && (currSymb <= static_cast<char>('9')))
			i++;
		else
			result.replace(i, 1, "");
	}

	return result;
}

bool CheckUserEmailExisting(string userNameToCheck, CMysql *db) {
	CUser		user;

	MESSAGE_DEBUG("", "", "start");

	user.SetDB(db);
	user.SetLogin(userNameToCheck);
	user.SetEmail(userNameToCheck);

	if(user.isLoginExist() or user.isEmailDuplicate()) 
	{
		MESSAGE_DEBUG("", "", "login or email already registered");

		return true;
	}
	else 
	{
		MESSAGE_DEBUG("", "", "login or email not yet exists");

		return false;
	}
}

string UniqueUserIDInUserIDLine(string userIDLine) //-> decltype(static_cast<string>("123"))
{
	list<long int>	listUserID;
	auto			result = ""s;
	std::size_t		prevPointer {0}, nextPointer;

	MESSAGE_DEBUG("", "", "start (" + userIDLine + ")");

	do
	{
		nextPointer = userIDLine.find(",", prevPointer);
		if(nextPointer == string::npos)
		{
			listUserID.push_back(atol(userIDLine.substr(prevPointer).c_str()));
		}
		else
		{
			listUserID.push_back(atol(userIDLine.substr(prevPointer, nextPointer - prevPointer).c_str()));
		}
		prevPointer = nextPointer + 1;
	} while( (nextPointer != string::npos) );

	listUserID.sort();
	listUserID.unique();
	for(auto it: listUserID)
	{
		result += (result.length() ? "," : "") + to_string(it);
	}

	{
		MESSAGE_DEBUG("", "", "finish (result " + result + ")");
	}

	return result;
}

bool	isFilenameImage(const string &filename)
{
	auto	result = false;
	regex   e1("[.](gif|jpg|jpeg|png)$", regex_constants::icase);
	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "(" + filename + ")[" + to_string(__LINE__) + string("]: start" ));
	}

	result = regex_search(filename, e1);

	{
		MESSAGE_DEBUG("", "", "finish (result: " + (result ? "true" : "false") + ")");
	}
	return  result;
}

bool	isFilenameVideo(const string &filename)
{
	auto	result = false;
	regex   e1("[.](mov|avi|mp4|webm)$", regex_constants::icase);
	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "(" + filename + ")[" + to_string(__LINE__) + string("]: start" ));
	}

	result = regex_search(filename, e1);

	{
		MESSAGE_DEBUG("", "", "finish (result: " + (result ? "true" : "false") + ")");
	}
	return  result;
}

string GetFileExtension(const string &filename)
{
	MESSAGE_DEBUG("", "", "start (" + filename + ")");

	auto	result = ""s;
	auto	dot_pos = filename.find(".");

	if(dot_pos != string::npos) result = filename.substr(dot_pos + 1);

	MESSAGE_DEBUG("", "", "finish (" + result + ")");

	return result;
}

off_t getFileSize(const std::string& name) 
{
	off_t			result = -1;
	struct stat		buffer;

	if(stat(name.c_str(), &buffer) == -1)
	{
		MESSAGE_ERROR("", "", "fail to get file size (" + name + "), probably file doesn't exists");
	}
	else
	{
		result = buffer.st_size;
	}

	return result;
}

// --- extract all @[[:digit:]] patterns form srcMessage
vector<string> GetUserTagsFromText(string srcMessage)
{
	vector<string>  result;
	regex		   exp1("@([[:digit:]]+)");

	MESSAGE_DEBUG("", "", "start");

	regex_token_iterator<string::iterator>   rItr(srcMessage.begin(), srcMessage.end(), exp1, 1);
	regex_token_iterator<string::iterator>   rItrEnd;
	while(rItr != rItrEnd)
	{
		result.push_back(rItr->str());
		++rItr;
	}

	{
		CLog	log;
		log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + string("]: end (result length: ") + to_string(result.size()) + string(")"));
	}

	return result;
}

string	AutodetectSexByName(string name, CMysql *db)
{
	string		result = "";

	MESSAGE_DEBUG("", "", "start");

	if(db->Query("SELECT * FROM `name_sex` WHERE `name`=\"" + name + "\";"))
		result = db->Get(0, "sex");

	{
		MESSAGE_DEBUG("", "", "finish (result = " + result + ")");
	}

	return result;
}

string GetGeoLocalityIDByCityAndRegion(string regionName, string cityName, CMysql *db)
{
	ostringstream   ost;
	string		 	result = "";
	string			regionID = "", cityID = "";

	MESSAGE_DEBUG("", "", "start");

	if(regionName.length())
	{
		if(db->Query("SELECT `id` FROM `geo_region` WHERE `title`=\"" + regionName + "\";"))
		{
			regionID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;

			MESSAGE_DEBUG("", "", "region[" + regionName + "] not found. Creating new one.");

			tmp = db->InsertQuery("INSERT INTO `geo_region` SET `geo_country_id`=\"0\", `title`=\"" + regionName + "\";");
			if(tmp)
				regionID = to_string(tmp);
			else
			{
				CLog			log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: insert into geo_region_db");
			}
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "regionName is empty");
	}

	if(cityName.length())
	{
		if(db->Query("SELECT `id` FROM `geo_locality` WHERE `title`=\"" + cityName + "\" " + (regionID.length() ? " AND `geo_region_id`=\"" + regionID + "\" " : "") + ";"))
		{
			cityID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;

			MESSAGE_DEBUG("", "", "region[" + cityName + "] not found. Creating new one.");

			tmp = db->InsertQuery("INSERT INTO `geo_locality` SET " + (regionID.length() ? "`geo_region_id`=\"" + regionID + "\"," : "") + " `title`=\"" + cityName + "\";");
			if(tmp)
				cityID = to_string(tmp);
			else
			{
				CLog			log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: insert into geo_region_db");
			}
		}
		result = cityID;
	}
	else
	{
		MESSAGE_DEBUG("", "", "cityName is empty");
	}

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");

	return result;
}

bool AllowMessageInNewsFeed(CUser *me, const string messageOwnerID, const string messageAccessRights, vector<string> *messageFriendList)
{

	MESSAGE_DEBUG("", "", "parameters (user [" + me->GetID() + "], messageOwnerID [" + messageOwnerID + "], messageAccessRights [" + messageAccessRights + "]): start");

	// --- messages belongs to yourself must be shown unconditionally
	// --- must be checked before message access private
	if(me->GetID() == messageOwnerID) return true;

	if(messageAccessRights == "public") return true;
	if(messageAccessRights == "private") return false;

	// --- require to check friendship list;
	if(messageAccessRights == "friends")
	{
		for(auto it = messageFriendList->begin(); it != messageFriendList->end(); ++it)
		{
			if(*it == messageOwnerID) return true;
		}

		return false;
	}

	return true;
}

// --- rate-limit on sessid persistence
// --- input: REMOTE_ADDR
// --- output: true, if rate-limit required
// ---		 false, if rate-limit not required
bool isPersistenceRateLimited(string REMOTE_ADDR, CMysql *db)
{
	auto 			maxTime = BRUTEFORCE_TIME_INTERVAL, maxAttempts = BRUTEFORCE_ATTEMPTS;
	auto			rateLimitID = ""s;
	auto			affected = 0, attempts = 0;
	auto			result = false;

	MESSAGE_DEBUG("", "", "start (REMOTE_ADDR " + REMOTE_ADDR + ")");

	// --- cleanup rate-limit table
	db->Query("delete from `sessions_persistence_ratelimit` where `eventTimestamp` < (NOW() - interval "s + to_string(maxTime) + " second);");

	affected = db->Query("select `id`, `attempts` from `sessions_persistence_ratelimit` where `ip`='"s + REMOTE_ADDR + "';");
	if(affected)
	{
		MESSAGE_DEBUG("", "", "REMOTE_ADDR is in rate-limit table");

		rateLimitID = db->Get(0, "id");
		attempts = stoi(db->Get(0, "attempts"));

		db->Query("update `sessions_persistence_ratelimit` set `attempts`=`attempts`+1 where `id`='" + rateLimitID + "';");

		if(attempts > maxAttempts)
		{
			MESSAGE_ERROR("", "", "REMOTE_ADDR has tried " + to_string(attempts) + " times during the last " + to_string(maxTime) + "sec. Needed to be rate-limited.")

			result = true;
		}
		else
		{
			MESSAGE_DEBUG("", "", "REMOTE_ADDR has tried " + to_string(attempts) + " times during the last " + to_string(maxTime) + "sec. No need to rate-limit.")
			
			result = false;
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "REMOTE_ADDR not in rate-limit table.")

		db->Query("insert into `sessions_persistence_ratelimit` set `attempts`='1', `ip`='" + REMOTE_ADDR + "', `eventTimestamp`=NOW();");

		result = false;
	}

	MESSAGE_DEBUG("", "", "finish. "s + (result ? "rate-limit" : "no need rate-limit"))

	return result;
}

void CopyFile(const string src, const string dst)
{
	clock_t start, end;

	MESSAGE_DEBUG("", "", "start (" + src + ", " + dst + ")");

	start = clock();

	ifstream source(src.c_str(), ios::binary);
	ofstream dest(dst.c_str(), ios::binary);

	dest << source.rdbuf();

	source.close();
	dest.close();


	end = clock();

	MESSAGE_DEBUG("", "", "finish (time of file copying is " + to_string((end - start) / CLOCKS_PER_SEC) + " second)");
}

// --- admin function
string GetCompanyDuplicates(CMysql *db)
{
	// --- map<companyName, companyID>
	map<string, string>		companyMap;

	// --- map<companyID, companyName>
	map<string, string>		duplicatesMap;
	ostringstream			ost, ostResult;

	{
		CLog	log;
		ostringstream	ostTemp;

		ostTemp.str("");
		ostTemp << "GetCompanyDuplicates: start ";
		log.Write(DEBUG, ostTemp.str());
	}

	ostResult.str("");
	ost.str("");
	ost << "select * from `company`;";
	for(auto i = 0; i < db->Query(ost.str()); i++)
	{
		string		companyToTestName, companyToTestID;

		companyToTestID = db->Get(i, "id");
		companyToTestName = RemoveAllNonAlphabetSymbols(db->Get(i, "name"));

		auto it = companyMap.find(companyToTestName);
		if(it != companyMap.end())
		{
			// --- duplicate found
			duplicatesMap[companyToTestID] = companyToTestName;
			duplicatesMap[it->second] = it->first;
		}
		companyMap[companyToTestName] = companyToTestID;
	}

	for (const auto& it : duplicatesMap)
	{
		if(ostResult.str().length()) ostResult << ",";
		ost.str("");
		ost << "select * from `company` where `id`='" << it.first << "';";
		db->Query(ost.str());
		ostResult << "{\"id\":\"" << it.first << "\", \"companyName\":\"" << db->Get(0, "name") << "\"";

		ost.str("");
		ost << "select count(*) as `number_of_users` from `users_company` where `company_id`='" << it.first << "';";
		db->Query(ost.str());
		ostResult << ", \"usersInCompany\":\"" << db->Get(0, "number_of_users") << "\"}";
	}

	{
		CLog	log;
		ostringstream	ostTemp;

		ostTemp.str("");
		ostTemp << "GetCompanyDuplicates: end (number of duplicates: " << duplicatesMap.size() << ")";
		log.Write(DEBUG, ostTemp.str());
	}

	return ostResult.str();
}

// --- admin function
string GetPicturesWithEmptySet(CMysql *db)
{
	ostringstream		   ost, ostResult;
	int					 affected;

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithEmptySet: start ";
		log.Write(DEBUG, ostTemp.str());
	}

	ostResult.str("");
	ost.str("");
	ost << "SELECT * FROM `feed_images` where `setID`='0';";
	affected = db->Query(ost.str());
	for(auto i = 0; i < affected; i++)
	{
		if(ostResult.str().length()) ostResult << ",";
		ostResult << "{\"id\":\"" << db->Get(i, "id") << "\",\"srcType\":\"" << db->Get(i, "srcType") << "\",\"userID\":\"" << db->Get(i, "userID") << "\",\"folder\":\"" << db->Get(i, "folder") << "\",\"filename\":\"" << db->Get(i, "filename") << "\"}";
	}

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithEmptySet: end (# of lost pictures: " << affected << ")";
		log.Write(DEBUG, ostTemp.str());
	}

	return ostResult.str();
}

// --- admin function
string GetPicturesWithUnknownMessage(CMysql *db)
{
	ostringstream					   ost, ostResult;
	int								 affected, lostCount = 0;
	unordered_set<unsigned long>		allImageSets;
	unordered_set<unsigned long>		lostImageSets;

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithUnknownMessage: start ";
		log.Write(DEBUG, ostTemp.str());
	}

	ostResult.str("");
	ost.str("");
	ost << "SELECT `setID` FROM `feed_images`;";
	affected = db->Query(ost.str());
	for(auto i = 0; i < affected; i++)
	{
		allImageSets.insert(stol(db->Get(i, "setID")));
	}

	for(const unsigned long& id: allImageSets)
	{
		ost.str("");
		ost << "select count(*) as count from `feed_message` where `imageSetID`=\"" << id << "\";";
		db->Query(ost.str());
		if(!stoi(db->Get(0, "count")))
		{
			lostImageSets.insert(id);
		}
	}

	ostResult.str("");
	for(const unsigned long& id: lostImageSets)
	{
		ost.str("");
		ost << "select * from `feed_images` where `setID`=\"" << id << "\";";
		for(auto i = 0; i < db->Query(ost.str()); i++, lostCount++)
		{
			if(ostResult.str().length()) ostResult << ",";
			ostResult << "{\"id\":\"" << db->Get(i, "id") << "\",\"srcType\":\"" << db->Get(i, "srcType") << "\",\"userID\":\"" << db->Get(i, "userID") << "\",\"setID\":\"" << db->Get(i, "setID") << "\",\"folder\":\"" << db->Get(i, "folder") << "\",\"filename\":\"" << db->Get(i, "filename") << "\"}";
		}
	}

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithUnknownMessage: end (# of lost pictures: " << affected << ")";
		log.Write(DEBUG, ostTemp.str());
	}

	return ostResult.str();
}

// --- admin function
string GetPicturesWithUnknownUser(CMysql *db)
{
	ostringstream					   		ost, ostResult;
	int								 		affected, lostCount = 0;
	unordered_set<string>					allImageOwners;
	unordered_set<unsigned long>			lostImages;

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithUnknownUser: start ";
		log.Write(DEBUG, ostTemp.str());
	}

	ostResult.str("");
	ost.str("");
	ost << "SELECT `srcType`,`userID` FROM `feed_images`;";
	affected = db->Query(ost.str());
	for(auto i = 0; i < affected; i++)
		allImageOwners.insert(string(db->Get(i, "srcType")) + string(db->Get(i, "userID")));

	for(auto &item: allImageOwners)
	{
		string				tableName = "";
		string				id;
		size_t				pos;

		if((pos = item.find("company")) != string::npos)
		{
			tableName = "company";
			id = item.substr(string("company").length(), item.length() - string("company").length());
		}
		else if((pos = item.find("user")) != string::npos)
		{
			tableName = "users";
			id = item.substr(string("user").length(), item.length() - string("user").length());
		}

		if(tableName.length() && !db->Query("select `id` from `" + tableName + "` where `id`=\"" + id + "\";"))
			lostImages.insert(stol(id));
	}

	ostResult.str("");
	for(const unsigned long& id: lostImages)
	{
		ost.str("");
		ost << "select * from `feed_images` where `userID`=\"" << id << "\";";
		for(auto i = 0; i < db->Query(ost.str()); i++, lostCount++)
		{
			if(ostResult.str().length()) ostResult << ",";
			ostResult << "{\"id\":\"" << db->Get(i, "id") << "\",\"srcType\":\"" << db->Get(i, "srcType") << "\",\"userID\":\"" << db->Get(i, "userID") << "\",\"setID\":\"" << db->Get(i, "setID") << "\",\"folder\":\"" << db->Get(i, "folder") << "\",\"filename\":\"" << db->Get(i, "filename") << "\"}";
		}
	}

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithUnknownUser: end (# of lost pictures: " << affected << ")";
		log.Write(DEBUG, ostTemp.str());
	}

	return ostResult.str();
}

// --- admin function
string GetRecommendationAdverse(CMysql *db)
{
	ostringstream					   ost, ostResult, dictionaryStatement;
	int								 affected;

	dictionaryStatement.str("");
	ostResult.str("");
	ost.str("");
	ost << "SELECT * FROM `dictionary_adverse`;";
	affected = db->Query(ost.str());
	if(affected)
	{
		for(auto i = 0; i < affected; i++)
		{
			if(i) dictionaryStatement << " or ";
			dictionaryStatement << "(`title` like \"%" << db->Get(i, "word") << "%\")";
		}
	}

	ost.str("");
	ost << "select * from `users_recommendation` where `state` = 'unknown' and (" << dictionaryStatement.str() << ");";
	affected = db->Query(ost.str());
	if(affected)
	{
		for(auto i = 0; i < affected; i++)
		{
			if(i) ostResult << ",";
			ostResult << "{";
			ostResult << "\"recommendationId\":\"" << db->Get(i, "id") <<"\",";
			ostResult << "\"recommendationRecommended_userID\":\"" << db->Get(i, "recommended_userID") <<"\",";
			ostResult << "\"recommendationRecommending_userID\":\"" << db->Get(i, "recommending_userID") <<"\",";
			ostResult << "\"recommendationTitle\":\"" << db->Get(i, "title") <<"\",";
			ostResult << "\"recommendationEventTimestamp\":\"" << db->Get(i, "eventTimestamp") <<"\",";
			ostResult << "\"recommendationState\":\"" << db->Get(i, "state") <<"\"";
			ostResult << "}";
		}
	}

	return ostResult.str();
}

// --- User notification part
string GetUserAvatarByUserID(string userID, CMysql *db)
{
	ostringstream   ost;
	string		  userAvatar = "";

	ost.str("");
	ost << "select * from `users_avatars` where `userid`='" << userID << "' and `isActive`='1';";
	if(db->Query(ost.str()))
	{
		ost.str("");
		ost << "/images/avatars/avatars" << db->Get(0, "folder") << "/" << db->Get(0, "filename");
		userAvatar = ost.str();
	}

	return userAvatar;
}

// --- function removes message image from FileSystems and cleanup DB
// --- as input require SWL WHERE clause (because of using SELECT and DELETE statements)
// --- input params:
// --- 1) SQL WHERE statement
// --- 2) db reference
void	RemoveMessageImages(string sqlWhereStatement, CMysql *db)
{
	int			 affected;
	ostringstream   ost;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: start (sqlWhereStatement: " + sqlWhereStatement + ")");
	}

	ost.str("");
	ost << "select * from `feed_images` where " << sqlWhereStatement;
	affected = db->Query(ost.str());
	if(affected)
	{
		for(auto i = 0; i < affected; i++)
		{
			string  filename = "";
			string  mediaType = db->Get(i, "mediaType");

			if(mediaType == "image" || mediaType == "video")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: file must be deleted [" + filename + "]");
				}

				if(mediaType == "image") filename = IMAGE_FEED_DIRECTORY;
				if(mediaType == "video") filename = VIDEO_FEED_DIRECTORY;

				filename +=  "/";
				filename +=  db->Get(i, "folder");
				filename +=  "/";
				filename +=  db->Get(i, "filename");


				if(isFileExists(filename))
				{
					unlink(filename.c_str());
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) +  "]:ERROR: file is not exists  [filename=" + filename + "]");
				}
			}
			else
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: mediaType[" + mediaType + "] doesn't have local file");
				}

			}

		}
		// --- cleanup DB with images pre-populated for posted message
		ost.str("");
		ost << "delete from `feed_images` where " << sqlWhereStatement;
		db->Query(ost.str());
	}

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: finish");
	}
}

// --- function removes message image from FileSystems and cleanup DB
// --- as input require SWL WHERE clause (because of using SELECT and DELETE statements)
// --- input params:
// --- 1) SQL WHERE statement
// --- 2) db reference
void	RemoveBookCover(string sqlWhereStatement, CMysql *db)
{
	int			 affected;
	ostringstream   ost;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: start (sqlWhereStatement: " + sqlWhereStatement + ")");
	}

	ost.str("");
	ost << "select * from `book` where " << sqlWhereStatement;
	affected = db->Query(ost.str());
	if(affected)
	{
		for(auto i = 0; i < affected; i++)
		{
			string  filename = "";
			string  mediaType = "image";

			if(mediaType == "image" || mediaType == "video")
			{

				if(mediaType == "image") filename = IMAGE_BOOKS_DIRECTORY;

				filename +=  "/";
				filename +=  db->Get(i, "coverPhotoFolder");
				filename +=  "/";
				filename +=  db->Get(i, "coverPhotoFilename");

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: file must be deleted [" + filename + "]");
				}

				if(isFileExists(filename))
				{
					unlink(filename.c_str());
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) +  "]:ERROR: file doesn't exists  [filename=" + filename + "]");
				}
			}
			else
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: mediaType[" + mediaType + "] doesn't have local file");
				}

			}

		}
		// --- cleanup DB with images pre-populated for posted message
		ost.str("");
		ost << "UPDATE `book` SET `coverPhotoFolder`=\"\",`coverPhotoFilename`=\"\" where " << sqlWhereStatement;
		db->Query(ost.str());
	}

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: finish");
	}
}

// --- function removes specified image from FileSystems and cleanup DB
// --- input params:
// --- 1) id
// --- 2) type
// --- 3) db reference
bool	RemoveSpecifiedCover(string itemID, string itemType, CMysql *db)
{
	int			 affected;
	bool			result = true;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: start (itemID = " + itemID + ", itemType = " + itemType + ")");
	}

	affected = db->Query(GetSpecificData_SelectQueryItemByID(itemID, itemType));
	if(affected)
	{
		for(auto i = 0; i < affected; i++)
		{
			string  filename = "";
			string  mediaType = "image";

			if(mediaType == "image" || mediaType == "video")
			{

				if(mediaType == "image") filename = GetSpecificData_GetBaseDirectory(itemType);

				filename +=  "/";
				filename +=  db->Get(i, GetSpecificData_GetDBCoverPhotoFolderString(itemType).c_str());
				filename +=  "/";
				filename +=  db->Get(i, GetSpecificData_GetDBCoverPhotoFilenameString(itemType).c_str());

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: file must be deleted [" + filename + "]");
				}

				if(isFileExists(filename))
				{
					unlink(filename.c_str());
				}
				else
				{
					result = false;
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) +  "]:ERROR: file doesn't exists  [filename=" + filename + "]");
					}
				}
			}
			else
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: mediaType[" + mediaType + "] doesn't have local file");
				}

			}

		}
		// --- cleanup DB with images pre-populated for posted message
		db->Query(GetSpecificData_UpdateQueryItemByID(itemID, itemType, "", ""));
	}

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: finish (result = " + (result ? "true" : "false") + ")");
	}

	return result;
}


bool is_base64(BYTE c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

string base64_encode(BYTE const* buf, unsigned int bufLen)
{
  std::string ret;
  int i = 0;
  int j = 0;
  BYTE char_array_3[3];
  BYTE char_array_4[4];

  while (bufLen--) {
	char_array_3[i++] = *(buf++);
	if (i == 3) {
	  char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
	  char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
	  char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
	  char_array_4[3] = char_array_3[2] & 0x3f;

	  for(i = 0; (i <4) ; i++)
		ret += base64_chars[char_array_4[i]];
	  i = 0;
	}
  }

  if (i)
  {
	for(j = i; j < 3; j++)
	  char_array_3[j] = '\0';

	char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
	char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
	char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
	char_array_4[3] = char_array_3[2] & 0x3f;

	for (j = 0; (j < i + 1); j++)
	  ret += base64_chars[char_array_4[j]];

	while((i++ < 3))
	  ret += '=';
  }

  return ret;
}

string base64_decode(std::string const& encoded_string)
{
  size_t in_len = encoded_string.size();
  size_t i = 0;
  size_t j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
	char_array_4[i++] = encoded_string[in_]; in_++;
	if (i ==4) {
	  for (i = 0; i <4; i++)
		char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

	  char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
	  char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
	  char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	  for (i = 0; (i < 3); i++)
		ret += char_array_3[i];
	  i = 0;
	}
  }

  if (i) {
	for (j = i; j <4; j++)
	  char_array_4[j] = 0;

	for (j = 0; j <4; j++)
	  char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

	char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
	char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
	char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

bool RedirStdoutToFile(string fname)
{
	bool		result = true;
	FILE		*fRes;

	fRes = freopen(fname.c_str(), "w", stdout);
	if(!fRes)
	{
		result = false;
		{
			CLog	log;
			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: redirect stderr to " + fname);
		}
	}

	return  result;
}

bool RedirStderrToFile(string fname)
{
	bool		result = true;
	FILE		*fRes;

	fRes = freopen(fname.c_str(), "w", stderr);
	if(!fRes)
	{
		result = false;
		{
			CLog	log;
			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: redirect stderr to " + fname);
		}
	}

	return  result;
}

auto isAllowed_NoSession_Action(string action) -> bool
{
	auto			result = false;
	vector<string>	allowed_nosession_actions = ALLOWED_NO_SESSION_ACTION;

	MESSAGE_DEBUG("", "", "start (action " + action + ")");

	for(auto &allowed_nosession_action : allowed_nosession_actions)
	{
		if(action == allowed_nosession_action)
		{
			result = true;
			break;
		}
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

pair<struct tm, struct tm> GetFirstAndLastMonthDaysByDate(const struct tm &_date)
{
	struct tm end_of_mon, start_of_mon;

	MESSAGE_DEBUG("", "", "start(" + to_string(_date.tm_mday) + "/" + to_string(_date.tm_mon + 1) + "/" + to_string(_date.tm_year + 1900) + ")");

	start_of_mon.tm_sec		= 0;   // seconds of minutes from 0 to 61
	start_of_mon.tm_min		= 0;   // minutes of hour from 0 to 59
	start_of_mon.tm_hour	= 0;  // hours of day from 0 to 24
	start_of_mon.tm_mday	= 0;  // day of month from 1 to 31
	start_of_mon.tm_mon		= 0;   // month of year from 0 to 11
	start_of_mon.tm_year	= 0;  // year since 1900
	start_of_mon.tm_wday	= 0;  // days since sunday
	start_of_mon.tm_yday	= 0;  // days since January 1st
	start_of_mon.tm_isdst	= 0; // hours of daylight savings time

	end_of_mon.tm_sec		= 0;   // seconds of minutes from 0 to 61
	end_of_mon.tm_min		= 0;   // minutes of hour from 0 to 59
	end_of_mon.tm_hour		= 0;  // hours of day from 0 to 24
	end_of_mon.tm_mday		= 0;  // day of month from 1 to 31
	end_of_mon.tm_mon		= 0;   // month of year from 0 to 11
	end_of_mon.tm_year		= 0;  // year since 1900
	end_of_mon.tm_wday		= 0;  // days since sunday
	end_of_mon.tm_yday		= 0;  // days since January 1st
	end_of_mon.tm_isdst		= 0; // hours of daylight savings time

	start_of_mon.tm_year	= _date.tm_year;
	start_of_mon.tm_mon		= _date.tm_mon;
	start_of_mon.tm_mday	= 1;

	end_of_mon.tm_year		= _date.tm_year;
	end_of_mon.tm_mon		= _date.tm_mon + 1;
	end_of_mon.tm_mday		= 0;

	mktime(&start_of_mon);
	mktime(&end_of_mon);

	MESSAGE_DEBUG("", "", "finish (" + to_string(start_of_mon.tm_mday) + "/" + to_string(start_of_mon.tm_mon + 1) + "/" + to_string(start_of_mon.tm_year + 1900) + " - " + to_string(end_of_mon.tm_mday) + "/" + to_string(end_of_mon.tm_mon + 1) + "/" + to_string(end_of_mon.tm_year + 1900) + ")");

	return make_pair(start_of_mon, end_of_mon);
}

pair<struct tm, struct tm> GetFirstAndLastWeekDaysByDate(const struct tm &_date)
{
	struct tm end_of_week, start_of_week;

	MESSAGE_DEBUG("", "", "start(" + to_string(_date.tm_mday) + "/" + to_string(_date.tm_mon + 1) + "/" + to_string(_date.tm_year + 1900) + ")");

	start_of_week.tm_sec	= 0;   // seconds of minutes from 0 to 61
	start_of_week.tm_min	= 0;   // minutes of hour from 0 to 59
	start_of_week.tm_hour	= 0;  // hours of day from 0 to 24
	start_of_week.tm_mday	= 0;  // day of month from 1 to 31
	start_of_week.tm_mon	= 0;   // month of year from 0 to 11
	start_of_week.tm_year	= 0;  // year since 1900
	start_of_week.tm_wday	= 0;  // days since sunday
	start_of_week.tm_yday	= 0;  // days since January 1st
	start_of_week.tm_isdst	= 0; // hours of daylight savings time

	end_of_week.tm_sec		= 0;   // seconds of minutes from 0 to 61
	end_of_week.tm_min		= 0;   // minutes of hour from 0 to 59
	end_of_week.tm_hour		= 0;  // hours of day from 0 to 24
	end_of_week.tm_mday		= 0;  // day of month from 1 to 31
	end_of_week.tm_mon		= 0;   // month of year from 0 to 11
	end_of_week.tm_year		= 0;  // year since 1900
	end_of_week.tm_wday		= 0;  // days since sunday
	end_of_week.tm_yday		= 0;  // days since January 1st
	end_of_week.tm_isdst	= 0; // hours of daylight savings time

	start_of_week.tm_year	= _date.tm_year;
	start_of_week.tm_mon	= _date.tm_mon;
	start_of_week.tm_mday	= _date.tm_mday;

	end_of_week.tm_year		= _date.tm_year;
	end_of_week.tm_mon		= _date.tm_mon;
	end_of_week.tm_mday		= _date.tm_mday;

	mktime(&start_of_week);
	mktime(&end_of_week);

	start_of_week.tm_mday	= start_of_week.tm_mday - start_of_week.tm_wday + 1;

	end_of_week.tm_mday		= end_of_week.tm_mday - end_of_week.tm_wday + 7;

	mktime(&start_of_week);
	mktime(&end_of_week);

	MESSAGE_DEBUG("", "", "finish (" + to_string(start_of_week.tm_mday) + "/" + to_string(start_of_week.tm_mon + 1) + "/" + to_string(start_of_week.tm_year + 1900) + " - " + to_string(end_of_week.tm_mday) + "/" + to_string(end_of_week.tm_mon + 1) + "/" + to_string(end_of_week.tm_year + 1900) + ")");

	return make_pair(start_of_week, end_of_week);
}

pair<struct tm, struct tm> GetFirstAndLastDateOfThisMonth()
{
	time_t rawtime;
	struct tm * timeinfo;

	struct tm _date;

	MESSAGE_DEBUG("", "", "start");

	_date.tm_sec	= 0;   // seconds of minutes from 0 to 61
	_date.tm_min	= 0;   // minutes of hour from 0 to 59
	_date.tm_hour	= 0;  // hours of day from 0 to 24
	_date.tm_mday	= 0;  // day of month from 1 to 31
	_date.tm_mon	= 0;   // month of year from 0 to 11
	_date.tm_year	= 0;  // year since 1900
	_date.tm_wday	= 0;  // days since sunday
	_date.tm_yday	= 0;  // days since January 1st
	_date.tm_isdst	= 0; // hours of daylight savings time

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	_date.tm_year	= timeinfo->tm_year;
	_date.tm_mon	= timeinfo->tm_mon;
	_date.tm_mday	= 1;

	mktime(&_date);

	MESSAGE_DEBUG("", "", "finish");

	return GetFirstAndLastMonthDaysByDate(_date);
}

pair<struct tm, struct tm> GetFirstAndLastDateOfLastMonth()
{
	time_t rawtime;
	struct tm * timeinfo;

	struct tm _date;

	MESSAGE_DEBUG("", "", "start");

	_date.tm_sec	= 0;   // seconds of minutes from 0 to 61
	_date.tm_min	= 0;   // minutes of hour from 0 to 59
	_date.tm_hour	= 0;  // hours of day from 0 to 24
	_date.tm_mday	= 0;  // day of month from 1 to 31
	_date.tm_mon	= 0;   // month of year from 0 to 11
	_date.tm_year	= 0;  // year since 1900
	_date.tm_wday	= 0;  // days since sunday
	_date.tm_yday	= 0;  // days since January 1st
	_date.tm_isdst	= 0; // hours of daylight savings time

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	_date.tm_year	= timeinfo->tm_year;
	_date.tm_mon	= timeinfo->tm_mon;
	_date.tm_mday	= 0;

	mktime(&_date);

	MESSAGE_DEBUG("", "", "finish");

	return GetFirstAndLastMonthDaysByDate(_date);
}

pair<struct tm, struct tm> GetFirstAndLastDateOfThisWeek()
{
	time_t rawtime;
	struct tm * timeinfo;

	struct tm _date;

	MESSAGE_DEBUG("", "", "start");

	_date.tm_sec	= 0;   // seconds of minutes from 0 to 61
	_date.tm_min	= 0;   // minutes of hour from 0 to 59
	_date.tm_hour	= 0;  // hours of day from 0 to 24
	_date.tm_mday	= 0;  // day of month from 1 to 31
	_date.tm_mon	= 0;   // month of year from 0 to 11
	_date.tm_year	= 0;  // year since 1900
	_date.tm_wday	= 0;  // days since sunday
	_date.tm_yday	= 0;  // days since January 1st
	_date.tm_isdst	= 0; // hours of daylight savings time

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	_date.tm_year	= timeinfo->tm_year;
	_date.tm_mon	= timeinfo->tm_mon;
	_date.tm_mday	= timeinfo->tm_mday;

	mktime(&_date);

	MESSAGE_DEBUG("", "", "finish");

	return GetFirstAndLastWeekDaysByDate(_date);
}

pair<struct tm, struct tm> GetFirstAndLastDateOfLastWeek()
{
	time_t rawtime;
	struct tm * timeinfo;

	struct tm _date;

	MESSAGE_DEBUG("", "", "start");

	_date.tm_sec	= 0;   // seconds of minutes from 0 to 61
	_date.tm_min	= 0;   // minutes of hour from 0 to 59
	_date.tm_hour	= 0;  // hours of day from 0 to 24
	_date.tm_mday	= 0;  // day of month from 1 to 31
	_date.tm_mon	= 0;   // month of year from 0 to 11
	_date.tm_year	= 0;  // year since 1900
	_date.tm_wday	= 0;  // days since sunday
	_date.tm_yday	= 0;  // days since January 1st
	_date.tm_isdst	= 0; // hours of daylight savings time

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	_date.tm_year	= timeinfo->tm_year;
	_date.tm_mon	= timeinfo->tm_mon;
	_date.tm_mday	= timeinfo->tm_mday - 7;

	mktime(&_date);

	MESSAGE_DEBUG("", "", "finish");

	return GetFirstAndLastWeekDaysByDate(_date);
}

auto GetSpellingDate(long int seconds_since_epoch) -> string
{
	C_Date_Spelling	date_spelling(*(localtime(&seconds_since_epoch)));

	return date_spelling.Spell();
}

auto GetSpellingFormattedDate(string date, string format) -> string
{
	return GetSpellingFormattedDate(GetTMObject(date), format);
}

auto GetSpellingFormattedDate(struct tm date_obj, string format) -> string
{
	MESSAGE_DEBUG("", "", "start (" + format + ")");

	auto	result = ""s;
	char	buffer[100];

	mktime(&date_obj);

	if(strftime(buffer, sizeof(buffer), format.c_str(), &date_obj))
	{
		result = buffer;
	}
	else
	{
		MESSAGE_ERROR("", "", "strftime returns 0");
	}


	MESSAGE_DEBUG("", "", "finish (" + result + ")");

	return result;
}



struct tm GetTMObject(string date)
{
	MESSAGE_DEBUG("", "", "start (" + date + ")");

	struct tm	result;
	smatch		sm;
	
	result.tm_sec	= 0;   // seconds of minutes from 0 to 61
	result.tm_min	= 0;   // minutes of hour from 0 to 59
	result.tm_hour	= 0;  // hours of day from 0 to 24
	result.tm_mday	= 1;  // day of month from 1 to 31
	result.tm_mon	= 1;   // month of year from 0 to 11
	result.tm_year	= 70;  // year since 1900
	result.tm_wday	= 0;  // days since sunday
	result.tm_yday	= 0;  // days since January 1st
	result.tm_isdst	= 0; // hours of daylight savings time

	// --- normalize start, end format
	if(regex_match(date, sm, regex("^([[:digit:]]{1,2})\\/([[:digit:]]{1,2})\\/([[:digit:]]{2,4})$")))
	{
		result.tm_mday = stoi(sm[1]);
		result.tm_mon = stoi(sm[2]) - 1;
		if(stoi(sm[3]) < 100)
			result.tm_year = 2000 + stoi(sm[3]) - 1900;
		else
			result.tm_year = stoi(sm[3]) - 1900;
	}
	else if(regex_match(date, sm, regex("^([[:digit:]]{2,4})\\-([[:digit:]]{1,2})\\-([[:digit:]]{1,2})$")))
	{
		result.tm_mday = stoi(sm[3]);
		result.tm_mon = stoi(sm[2]) - 1;
		if(stoi(sm[1]) < 100)
			result.tm_year = 2000 + stoi(sm[1]) - 1900;
		else
			result.tm_year = stoi(sm[1]) - 1900;
	}
	else
	{
		MESSAGE_ERROR("", "", "incorrect date(" + date + ") format");
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

bool operator <(const struct tm &param_1, const struct tm &param_2)
{
	bool		result = false;
	struct tm	tm_1 = param_1;
	struct tm	tm_2 = param_2;

	MESSAGE_DEBUG("", "", "start");

	if(difftime(mktime(&tm_2), mktime(&tm_1)) > 0) result = true;

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

bool operator <=(const struct tm &param_1, const struct tm &param_2)
{
	bool		result = false;
	struct tm	tm_1 = param_1;
	struct tm	tm_2 = param_2;

	MESSAGE_DEBUG("", "", "start");

	if(difftime(mktime(&tm_2), mktime(&tm_1)) >= 0) result = true;

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

bool operator >(const struct tm &param_1, const struct tm &param_2)
{
	bool		result = false;
	struct tm	tm_1 = param_1;
	struct tm	tm_2 = param_2;

	MESSAGE_DEBUG("", "", "start");

	if(difftime(mktime(&tm_2), mktime(&tm_1)) < 0) result = true;

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

bool operator >=(const struct tm &param_1, const struct tm &param_2)
{
	bool		result = false;
	struct tm	tm_1 = param_1;
	struct tm	tm_2 = param_2;

	MESSAGE_DEBUG("", "", "start");

	if(difftime(mktime(&tm_2), mktime(&tm_1)) <= 0) result = true;

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

bool operator ==(const struct tm &param_1, const struct tm &param_2)
{
	bool		result = false;
	struct tm	tm_1 = param_1;
	struct tm	tm_2 = param_2;

	MESSAGE_DEBUG("", "", "start");

	if(difftime(mktime(&tm_2), mktime(&tm_1)) == 0) result = true;

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

string PrintDate(const struct tm &_tm)
{
	string	result = "";

	result = to_string(_tm.tm_mday) + "/" + to_string(_tm.tm_mon + 1) + "/" + to_string(_tm.tm_year + 1900);

	return result;
}

string PrintSQLDate(const struct tm &_tm)
{
	string	result = "";

	result = to_string(_tm.tm_year + 1900) + "-" + to_string(_tm.tm_mon + 1) + "-" + to_string(_tm.tm_mday);

	return result;
}

string PrintDateTime(const struct tm &_tm)
{
	return PrintTime(_tm, "%T");
}

string PrintTime(const struct tm &_tm, string format)
{
	char		buffer[256];
	string		result = "";
	struct tm	temp_tm = _tm;

	mktime(&temp_tm);
	memset(buffer, 0, sizeof(buffer));
	strftime(buffer, 255, format.c_str(), &temp_tm);
	result = buffer;

	return result;
}

