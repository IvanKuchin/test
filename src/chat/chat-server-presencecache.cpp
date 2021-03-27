#include "chat-server-presencecache.h"

CPresenceCache::CPresenceCache ()
{
	db = NULL;
	presenceCache.empty();
}

void CPresenceCache::SetDB(CMysql *param)
{
	db = param;
}

string CPresenceCache::DumpCache()
{
	ostringstream	ost;
	string			result = "";

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::DumpCache: begin";
		log.Write(DEBUG, ost.str());
	}
	
	ost.str("");
	for(auto it = presenceCache.begin(); it != presenceCache.end(); it++)
	{
		ost << (ost.str().length() ? "," : "");
		ost << "{\"userID\": \"" << it->first << "\",  \"updateCacheTimestamp\":\"" << to_string(it->second.first) << "\", \"lastUserOnline\":\"" << it->second.second << "\"}";
	}

	result = "[" + ost.str() + "]";

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::DumpCache: end";
		log.Write(DEBUG, ost.str());
	}
	
	return result;	
}

string CPresenceCache::GetLastOnlineSinceY2kFromDB(long int userID)
{
	ostringstream	ost;
	string			result = "";

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::GetLastOnlineSinceY2kFromDB(" << to_string(userID) << "): begin";
		log.Write(DEBUG, ost.str());
	}

	if(db)
	{

		ost.str("");
		ost << "select * from `users` where `id`='" << to_string(userID) << "';";
		if(db->Query(ost.str()))
		{
			result = db->Get(0, "last_onlineSecondsSinceY2k");
		}
		else
		{
			{
				CLog	log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << "CPresenceCache::GetLastOnlineSinceY2kFromDB: ERROR: db has no user [id = " << to_string(userID) << "]";
				log.Write(ERROR, ost.str());
			}

		}
	}
	else
	{
	
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << "CPresenceCache::GetLastOnlineSinceY2kFromDB: ERROR: db has not been initialized";
			log.Write(ERROR, ost.str());
		}
	}

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::GetLastOnlineSinceY2kFromDB(" << to_string(userID) << "): end (result = " << result << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}

bool CPresenceCache::RefreshCache(long int userID)
{
	bool		result = false;
	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::RefreshCache(" << to_string(userID) << "): begin";
		log.Write(DEBUG, ost.str());
	}

	if(isUserInCache(userID))
	{
		if((GetSecondsSinceY2k() - presenceCache[userID].first) >= CHAT_PRESENCE_CACHE_LIFETIME)
		{
			string	tmp = GetLastOnlineSinceY2kFromDB(userID);

			if(tmp.length())
			{
				presenceCache[userID].first = GetSecondsSinceY2k();
				presenceCache[userID].second = tmp;

				result = true;
			}
			else
			{

				{
					CLog	log(CHAT_LOG_FILE_NAME);
					ostringstream	ost;

					ost.str("");
					ost << "CPresenceCache::RefreshCache(" << to_string(userID) << "): ERROR: fail to get last_onlineSecondsSinceY2k from DB";
					log.Write(ERROR, ost.str());
				}
			}
		}
		else
		{
			result = true;
		}
	}
	else
	{
		
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << "CPresenceCache::RefreshCache(" << to_string(userID) << "): ERROR: userID [" << to_string(userID) << "] not in cache";
			log.Write(ERROR, ost.str());
		}
	}

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::RefreshCache(" << to_string(userID) << "): end (result = " << result << ")";
		log.Write(DEBUG, ost.str());
	}
	
	return result;
}

bool CPresenceCache::AddUser(long int userID)
{
	bool	result = false;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::AddUser(long int " << userID << "): begin";
		log.Write(DEBUG, ost.str());
	}

	if(isUserInCache(userID))
	{
		if(RefreshCache(userID))
		{
			result = true;
		}
		else
		{
			
			{
				CLog	log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << "CPresenceCache::AddUser: ERROR: cache refresh error for userID [" << to_string(userID) << "]";
				log.Write(ERROR, ost.str());
			}
		}
	}
	else
	{
		string	last_onlineSecondsSinceY2k = GetLastOnlineSinceY2kFromDB(userID);
		if(last_onlineSecondsSinceY2k.length())
		{
			presenceCache.emplace(userID, make_pair(GetSecondsSinceY2k(), last_onlineSecondsSinceY2k));
			result = true;
		}
		else
		{
								
			{
				CLog	log(CHAT_LOG_FILE_NAME);
				ostringstream	ost;

				ost.str("");
				ost << "CPresenceCache::AddUser: ERROR: retrieving last_onlineSecondsSinceY2k from db";
				log.Write(ERROR, ost.str());
			}
		}

	}

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::AddUser(long int " << userID << "): end (result = " << result << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}

bool CPresenceCache::AddUser(string strUserID)
{
	long int		userID;
	bool			result = false;

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::AddUser(string " << strUserID << "): begin";
		log.Write(DEBUG, ost.str());
	}

	userID = atol(strUserID.c_str());
	if(userID)
	{
		result = AddUser(userID);
	}
	else
	{
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << "CPresenceCache::AddUser: ERROR: conversation strUserID[" << strUserID << "] to long [" << to_string(userID) << "]";
			log.Write(ERROR, ost.str());
		}
	}


	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::AddUser(string " << strUserID << "): end (result = " << result << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}

string CPresenceCache::GetUserLastOnline(long int userID)
{
	// long int	userID = atol(strUserID.c_str());
	string		result = "";

	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::GetUserLastOnline(" << userID << "): begin";
		log.Write(DEBUG, ost.str());
	}

	if(userID)
	{
		if(isUserInCache(userID))
		{
			RefreshCache(userID);
		}
		else
		{
			AddUser(userID);
		}
		result = presenceCache[userID].second;
	}	
	else
	{
		
		{
			CLog	log(CHAT_LOG_FILE_NAME);
			ostringstream	ost;

			ost.str("");
			ost << "CPresenceCache::GetUserLastOnline: ERROR: failed conversation strUserID[" << userID << "] to long [" << to_string(userID) << "]";
			log.Write(ERROR, ost.str());
		}
	}


	{
		CLog	log(CHAT_LOG_FILE_NAME);
		ostringstream	ost;

		ost.str("");
		ost << "CPresenceCache::GetUserLastOnline(" << userID << "): end (result = " << result << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;
}

bool	CPresenceCache::isUserInCache(long int userID)
{
	unordered_map<long int, pair<double, string>>::iterator		it;
	bool														result = false;

	it = presenceCache.find(userID);
	if(it == presenceCache.end())
		result = false;
	else
		result = true;

	return result;
}

CPresenceCache::~CPresenceCache ()
{

}

