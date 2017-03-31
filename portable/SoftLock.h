// libportable/SoftLock.h
// Robin.Rowe@cinepaint.org 2014/11/20

#ifndef SoftLock_h
#define SoftLock_h

#include "AtomicMutex.h"

namespace portable
{

class AtomicMutex;

class SoftLock
{	const bool isLocked;
	AtomicMutex& atomicMutex;
	SoftLock(SoftLock&);
	void operator=(SoftLock&);
public:
	~SoftLock()
	{	if(isLocked)
		{	atomicMutex.Unlock();
	}	}
	SoftLock(AtomicMutex& atomicMutex)
	:	atomicMutex(atomicMutex),
		isLocked(atomicMutex.Lock())
	{}
	bool IsLocked() const
	{	return isLocked;
	}
	bool operator!() const
	{	return !isLocked;
	}
};

}

#endif
