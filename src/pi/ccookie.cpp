#include "ccookie.h"

CCookie::CCookie() 
{
	// --- 5am used to avoid jumping between dates in case of DST
	strptime("1/1/1970 05:00:00", "%d/%m/%Y %H:%M:%S", &expiration_tm);
	expiration_tm.tm_isdst = -1;      /*  Not set by strptime(); tells mktime()
				                          to determine whether daylight saving time
				                          is in effect */
	mktime(&expiration_tm);
}

void	CCookie::SetNew(bool s) 
{
	MESSAGE_DEBUG("", "", "name = " + GetName());

	isNew = s;
}

string	CCookie::GetExpiration()
{
	MESSAGE_DEBUG("", "", "start (name is " + GetName() + ")");

	auto	result = ""s;

	if((expiration_tm.tm_year + 1900) > 1971)
	{
		auto t = mktime(&expiration_tm);
		auto utc_tm = gmtime(&t);

		if(utc_tm)
		{
			char			utc_str[100];
			auto			curr_locale	= string(setlocale(LC_ALL, NULL));	// --- DO NOT remove string() !
																			// --- otherwise following setlocale call will change memory content where curr_locale pointing out
																			// --- string() copies memory content to local stack therefore it could be reused later.
		    setlocale(LC_ALL, LOCALE_ENGLISH.c_str());
			auto	str_length = strftime(utc_str, sizeof(utc_str), "%a, %02d %b %Y %T %Z", utc_tm);
		    setlocale(LC_ALL, curr_locale.c_str());

			if(str_length)
			{
				result = utc_str;
			} 
			else
			{
				MESSAGE_ERROR("", "", "strftime(" + utc_str + ", " + to_string(sizeof(utc_str)) + ", '', utc_tm) buffer size is not enough to convert date to GMT");
			}
		}
		else
		{
			MESSAGE_ERROR("", "", "fail from gmtime(&t)  (possible problem with cookie expiration)");
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "session cookie");
	}


	MESSAGE_DEBUG("", "", "finish(" + result + ")");

	return result;
}

string CCookie::SetExpirationShiftedFromNow(int delta_seconds)
{
	MESSAGE_DEBUG("", "", "start");

	auto		error_message = ""s;
	time_t 		curr_time;
	struct tm	*curr_tm;

	curr_time = time(NULL);
	curr_tm = localtime(&curr_time);
	curr_tm->tm_sec += delta_seconds;

	SetExpiration(curr_tm);
	mktime(&expiration_tm);

	MESSAGE_DEBUG("", "", "finish(" + PrintDateTime(expiration_tm) + " " + PrintDate(expiration_tm) + ")");

	return error_message;

}




CCookie CCookies::PreBuildNewCookie(string name, string value, string domain, string path, string secure, bool newCookie)
{
	CCookie		c;

	if((cookies.capacity() - cookies.size()) <= 0)
	{
		MESSAGE_ERROR("", "", "Size of cookie vector was overflown (memory could be reallocated).");
	}

	c.SetName(name);
	c.SetValue(value);
	c.SetDomain(domain);
	c.SetPath(path);
	c.SetSecure(secure);
	c.SetNew(newCookie);

	return c;
}

void CCookies::Add(string name, string value, struct tm *expiration, string domain, string path, string secure, bool newCookie)
{
	MESSAGE_DEBUG("", "", "start (add " + name + "=" + value + ";expiration=" + (expiration ? to_string(expiration->tm_hour) + ":" + to_string(expiration->tm_min) + ":" + to_string(expiration->tm_sec) + " " + to_string(expiration->tm_mday) + "/" + to_string(expiration->tm_mon) + "/" + to_string(expiration->tm_year) : "nullptr") + ";newCookie=" + to_string(newCookie) + ")");

	auto c = PreBuildNewCookie(name, value, domain, path, secure, newCookie);

	c.SetExpiration(expiration);

	Remove_VectorInstance_IfExists(name);

	cookies.push_back(c);

	MESSAGE_DEBUG("", "", "finish (size of cookies=" + to_string(cookies.size()) + " object)");
}

void CCookies::Add(string name, string value, int shift_in_seconds_from_now, string domain, string path, string secure, bool newCookie)
{
	MESSAGE_DEBUG("", "", "start (add " + name + "=" + value + ";shift_in_seconds_from_now=" + to_string(shift_in_seconds_from_now) + ";newCookie=" + to_string(newCookie) + ")");

	auto c = PreBuildNewCookie(name, value, domain, path, secure, newCookie);

	c.SetExpirationShiftedFromNow(shift_in_seconds_from_now);

	Remove_VectorInstance_IfExists(name);

	cookies.push_back(c);

	MESSAGE_DEBUG("", "", "finish (size of cookies=" + to_string(cookies.size()) + " object)");
}

void CCookies::Remove_VectorInstance_IfExists(string name)
{
	MESSAGE_DEBUG("", "", "start(cookies vector size is " + to_string(cookies.size()) + ")");

	for(auto itr = cookies.begin(); itr != cookies.end(); ++itr)
	{
		if(itr->GetName() == name)
		{
			cookies.erase(itr);
			--itr;
		}
	}
	MESSAGE_DEBUG("", "", "finish(cookies vector size is " + to_string(cookies.size()) + ")");
}

