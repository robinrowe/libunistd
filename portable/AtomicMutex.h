// libportable/AtomicMutex.h
// Robin.Rowe@cinepaint.org 2014/11/20

#ifndef AtomicMutex_h
#define AtomicMutex_h

#ifdef UNREAL_ENGINE
#include <AllowWindowsPlatformTypes.h>
#endif

#include <atomic>

#ifdef UNREAL_ENGINE
#include <HideWindowsPlatformTypes.h>
#endif

namespace portable
{

class AtomicMutex
{	friend class SoftLock;
	std::atomic<int> lock;
public:
	AtomicMutex()
	{	lock=0;
	}
	bool IsLocked() const
	{	return 1==lock;
	}
	bool operator!() const
	{	return !IsLocked();
	}
	bool Lock()
	{	const int lockCount=lock.fetch_add(1,std::memory_order_relaxed)+1;
		const bool isLocked = (1==lockCount);
		if(!isLocked)
		{	Unlock();
		}
		return isLocked;
	}
	int Unlock()
	{	const int lockCount=lock.fetch_sub(1,std::memory_order_relaxed)-1;
		return lockCount;
	}
};

}

#endif
