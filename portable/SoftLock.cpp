// libportable/SoftLock.cpp
// Robin.Rowe@cinepaint.org 2014/11/20

#include "SoftLock.h"
#include "AtomicMutex.h"

namespace portable
{

SoftLock::~SoftLock()
{	if(isLocked)
	{	atomicMutex.Unlock();
}	}

SoftLock::SoftLock(AtomicMutex& atomicMutex)
:	atomicMutex(atomicMutex),
	isLocked(atomicMutex.Lock())
{}

}

