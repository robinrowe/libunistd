// AtomicLock.h
// 2014/8/4

#ifndef AtomicLock_h
#define AtomicLock_h

#include "AtomicCounter.h"

class AtomicLock
{	AtomicCounter<int>& i;
	int count;
public:
	AtomicLock(AtomicCounter<int>& i)
	:	i(i)
	{	count=++i;
	}
	~AtomicLock()
	{	--i;
	}
	bool operator!() const
	{	return count!=1;
}	};

#endif

