// RingQueue.h
// 2014/10/26 Robin.Rowe@CinePaint.org

#ifndef AtomicRingQueue_h
#define AtomicRingQueue_h

#include <atomic>
#include <memory>
#include <memory.h>

namespace Atomic
{
// Usage: const int size;
//        Atomic::RingQueue<int> ringQueue(size);
//        ringQueue.Push(10);
//        const int popped=ringQueue.Pop();

template <typename T>
class RingQueue
{	std::unique_ptr<T []> buffer;
	std::atomic<int> front;
	std::atomic<int> back;
	std::atomic<int> counter;
	const int size;
	const char* queueName;
	int WrapAroundFront(int i)
	{	if(i==size)
		{	front.fetch_sub(size,std::memory_order_relaxed);
		}
		i-=size;
		while(i>=size)
		{	i-=size;
		}
		return i;
	}
	int WrapAroundBack(int i)
	{	if(i==size)
		{	back.fetch_sub(size,std::memory_order_relaxed);
		}
		i-=size;
		while(i>=size)
		{	i-=size;
		}
		return i;
	}
public:
	~RingQueue()
	{}
	RingQueue(const char* queueName,unsigned size)
	:	queueName(queueName)
	,	size(size)
	{	Clear();
		buffer=std::unique_ptr<T []>(new T[size]);
		memset(buffer.get(),0,sizeof(T)*size);
	}
	const char* QueueName() const
	{	return queueName;
	}
	bool IsEmpty() const
	{	return counter<=0;
	}
	void Clear()
	{	front=0;
		back=0;
		counter=0;
	}
	void Reset()
	{	T t;
		while(!IsEmpty())
		{	Pop(t);
	}	}
	bool Push(T t)
	{	int i=front.fetch_add(1,std::memory_order_relaxed);
		if(i>=size)
		{	i=WrapAroundFront(i);
		}
		T x=buffer[i];
		buffer[i]=t;
		int count=counter.fetch_add(1,std::memory_order_relaxed);
		if(count>int(size))
		{	buffer[i]=x;
			counter.fetch_sub(1,std::memory_order_relaxed);
			return false;
		}
		return true;
	}
	T Pop()
	{	const int count=counter.fetch_sub(1,std::memory_order_relaxed);
		if(count<=0)
		{	counter.fetch_add(1,std::memory_order_relaxed);
			return 0;
		}
		int i=back.fetch_add(1,std::memory_order_relaxed);
		if(i>=size)
		{	i=WrapAroundBack(i);
		}
		return buffer[i];
	}
	int GetSize() const
	{	const int count=counter;
		return count<0 ? 0:count;
	}
	unsigned GetFreeSize() const
	{	return GetMaxSize()-GetSize();
	}
	bool IsFull() const
	{	return GetMaxSize()==GetSize();
	}
	unsigned GetMaxSize() const
	{	return size;
	}
};

}

#endif