// RingAllocator.h
// Copyright 2018/4/10 Robin.Rowe@CinePaint.org
// License MIT open source

#ifndef RingAllocator_h
#define RingAllocator_h

#include <atomic>
#include <memory>
#include <memory.h>

namespace Atomic
{
// Usage: const int size;
//        Atomic::RingQueue<int> ringQueue(size);
//        ringQueue.Push(10);
//        const int popped=ringQueue.Pop();

class RingAllocator
{	std::atomic<int> p;
	char* buffer;
	int bitmask;
public:
	RingAllocator(char* buffer,unsigned sizeInBits)
	:	buffer(buffer)
	,	p(0)
	{	bitmask = (1 << sizeInBits) - 1;
	}
	char* Get(unsigned blockSize)
	{	blockSize += sizeof(int);
		int before = p.fetch_add(blockSize,std::memory_order_relaxed);
		before &= bitmask;
		if(before + blockSize > bitmask)
		{	// Buffer overflow, 2nd try:
			before = p.fetch_add(blockSize,std::memory_order_relaxed);
			before &= bitmask;
		}
		if(before + blockSize > bitmask)
		{	// Too big
			return 0;
		}
		int* header = (int*) buffer[before];
		*header = before;
		return buffer = before + sizeof(int);
	}
};

class SizeTracker
{	std::atomic<int> size;
	int bitmask;
public:
	SizeTracker(unsigned sizeInBits)
	:	size(0)
	{	bitmask = (1 << sizeInBits) - 1;
	}
	int Add(int qty)
	{	const int i = size.fetch_add(qty,std::memory_order_relaxed);
		return i + qty;
	}
	int Free(int qty)
	{	const int i = size.fetch_sub(size,std::memory_order_relaxed);
		return i - qty;
	}
	int GetSize() const
	{	const int s=size;
		return s;
	}
	int GetMaxSize() const
	{	return bitmask;
	}
	int GetFreeSize() const
	{	return GetMaxSize() - GetSize();
	}
	bool IsFull() const
	{	return GetSize() >= GetMaxSize();
	}
	bool IsEmpty() const
	{	return GetSize() <= 0;
	}
};

}

#endif