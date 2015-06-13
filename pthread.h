// pthread_t.h
// 2015/3/3 Robin.Rowe@cinepaint.org

#ifndef pthread_h
#define pthread_h

#include <thread>
#include <condition_variable>
#include <mutex>

typedef std::thread pthread_t;
typedef std::mutex pthread_mutex_t;
typedef std::condition_variable pthread_cond_t;

inline
int pthread_mutex_init(pthread_mutex_t*, void*)
{	return 0;
}

inline
int pthread_cond_init(pthread_cond_t*, void*)
{	return 0;
}


inline
void pthread_mutex_destroy(pthread_mutex_t*)
{}

inline
void pthread_cond_destroy(pthread_cond_t*)
{}


inline
int pthread_cond_signal(pthread_cond_t* c)
{	c->notify_one();
	return 0;
}

inline
int pthread_join(pthread_t& t,void*)
{	t.join();
	return 0;
}

inline
void pthread_cond_wait(pthread_cond_t* c, pthread_mutex_t* m)
{	std::unique_lock<std::mutex> lk(*m);
	c->wait(lk);
}

inline
void pthread_detach(pthread_t& t)
{	t.detach();
}

//typedef void (*action_fn)(void *data);

template <typename Func>
int pthread_create(pthread_t* t1, void*,Func f, void* data)
{	if(!t1)
	{	return -1;
	}
	std::thread t2(f,data);
	t1->swap(t2);
	return 0; 
}

typedef std::lock_guard<std::mutex> pthread_mutex_lock;

inline
void pthread_mutex_unlock(pthread_mutex_t* m)
{}

inline
void pthread_cond_broadcast(pthread_cond_t* cond)
{	if(cond)
	{	cond->notify_all();
}	}


#endif
