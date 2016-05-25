// semaphore.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef semaphore_h
#define semaphore_h

#ifndef __cplusplus
#error C++11 required
#endif

#include "unistd.h"
#include "stub.h"
#include <time.h>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <string>
#include <chrono>

class sem_t
{	bool isGood;
	std::mutex m;
	std::condition_variable cv;
	std::unique_lock<std::mutex> lk;
	std::atomic<int> v;
	std::string name;
public:
	~sem_t()
	{}
	sem_t()
	:	v(0)
	,	isGood(true)
	,	lk(m,std::defer_lock_t())
	{}
	static sem_t* sem_open(const char *name, int oflag)
	{	sem_t* st = new sem_t;
		st->name = name;
		return st;
	}
	static sem_t* sem_open(const char *name, int oflag,mode_t mode, unsigned int value)
	{	sem_t* st = new sem_t;
		st->name = name;
		st->v.exchange(value);
		return st;
	}
	int sem_init(int pshared, unsigned int value)
	{	if(!isGood)
		{	puts("ERROR: Windows sem_t clobbered by memset 0");
			return -1;  
		}
		v.exchange(value);
		return 0;
	}
	static int sem_close(sem_t *sem)
	{	delete sem;
		return 0;
	}
	int sem_destroy()
	{	return 0;
	}
	int sem_getvalue(sem_t *restrict, int *restrict2)
	{	*restrict2=0;
		return v;
	}
	int sem_trywait()
	{	const int prev = v.fetch_sub(1);
		if(prev > 0)
		{	return 0;
		}
		v.fetch_add(1);
		return -1;
	}
	int sem_wait()
	{	if(0==sem_trywait())
		{	return 0;
		}
		lk.lock();
		cv.wait(lk);
		return 0;
	}
	int sem_timedwait(const struct timespec* ts)
	{	if(0==sem_trywait())
		{	return 0;
		}
		lk.lock();
		time_t delay = 1000*ts->tv_sec;
		delay += (ts->tv_nsec)/1000;
		std::chrono::milliseconds duration(delay);
		cv.wait_for(lk,duration);
		return 0;
	}
	int sem_post()
	{	if(0!=sem_trywait())
		{	return 0;
		}
		lk.unlock();
		return 0;
	}
	static int sem_unlink(const char *)
	STUB0(sem_unlink)
};

/* Form Linux man:

sem_wait() decrements (locks) the semaphore pointed to by sem. If the semaphore's value is greater than zero, then the decrement proceeds, 
and the function returns, immediately. If the semaphore currently has the value zero, then the call blocks until either it becomes possible 
to perform the decrement (i.e., the semaphore value rises above zero), or a signal handler interrupts the call.

sem_trywait() is the same as sem_wait(), except that if the decrement cannot be immediately performed, then call returns an error (errno 
set to EAGAIN) instead of blocking.

sem_timedwait() is the same as sem_wait(), except that abs_timeout specifies a limit on the amount of time that the call should block if 
the decrement cannot be immediately performed. The abs_timeout argument points to a structure that specifies an absolute timeout in seconds 
and nanoseconds since the Epoch, 1970-01-01 00:00:00 +0000 (UTC). This structure is defined as follows:

struct timespec {
    time_t tv_sec;      // Seconds 
    long   tv_nsec;     // Nanoseconds [0 .. 999999999] 
};

All of these functions return 0 on success. Error is -1.

*/

inline
int sem_close(sem_t *st)
{	return sem_t::sem_close(st);
}

inline
int sem_destroy(sem_t *st)
{	return st->sem_destroy();
}

inline
int sem_getvalue(sem_t *st, int *val)
{	return st->sem_getvalue(st,val);
}

inline
int sem_init(sem_t *st, int pshared, unsigned int value)
{	return st->sem_init(pshared,value);
}

inline
sem_t* sem_open(const char *name, int oflag)
{	return sem_t::sem_open(name,oflag);
}

inline
sem_t* sem_open(const char *name, int oflag,mode_t mode, unsigned int value)
{	return sem_t::sem_open(name,oflag,mode,value);
}

inline
int sem_post(sem_t* st)
{	return st->sem_post();
}

inline
int sem_timedwait(sem_t* st, const struct timespec* ts)
{	return st->sem_timedwait(ts);
}

inline
int sem_trywait(sem_t* st)
{	return st->sem_trywait();
}

inline
int sem_unlink(const char* name)
{	return sem_t::sem_unlink(name);
}

inline
int sem_wait(sem_t* st)
{	return st->sem_wait();
}


#endif
