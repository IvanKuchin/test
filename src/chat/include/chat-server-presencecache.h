#ifndef __CHAT__SERVER__PRESENCECACHE__H__
#define __CHAT__SERVER__PRESENCECACHE__H__

#include <unordered_map>
#include <utility>

#include "chat-server-const.h"
#include "cmysql.h"
#include "clog.h"
#include "localy.h"

using namespace std;

extern double GetSecondsSinceY2k();

class CPresenceCache {
private:
	// --- presenceMap
	//		key - userID
	//		value - pair <double, string>
	//				.first - update cache timestamp (seconds sinceY2k)
	//				.second - last user online timestamp (seconds sinceY2k)
	unordered_map<long int, pair<double, string>>		presenceCache;
	CMysql												*db;

	string	GetLastOnlineSinceY2kFromDB(long int userID);
	bool	RefreshCache(long int userID);

public:
			CPresenceCache();

	void	SetDB(CMysql *param);

	bool	isUserInCache(long int userID);
	bool	AddUser(long int userID);
	bool	AddUser(string strUserID);
	string	GetUserLastOnline(long int userID);

	string	DumpCache();

			~CPresenceCache();
};


#endif