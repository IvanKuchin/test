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

// --- Distances
#define	SINGLE_MESSAGE_DISTANCE						0.1 // --- about 30 km
#define	PRESENCE_DIAMETER							2	// --- about 140 km
#define	YEAR_AGO_REPOST_THRESHOLD					80

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
