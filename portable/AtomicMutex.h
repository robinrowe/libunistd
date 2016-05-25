// libportable/AtomicMutex.h
// Robin.Rowe@cinepaint.org 2014/11/20

#ifndef AtomicMutex_h
#define AtomicMutex_h

#include <atomic>

class AtomicMutex
{	friend class SoftLock;
	std::atomic<int> lock;
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
public:
	AtomicMutex()
	{	lock=0;
	}
	bool IsLocked() const
	{	return 1==lock;
	}
};

#endif
