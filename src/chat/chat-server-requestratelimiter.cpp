#include "chat-server-requestratelimiter.h"

int CChatRequestRatelimiter::IncreaseCounter(string userIP)
{
	double 		currTimestamp = GetSecondsSinceY2k();

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CChatRequestRatelimiter::IncreaseCounter(userIP [" << userIP <<"]): begin";
		log.Write(DEBUG, ost.str());
	}
	

	if(!isExist(userIP))
	{
		requestRatelimiter[userIP] = make_pair<double, int>(0, 0);
	}

	if((currTimestamp - requestRatelimiter[userIP].first) <= 5 /*per second rate-limiting*/ )
		requestRatelimiter[userIP].second++;
	else
		requestRatelimiter[userIP].second = 1;
	requestRatelimiter[userIP].first = currTimestamp;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CChatRequestRatelimiter::IncreaseCounter(userIP [" << userIP <<"]): end (return counter = " << requestRatelimiter[userIP].second << ")";
		log.Write(DEBUG, ost.str());
	}
	
	return requestRatelimiter[userIP].second;
}

double	CChatRequestRatelimiter::GetLastUserRequestTimestamp(string userIP)
{
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CChatRequestRatelimiter::GetLastUserRequestTimestamp(userIP [" << userIP <<"]): begin";
		log.Write(DEBUG, ost.str());
	}
	
	if(!isExist(userIP))
	{
		requestRatelimiter[userIP] = make_pair<double, int>(0, 0);
	}

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CChatRequestRatelimiter::GetLastUserRequestTimestamp(userIP [" << userIP <<"]): end (return timestamp = " << requestRatelimiter[userIP].second << ")";
		log.Write(DEBUG, ost.str());
	}
	return requestRatelimiter[userIP].second;
}

bool CChatRequestRatelimiter::isRatelimited(string userIP)
{
	double 		currTimestamp = GetSecondsSinceY2k();
	bool		result = false;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CChatRequestRatelimiter::isRatelimited(userIP [" << userIP <<"]): begin";
		log.Write(DEBUG, ost.str());
	}

	if(!isExist(userIP))
	{
		requestRatelimiter[userIP] = make_pair<double, int>(0, 0);
	}

	if((currTimestamp - requestRatelimiter[userIP].first) <= 5 /*per second rate-limiting*/ )
		if(requestRatelimiter[userIP].second >= CHAT_MESSAGES_REQUEST_PER_SEC)
			result = true;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CChatRequestRatelimiter::isRatelimited(userIP [" << userIP <<"]): end (return isRatelimited = " << (result ? "yes" : "no") << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}



bool CChatRequestRatelimiter::isExist(string userIP)
{
	auto	it = requestRatelimiter.find(userIP);
	bool	result = false;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CChatRequestRatelimiter::isExist(userIP [" << userIP <<"]): start";
		log.Write(DEBUG, ost.str());
	}
	

	if(it == requestRatelimiter.end())
	{
		result = false;
	}
	else
	{
		result = true;
	}

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CChatRequestRatelimiter::isExist(userIP [" << userIP <<"]): end (return isExist = " << (result ? "yes" : "no") << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}

CChatRequestRatelimiter::~CChatRequestRatelimiter()
{

}