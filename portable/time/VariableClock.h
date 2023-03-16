// VariableClock.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef VariableClock_h
#define VariableClock_h

#include <unistd.h>
#include <sys/time.h>
#include <time.h>

namespace portable
{

class VariableClock
{	static double speed;
	static timeval dayStart;
	static timespec clockStart;
public:
	VariableClock()
	{	SetSpeed(1.);
	}
	static bool IsNormaltime()
	{	if(speed<=0.)
		{	return true;// negative time ignored
		}
		if(speed<1.)
		{	return false;// fractional speed
		}
		if(speed>1.)
		{	return false;// high speed
		}
		return true;// 1X speed
	}
	static void SetSpeed(double speed=1.)
	{	::gettimeofday(&dayStart,nullptr);
		::clock_gettime(CLOCK_MONOTONIC,&clockStart);
		VariableClock::speed = speed;
	}
	static int gettimeofday(struct timeval* tv, struct timezone* tz)
	{	if(IsNormaltime())
		{	return ::gettimeofday(tv,tz);
		}
		timeval delta;
		::gettimeofday(&delta,tz);
		delta.tv_sec-=dayStart.tv_sec;
		delta.tv_sec=static_cast<long>(double(delta.tv_sec)*speed);
		delta.tv_usec-=dayStart.tv_usec;
		delta.tv_usec=static_cast<long>(double(delta.tv_usec)*speed);
		tv->tv_sec=dayStart.tv_sec+delta.tv_sec;
		tv->tv_usec=dayStart.tv_usec+delta.tv_sec;
		return 0;		
	}
	static int clock_gettime(clockid_t clk_id, struct timespec *tp)
	{	if(IsNormaltime())
		{	return ::clock_gettime(clk_id,tp);
		}
		timespec delta;
		::clock_gettime(clk_id,&delta);
		delta.tv_sec-=clockStart.tv_sec;
		delta.tv_sec=static_cast<time_t>(double(delta.tv_sec)*speed);
		delta.tv_nsec-=clockStart.tv_nsec;
		delta.tv_nsec=static_cast<long>(double(delta.tv_nsec)*speed);
		tp->tv_sec=clockStart.tv_sec+delta.tv_sec;
		tp->tv_nsec=clockStart.tv_nsec+delta.tv_nsec;
		return 0;		
}	};

}

#endif
