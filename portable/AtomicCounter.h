// AtomicCounter.h
// 2014/3/17

#ifndef AtomicCounter_h
#define AtomicCounter_h

#include <atomic>

namespace portable
{

template<typename T>
class AtomicCounter
{	std::atomic<T> n;
public:
	AtomicCounter<T>()
	{	clear();
	}
	void clear()
	{	n=0;
	}
	AtomicCounter<T>(T n)
	{	this->n=n;
	}
	AtomicCounter<T>& operator=(const T rhs)
	{	n=rhs;
		return *this;
	}
	T operator++(int)
	{	return n.fetch_add(1);
	}
	T operator--(int)
	{	return n.fetch_sub(1);
	}
	T operator++()
	{	const T prev=n.fetch_add(1);
		return prev+1;
	}
	T operator--()
	{	const T prev=n.fetch_sub(1);
		return prev-1;
	}
	operator T()
	{	return n;
	}
	operator T() const
	{	return n;
	}
	void Set(T n)
	{	this->n=n;
	}
	T Get() const
	{	return n;
	}
};

}

#endif
 