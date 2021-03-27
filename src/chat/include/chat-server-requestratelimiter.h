#ifndef __CHAT__SERVER__REQUESTRATELIMITER__H__
#define __CHAT__SERVER__REQUESTRATELIMITER__H__

#include <unordered_map>
#include <utility>
#include "cmysql.h"
#include "clog.h"
#include "localy.h"

using namespace std;

extern double GetSecondsSinceY2k();

class CChatRequestRatelimiter {
private:
	// --- presenceMap
	//		key - user IP
	//		value - pair <double, string>
	//				.first - last request timestamp (seconds sinceY2k)
	//				.second - number of attempts on previous time interval
	unordered_map<string, pair<double, int>>		requestRatelimiter;

public:
			CChatRequestRatelimiter() = default;

	bool	isExist(string userIP);
	bool	isRatelimited(string userIP);
	int		IncreaseCounter(string userIP);
	double	GetLastUserRequestTimestamp(string userIP);

			~CChatRequestRatelimiter();
};


#endif