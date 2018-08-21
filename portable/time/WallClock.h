// WallClock.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef WallClock_h
#define WallClock_h

#include <ctime>
#include <ratio>
#include <chrono>

class WallClock
{	std::chrono::system_clock::time_point  start;
	std::chrono::system_clock::time_point  lastWakeUp;
public:
	WallClock()
	{	Start();
	}
	void Start()
	{	start = std::chrono::system_clock::now();
		lastWakeUp = start;
	}
	long long BumpIntervalMilliseconds(long long interval)
	{	std::chrono::milliseconds delay(interval);
		lastWakeUp += delay;
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::chrono::system_clock::duration pause = lastWakeUp - now;
		if(pause.count()<0)
		{	return 0;
		}
		return std::chrono::duration_cast<std::chrono::milliseconds>(pause).count();
	}
};

/*
const chrono::seconds seconds = chrono::duration_cast<chrono::seconds>(duration);
m_microseconds = (uint32_t)(chrono::duration_cast<chrono::microseconds>(duration - seconds).count() & UINT32_MAX);
time_t timet  = (time_t)(seconds.count() & UINT32_MAX);
localtime_s(&m_tm, &timet);
*/

#endif
