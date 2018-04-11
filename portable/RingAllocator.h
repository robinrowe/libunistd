// RingAllocator.h
// Copyright 2018/4/10 Robin.Rowe@CinePaint.org
// License MIT open source

#ifndef RingAllocator_h
#define RingAllocator_h

#include <atomic>

namespace Atomic
{
// Usage: char buffer[16];
//      Atomic::RingAllocator a(buffer,4);
//      char* p = a.Get(12);
//		strcpy(p,"Hello World)
//		WakeOtherThread();
//			a.Free(p);

class RingAllocator
{	std::atomic<int> p;
	char* buffer;
	char* bufferEnd;
	int bitmask;
public:
	RingAllocator(char* buffer,unsigned sizeInBits)
	:	buffer(buffer)
	,	p(0)
	{	bitmask = (1 << sizeInBits) - 1;
		bufferEnd = buffer + bitmask;
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
		int* header = (int*) buffer + before;
		*header = before;
		return buffer + before + sizeof(int);
	}
	int Free(const char* p) const
	{	p -= sizeof(int);
		if(p < buffer || p >= bufferEnd)
		{	return 0;
		}
		int* header = (int*) p;
		return *header;
	}
};

}

#endif