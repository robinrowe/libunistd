// SizeTracker.h
// Copyright 2018/4/10 Robin.Rowe@CinePaint.org
// License MIT open source

#ifndef SizeTracker_h
#define SizeTracker_h

#include <atomic>

namespace Atomic
{

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