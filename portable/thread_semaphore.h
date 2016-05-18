// thread_semaphore.h
// 2013/10/31

#ifndef thread_semaphore_h
#define thread_semaphore_h

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/lock_guard.hpp> 
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <iostream>
using namespace std;
//#include <boost/interprocess/sync/interprocess_semaphore.hpp>

//typedef boost::mutex mutex;
typedef boost::lock_guard<boost::mutex> lock_guard;

#ifdef USE_IPC_SEMAPHORE
typedef boost::interprocess::interprocess_semaphore semaphore;
#else

class thread_semaphore
{	boost::mutex protect;
	boost::condition_variable cond;
	typedef boost::unique_lock<boost::mutex> Lock;
public:
	thread_semaphore()
	{}
	void Wait(long long microSeconds)
	{	if(0>=microSeconds)
		{	return;
		}	
		Lock lock(protect);
		boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(microSeconds);	
		if(!cond.timed_wait(lock,timeout))
		{	// false if timeout
			return;
		}
	}
	void Wait()
	{	Lock lock(protect);
		cond.wait(lock);
	}
	void Signal()
	{	cond.notify_one();
	}
};

class thread_lock
{	typedef boost::lock_guard<boost::mutex> Lock;
	boost::mutex protect;
	Lock lock;	
public:
	thread_lock()
	:	lock(protect)
	{}
};

#if 0

class thread_mutex
{	
public:
	bool Lock()
	{	try
		{	lock();
		}
		catch(...)
		{	return false;
		}
		return true;
	}
	bool Unlock()
	{	try
		{	unlock();
		}
		catch(...)
		{	return false;
		}
		return true;
	}
};
#endif

#endif

#endif
