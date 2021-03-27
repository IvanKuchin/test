#include "cstatistics.h"

CStatistics::CStatistics(): logFilename (LOG_FILE_NAME)
{
	startClock = clock();
	startTime = time(NULL);
	startChrono = std::chrono::duration_cast<std::chrono::microseconds> (system_clock::now().time_since_epoch());
}

CStatistics::~CStatistics()
{
	endClock = clock();
	endTime = time(NULL);
	endChrono = std::chrono::duration_cast<std::chrono::microseconds> (system_clock::now().time_since_epoch());

	MESSAGE_DEBUG("", "", "time duration (in ticks) = " + to_string((float)(endClock - startClock) /  CLOCKS_PER_SEC));
	MESSAGE_DEBUG("", "", "sec duration = " + to_string(endTime - startTime));
	MESSAGE_DEBUG("", "", "chrono duration = " + to_string(endChrono.count() - startChrono.count()) + " microseconds");
}
