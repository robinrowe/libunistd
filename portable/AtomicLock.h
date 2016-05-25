// AtomicLock.h
// Robin.Rowe@CinePaint.org 2014/8/22

#ifndef AtomicLock_h
#define AtomicLock_h

#include "AtomicCounter.h"

class AtomicLock
{	AtomicCounter<int> i;
public:
	AtomicLock()
	{}
	bool IsLocked() const
	{	return i==1;
	}
	bool Lock()
	{	if(i!=0)
		{	return false;
		}
		const int count=++i;
		if(count!=1)
		{	return false;
		}
		return true;
	}
	void Unlock()
	{	i=0;
}	};

#endif

