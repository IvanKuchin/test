#ifndef	__CSTATISTICS__H__
#define	__CSTATISTICS__H__

#include <chrono>
#include <time.h>
#include "clog.h"

class CStatistics
{
	private:
		clock_t						startClock, endClock;
		int							startTime, endTime;
		string						logFilename;
		std::chrono::microseconds	startChrono, endChrono;
	public:

				CStatistics();
		void	SetLogFile(const string &param)				{ logFilename = param; };
		void	SetLogFile(string &&param)		noexcept	{ logFilename = move(param); };
				~CStatistics();
};

#endif