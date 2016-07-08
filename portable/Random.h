// MinimalStandardRandom
// 2013/9/28
// http://c-faq.com/lib/rand.html

#ifndef MinimalStandardRandom_h
#define MinimalStandardRandom_h

// NOTE: The size of int is 4 bytes and the size of long long is 
// 8 bytes on Linux, Mac and Windows. The size of long is 4 bytes 
// on Windows 32/64 and 32-bit Linux/Mac, but is 8 bytes on 64-bit
// Linux/Mac. Consequently, we do not use long as shown in the c-faq.
// http://software.intel.com/en-us/articles/size-of-long-integer-type-on-different-architecture-and-os

#include <time.h>

class MinimalStandardRandom
{	static const int a=48271;//16807;
	static const int m=2147483647;
	static const int q=m/a;
	static const int r=m%a;
	int seed;
public:
	MinimalStandardRandom(int seed=0)
	:	seed(seed)
	{	if(!seed)
		{	time_t seconds;
			time(&seconds); 
			this->seed=(int) seconds;
	}	}
	int NextPositive() // range [1, 2147483646], except 0
	{	const int hi = seed / q;
		const int lo = seed % q;
		const int test = a * lo - r * hi;
		if(test > 0)
		{	seed = test;
		}
		else	
		{	seed = test + m;
		}
		return seed;
	}
	int Next() // range +-2147483646 except 0
	{	const int hi = seed / q;
		const int lo = seed % q;
		seed = a * lo - r * hi;
		return seed;
	}
};

#endif