bool CCookies::IsExist(string name) const
{
	bool 	result = false;

	for(auto &cookie: cookies)
	{
		if(cookie.GetName() == name)
		{
			result = true;
			break;
		}
	}

	return result;
}

// --- shift timestamp and update cookies
// --- name - cookie name
// --- deltaTimeStamp - delta time in minutes
bool CCookies::UpdateTS(string name, int deltaTimeStamp)
{
	// string						value, path, timestamp, domain, secure;
	bool						result = false;

	MESSAGE_DEBUG("", "", " start (name = " + name + ", deltaTimeStamp = " + to_string(deltaTimeStamp) + ")");

	for(auto &cookie: cookies)
	{
		if(cookie.GetName() == name)
		{
			// cookie.SetValue(value);
			cookie.SetDomain("");
			cookie.SetPath("/");
			cookie.SetSecure("");
			cookie.SetNew(true);
			if(deltaTimeStamp)
			{
				// cookie.SetExpiration(timestamp);
				cookie.SetExpirationShiftedFromNow(deltaTimeStamp * 60);
				// cookie.SetMaxAge(deltaTimeStamp * 60);
			}
			else
			{
				// cookie.SetExpiration("");
			}

			result = true;
			break;
		}
	}

	if(!result)
	{
		MESSAGE_ERROR("", "", "can;t find cookie " + name);
	}

	MESSAGE_DEBUG("", "", "finish (" + to_string(result) + ")");

	return result;
}

void CCookies::Delete(string name, string domain, string path, string secure)
{
	for(auto &cookie: cookies)
	{
		if(cookie.GetName() == name)
		{
			cookie.SetValue(cookie.GetValue());
			// cookie.SetExpiration(GetTimestampShifted(-3600 * 24));
			cookie.SetExpirationShiftedFromNow(-3600 * 24);
			cookie.SetPath(path);
			// cookie.SetMaxAge(0);
			cookie.SetNew(TRUE);
//			break;
		}
	}
}

string CCookies::Get(string name) const
{
	auto result = ""s;

	for(auto &cookie: cookies)
	{
		if(cookie.GetName() == name)
		{
			result = cookie.GetValue();
			break;
		}
	}

	return result;
}

string CCookies::GetAll()
{
	string 						result;
	ostringstream				ost;

	MESSAGE_DEBUG("", "", " start");

	result = "";
	for(auto &cookie: cookies)
	{
		if(cookie.GetNew())// does this cookie needs to be sendback to server ?
		{
			auto	temp = ""s;
			auto	cookie_exp = cookie.GetExpiration();

			temp = "Set-Cookie: " + cookie.GetName() + "=" + cookie.GetValue();

			if(cookie_exp.length() > 0)
			{
				temp += "; expires=" + cookie_exp;
			}
/*
			if(cookie.GetMaxAge() >= 0)
			{
				temp += "; max-age=" + cookie.GetMaxAge();
			}
*/
			if(cookie.GetPath().length() > 0)
			{
				temp += "; path=" + cookie.GetPath();
			}
			MESSAGE_DEBUG("", "", " add cookie: " + temp); // --- keep it here to avoid \n appears in the log

			temp += "\n";

			result += temp;
		}
	}
	
	if(result.length() > 1)	result.erase(result.length() - 1);

	MESSAGE_DEBUG("", "", " finish (result len = " + to_string(result.length()) + ")");

	return result;
}

void CCookies::RegisterCookieVariables(CVars *v)
{

	MESSAGE_DEBUG("", "", " start ");

	for(auto &cookie: cookies)
		if(cookie.GetName().length())
			v->Add(cookie.GetName(), cookie.GetValue());
		else
		{
			MESSAGE_ERROR("", "", "ERROR: cookie name is empty (" + cookie.GetName() + " = " + cookie.GetValue() + ")");
		}

	MESSAGE_DEBUG("", "", " finish ");
}

void CCookies::ParseString(string str)
{
	string				result, expr, cookName, cookValue;
	string::size_type	exprPos, eqPos;

	MESSAGE_DEBUG("", "", "start ");

	result = str;
	exprPos = result.find(";");

	while((exprPos != string::npos) || (result.length() > 2))
	{
		if(exprPos != string::npos)
		{
			expr = result.substr(0, exprPos);
			result = result.erase(0, exprPos + 1);
		}
		else
		{
			expr = result;
			result.erase(0, result.length());
		}

		eqPos = expr.find("=");
		if(eqPos == string::npos)
		{
			exprPos = result.find(";");
			continue;
		}

		cookName = expr.substr(0, eqPos);
		cookValue = expr.substr(eqPos + 1, expr.length() - eqPos - 1);
		trim(cookName);
		trim(cookValue);

		MESSAGE_DEBUG("", "", "adding cookie: " + cookName + " = " + cookValue);

		if(cookName.length())
			Add(cookName, cookValue, nullptr, "", "", "", FALSE);
		else
		{
			MESSAGE_ERROR("", "", "cookie parsing end-up with empty name() and value(" + cookValue + ")");
		}

		exprPos = result.find(";");
	}

	MESSAGE_DEBUG("", "", "finish");
}
