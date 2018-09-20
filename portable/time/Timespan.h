// Timespan.h
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef Timespan_h
#define Timespan_h

#include <chrono>
#include <stdio.h>

namespace portable
{

class Timespan
{   double longestDuration;
	double shortestDuration;
	unsigned tickCount;
	std::chrono::steady_clock::time_point start; 
public:
    Timespan()
	{	Reset();
	}
	void Reset(const char* msg = nullptr)
	{	tickCount = 0;
		longestDuration = 0.;
		shortestDuration = -1.;
		start = std::chrono::steady_clock::now();
		if(msg)
		{	printf("%s: resetting from %fs at tick %u\n",msg,longestDuration/1000,tickCount);
	}	}
	void Snap(const char* msg = nullptr)
	{	tickCount++;
		auto end = std::chrono::steady_clock::now();
 		std::chrono::duration<double,std::milli> interval = end - start;
		if(interval.count()>longestDuration)
		{	if(msg)
			{	printf("%s: increasing from %fs to %fs at tick %u\n",msg,longestDuration/1000,interval.count()/1000,tickCount);
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
{	Timespan Timespan;// starts time here
	for(unsigned i=0;i<5;i++)
	{	sleep(5);
		Timespan.Snap();// checks interval here
	}
	printf("Timespan intervals: shortest = %f, longest = %f, tickCount = %u",Timespan.GetShortest(),Timespan.GetLongest(), Timespan.GetTickCount());
	return 1;
}

*/
