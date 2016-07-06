// TimeSpan.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef TimeSpan_h
#define TimeSpan_h

#include <chrono>
#include <stdio.h>

namespace portable
{

class TimeSpan
{   double longestDuration;
	std::chrono::steady_clock::time_point start; 
public:
    TimeSpan()
	:	longestDuration(0.)
	{	Reset();
	}
	void Reset()
	{	start = std::chrono::steady_clock::now();
	}
	void Snap(const char* msg = nullptr)
	{	auto end = std::chrono::steady_clock::now();
 		std::chrono::duration<double,std::milli> interval = end - start;
		if(interval.count()>longestDuration)
		{	if(msg)
			{	printf("%s: increasing from %fus to %fus\n",msg,longestDuration,interval.count());
			}
			longestDuration = interval.count();
		}
		start = end;
		// integral duration: requires duration_cast
		// auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
	}
	operator double() const
	{	return longestDuration;
	} 
};

}

#endif
