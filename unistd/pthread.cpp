// pthread.cpp
// Copyright 2021 Robin.Rowe@cinepaint.org
// License MIT open source

#include "pthread.h"
#include "../portable/Logger.h"

#include <thread>
#include <system_error>

struct pthread_attr
{	bool isJoinable;
public:
	pthread_attr()
	:	isJoinable(true)
	{}
};

struct PortableThread
:	public std::thread
{	
public:
	pthread_attr_t attr;
	PortableThread(const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg)
	:	std::thread(start_routine,arg)
	{	if(attr)
		{	this->attr = *attr;
	}	}
};

int pthread_setschedparam(pthread_t pthread, int policy, const sched_param* param)
{	if(!pthread)
	{	return -1;
	}
	HANDLE h = (HANDLE) pthread->native_handle();
	return SetThreadPriority(h, param->sched_priority)? 0:-1;
}

#ifdef VERBOSE_PTHREAD

int uni_pthread_create(pthread_t* pthread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg,const char* name)
{	SysLogMsg("Thread",name); 
	PortableThread* t=new PortableThread(attr,start_routine,arg);
	*pthread=t;
	if(!t->attr.isJoinable)
	{	t->detach();
	}
	return 0;
}

#define pthread_create(t,at,f,arg) uni_pthread_create(t,at,f,arg,#f)
#else
int pthread_create(pthread_t* pthread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg)
{	PortableThread* t=0;
    try 
	{	t = new PortableThread(attr,start_routine,arg);
    } 
	catch(const std::system_error& e) 
	{	printf("Caught system_error %s\n",e.what());
		return -1;
    }
	*pthread=t;
	if(!t->attr.isJoinable)
	{	t->detach();
	}
	return 0;
}
#endif

int pthread_attr_setdetachstate(pthread_attr_t *attr, ThreadState detachstate)
{	if(!attr)
	{	return -1;
	}
	attr->isJoinable = (detachstate == PTHREAD_CREATE_JOINABLE);
	return 0;
}

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate)
{	if(!attr)
	{	return -1;
	}
	if(attr->isJoinable)
	{	*detachstate = (int) PTHREAD_CREATE_JOINABLE;
		return true;
	}
	*detachstate = (int) PTHREAD_CREATE_DETACHED;
	return 0;
}

int pthread_join(pthread_t thread, void **retval)
{	if(!thread)
	{	return -1;
	}
	if(!thread->attr.isJoinable)
	{	return -1;
	}
    try 
	{	thread->join();
    } 
	catch(const std::system_error& e) 
	{	printf("Caught system_error %s\n",e.what());
		return -1;
    }
	return 0;
}

int pthread_detach(pthread_t thread)
{	thread->detach();
	return 0;
}


void pthread_cleanup_push(void (*routine)(void *),void *arg)
{   STUB(pthread_cleanup_push);
}


void pthread_cleanup_pop(int execute)
{   STUB(pthread_cleanup_pop);
}


int pthread_equal(pthread_t t1, pthread_t t2)
{	return t1==t2;
}


int pthread_attr_init(pthread_attr_t *attr)
{	STUB_0(pthread_attr_init);
}


int pthread_attr_destroy(pthread_attr_t *attr)
{	STUB_0(pthread_attr_init);
}


int pthread_attr_setinheritsched(pthread_attr_t *attr,int inheritsched)
{   STUB_NEG(pthread_attr_setinheritsched);
}


int pthread_attr_getinheritsched(const pthread_attr_t *attr,int *inheritsched)
{   STUB_NEG(pthread_attr_getinheritsched);
}


int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{   STUB_NEG(pthread_attr_setschedpolicy);
}


int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy)
{   STUB_NEG(pthread_attr_getschedpolicy);
}


int pthread_attr_setschedparam(pthread_attr_t *attr,const sched_param *param)
{   STUB_NEG(pthread_attr_setschedparam);
}


int pthread_attr_getschedparam(const pthread_attr_t *attr,sched_param *param)
{   STUB_NEG(pthread_attr_getschedparam);
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{   STUB_NEG(pthread_mutex_lock);
}


int pthread_mutex_trylock(pthread_mutex_t *mutex)
{   STUB_NEG(pthread_mutex_trylock);
}


int pthread_mutex_unlock(pthread_mutex_t *mutex)
{   STUB_NEG(pthread_mutex_unlock);
}


void pthread_exit(void *retval)
{   STUB(pthread_exit);
}


int pthread_cancel(pthread_t thread)
{   STUB_NEG(pthread_cancel);
}


pthread_t pthread_self()
{	STUB_0(pthread_self);
}

int pthread_detach(pthread_t thread);


int pthread_cond_destroy(pthread_cond_t *cond)
{	STUB_NEG(pthread_cond_destroy);
}


int pthread_cond_init(pthread_cond_t* restrict_cond,const pthread_condattr_t* restrict_attr)
{	STUB_NEG(pthread_cond_destroy);
}


int pthread_cond_broadcast(pthread_cond_t *cond)
{	STUB_NEG(pthread_cond_broadcast);
}


int pthread_cond_signal(pthread_cond_t *cond)
{	STUB_NEG(pthread_cond_signal);
}


int pthread_cond_timedwait(pthread_cond_t* restrict_cond,pthread_mutex_t* restrict_mutex,const struct timespec * restrict_abstime)
{	STUB_NEG(pthread_cond_timedwait);
}


int pthread_cond_wait(pthread_cond_t* restrict_cond,pthread_mutex_t * restrict_mutex)
{	STUB_NEG(pthread_cond_wait);
}


int pthread_mutex_destroy(pthread_mutex_t *mutex)
{	STUB_NEG(pthread_mutex_destroy);
}


int pthread_mutex_init(pthread_mutex_t* restrict_mutex,const pthread_mutexattr_t * restrict_attr)
{	STUB_NEG(pthread_mutex_init);
}
