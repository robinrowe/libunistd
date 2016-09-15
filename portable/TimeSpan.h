// TimeSpan.h
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef TimeSpan_h
#define TimeSpan_h

#include <chrono>
#include <stdio.h>

namespace portable
{

class TimeSpan
{   double longestDuration;
	double shortestDuration;
	unsigned tickCount;
	std::chrono::steady_clock::time_point start; 
public:
    TimeSpan()
	{	Reset();
	}
	void Reset()
	{	tickCount = 0;
		longestDuration = 0.;
		shortestDuration = -1.;
		start = std::chrono::steady_clock::now();
	}
	void Snap(const char* msg = nullptr)
	{	tickCount++;
		auto end = std::chrono::steady_clock::now();
 		std::chrono::duration<double,std::milli> interval = end - start;
		if(interval.count()>longestDuration)
		{	if(msg)
			{	printf("%s: increasing from %fus to %fus at tick %u\n",msg,longestDuration,interval.count(),tickCount);
			}
			longestDuration = interval.count();
		}
		if(-1. == shortestDuration || interval.count()<shortestDuration)
		{	shortestDuration = interval.count();
		}
		start = end;
	}
	double GetLongest() const
	{	return longestDuration;
	} 
	double GetShortest() const
	{	return shortestDuration;
	} 
	unsigned GetTickCount() const
	{	return tickCount;
	}
};

}

#endif
/* Use like this...

int main()
{	TimeSpan timeSpan;// starts time here
	for(unsigned i=0;i<5;i++)
	{	sleep(5);
		timeSpan.Snap();// checks interval here
	}
	printf("TimeSpan intervals: shortest = %f, longest = %f, tickCount = %u",timeSpan.GetShortest(),timeSpan.GetLongest(), timeSpan.GetTickCount());
	return 1;
}

*/
