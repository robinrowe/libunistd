// pthread.h
// Copyright 2016 Robin.Rowe@cinepaint.org
// License MIT open source

#ifndef pthread_t_h
#define pthread_t_h

#include "../portable/stub.h"
#include "cfunc.h"

enum
{	PTHREAD_BARRIER_SERIAL_THREAD,
	PTHREAD_CANCEL_ASYNCHRONOUS,
	PTHREAD_CANCEL_ENABLE,
	PTHREAD_CANCEL_DEFERRED,
	PTHREAD_CANCEL_DISABLE,
	PTHREAD_CANCELED,
	PTHREAD_COND_INITIALIZER,
	PTHREAD_EXPLICIT_SCHED,
	PTHREAD_INHERIT_SCHED,
	PTHREAD_MUTEX_DEFAULT,
	PTHREAD_MUTEX_ERRORCHECK,
	PTHREAD_MUTEX_INITIALIZER,
	PTHREAD_MUTEX_NORMAL,
	PTHREAD_MUTEX_RECURSIVE,
	PTHREAD_ONCE_INIT,
	PTHREAD_PRIO_INHERIT,
	PTHREAD_PRIO_NONE,
	PTHREAD_PRIO_PROTECT,
	PTHREAD_PROCESS_SHARED,
	PTHREAD_PROCESS_PRIVATE,
	PTHREAD_SCOPE_PROCESS,
	PTHREAD_SCOPE_SYSTEM
};

typedef enum 
{	PTHREAD_CREATE_DETACHED,
	PTHREAD_CREATE_JOINABLE
} ThreadState;

typedef struct pthread_attr_t pthread_attr_t;
typedef struct PortableThread PortableThread;
typedef PortableThread* pthread_t;

typedef int pthread_barrier_t;
typedef int pthread_barrierattr_t;
typedef int pthread_cond_t;
typedef int pthread_condattr_t;
typedef int pthread_key_t;
typedef int pthread_mutex_t;
typedef int pthread_mutexattr_t;
typedef int pthread_once_t;
typedef int pthread_rwlock_t;
typedef int pthread_rwlockattr_t;
typedef int pthread_spinlock_t;

typedef struct sched_param
{	int sched_priority;
} sched_param;

inline
void pthread_cleanup_push(void (*routine)(void *),void *arg)
{   STUB(pthread_cleanup_push);
}

inline
void pthread_cleanup_pop(int execute)
{   STUB(pthread_cleanup_pop);
}

/*
struct pthread_attr_t
{	int __detachstate;
	int __schedpolicy;
	struct sched_param __schedparam;
	int __inheritsched;
	int __scope;
	size_t __guardsize;
	int __stackaddr_set;
	void *__stackaddr;
	unsigned long int __stacksize;
} ;
*/

CFUNC int pthread_setschedparam(pthread_t pthread, int policy, const sched_param* param);

/*	Windows Thread Priorities:

-15	THREAD_PRIORITY_IDLE
-2	THREAD_PRIORITY_LOWEST
-1	THREAD_PRIORITY_BELOW_NORMAL
0	THREAD_PRIORITY_NORMAL
1	THREAD_PRIORITY_ABOVE_NORMAL
2	THREAD_PRIORITY_HIGHEST
15	THREAD_PRIORITY_TIME_CRITICAL

*/

inline
int pthread_equal(pthread_t t1, pthread_t t2)
{	return t1==t2;
}

inline
int pthread_attr_init(pthread_attr_t *attr)
{	return 0;
}

inline
int pthread_attr_destroy(pthread_attr_t *attr)
{	return 0;
}

inline
int pthread_attr_setinheritsched(pthread_attr_t *attr,int inheritsched)
{   STUB_NEG(pthread_attr_setinheritsched);
}

inline
int pthread_attr_getinheritsched(const pthread_attr_t *attr,int *inheritsched)
{   STUB_NEG(pthread_attr_getinheritsched);
}

inline
int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{   STUB_NEG(pthread_attr_setschedpolicy);
}

inline
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy)
{   STUB_NEG(pthread_attr_getschedpolicy);
}

inline
int pthread_attr_setschedparam(pthread_attr_t *attr,const sched_param *param)
{   STUB_NEG(pthread_attr_setschedparam);
}

inline
int pthread_attr_getschedparam(const pthread_attr_t *attr,sched_param *param)
{   STUB_NEG(pthread_attr_getschedparam);
}

CFUNC int pthread_create(pthread_t* pthread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
CFUNC int pthread_attr_setdetachstate(pthread_attr_t *attr, ThreadState detachstate);
CFUNC int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
CFUNC int pthread_join(pthread_t thread, void **retval);

inline
int pthread_mutex_lock(pthread_mutex_t *mutex)
{   STUB_NEG(pthread_mutex_lock);
}

inline
int pthread_mutex_trylock(pthread_mutex_t *mutex)
{   STUB_NEG(pthread_mutex_trylock);
}

inline
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{   STUB_NEG(pthread_mutex_unlock);
}

inline
void pthread_exit(void *retval)
{   STUB(pthread_exit);
}

inline
int pthread_cancel(pthread_t thread)
{   STUB_NEG(pthread_cancel);
}

inline
pthread_t pthread_self()
{	STUB_0(pthread_self);
}

CFUNC int pthread_detach(pthread_t thread);

inline
int pthread_cond_destroy(pthread_cond_t *cond)
{	STUB_NEG(pthread_cond_destroy);
}

inline
int pthread_cond_init(pthread_cond_t* restrict_cond,const pthread_condattr_t* restrict_attr)
{	STUB_NEG(pthread_cond_destroy);
}

inline
int pthread_cond_broadcast(pthread_cond_t *cond)
{	STUB_NEG(pthread_cond_broadcast);
}

inline
int pthread_cond_signal(pthread_cond_t *cond)
{	STUB_NEG(pthread_cond_signal);
}

inline
int pthread_cond_timedwait(pthread_cond_t* restrict_cond,pthread_mutex_t* restrict_mutex,const struct timespec * restrict_abstime)
{	STUB_NEG(pthread_cond_timedwait);
}

inline
int pthread_cond_wait(pthread_cond_t* restrict_cond,pthread_mutex_t * restrict_mutex)
{	STUB_NEG(pthread_cond_wait);
}

inline
int pthread_mutex_destroy(pthread_mutex_t *mutex)
{	STUB_NEG(pthread_mutex_destroy);
}

inline
int pthread_mutex_init(pthread_mutex_t* restrict_mutex,const pthread_mutexattr_t * restrict_attr)
{	STUB_NEG(pthread_mutex_init);
}

#endif
