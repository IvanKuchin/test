#ifndef __CRON_DAILY_PD__H__
#define __CRON_DAILY_PD__H__

#include <sstream>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>
#include <utility>		// --- pair 
#include <set>			// --- Use "set" to build set of bestMessages to re-post

#include "ccgi.h"
#include "cmysql.h"
#include "cuser.h"
#include "cmail.h"
#include "cstatistics.h"
#include "utilities.h"
#include "clog.h"
#include "localy.h"

struct	MessageType
{
	string				messageID;
	int					commentsValue;
	pair<float, float>	gps;
};

struct	LocationBubble
{
	pair<float, float>	center;
	float				radius;
	long int			counter;
};



#endif
