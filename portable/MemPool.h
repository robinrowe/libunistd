// MemPool.h
// Created by Robin Rowe on 12/2/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org

#ifndef AtomicMemPool_h
#define AtomicMemPool_h

#include <memory>
#include <atomic>
#include <mutex>

namespace Atomic
{

// Usage: HiddenMemPool intended for internal use

class HiddenMemPool
{	std::unique_ptr<char[]> data;
	std::atomic<unsigned> bytes;
	const unsigned size;
	static void SetBlockSize(char* block,unsigned blockSize)
	{	memcpy(block,&blockSize,sizeof(blockSize));
	}
public:
	HiddenMemPool(unsigned size)
	:	size(size)
	{	data=std::unique_ptr<char[]>(new char[size]);
	}
	bool IsEmpty() const
	{	return 0==bytes;
	}
	static unsigned GetBlockSize(char* block)
	{	if(!block)
		{	return 0;
		}
		unsigned blockSize;
		memcpy(&blockSize,block-sizeof(blockSize),sizeof(blockSize));
		return blockSize;
	}
	char* NewBlock(unsigned blockSize)
	{	blockSize+=sizeof(blockSize);
		const unsigned end=bytes.fetch_add(blockSize,std::memory_order_relaxed);
		if(end>=size)
		{	return 0;
		}
		char* rawBlock=data.get()+end-blockSize;
		SetBlockSize(rawBlock,blockSize);
		return rawBlock+sizeof(blockSize);
	}
	void FreeBlock(char* block)
	{	const int blockSize=GetBlockSize(block);
		bytes.fetch_sub(blockSize,std::memory_order_relaxed);
	}
	bool IsMyBlock(char* block)
	{	block-=sizeof(size);
		if(block<data.get() || block>=data.get())
		{	return false;
		}
		return true;
	}
};

// Usage: Atomic::MemPool memPool(memoryBufferSize);
//        const unsigned size=200;
//	      char* block=memPool.NewBlock(size);
//        memPool.FreeBlock(block);
// Note: size==memPool.BlockSize(block);

class MemPool
{	HiddenMemPool a;
	HiddenMemPool b;
	HiddenMemPool* activePool;
	typedef std::lock_guard<std::mutex> LockGuard;
	std::mutex switchMutex;
	bool SwitchPools()
	{	if(&a==activePool)
		{	if(!b.IsEmpty())
			{	return false;
			}
			activePool=&b;
			return true;
		}
		if(!a.IsEmpty())
		{	return false;
		}
		activePool=&a;
		return true;
	}
public:
	MemPool(unsigned size)
	:	a(size/2)
	,	b(size/2)
	{	activePool=&a;
	}
	char* NewBlock(unsigned blockSize)
	{	char* data=activePool->NewBlock(blockSize);
		if(data)
		{	return data;
		}
		LockGuard lockGuard(switchMutex);//Lock only on wrap-around
		data=activePool->NewBlock(blockSize);//re-entrant
		if(data)
		{	return data;
		}
		if(!SwitchPools())
		{	return 0;
		}
		data=activePool->NewBlock(blockSize);
		return data;
	}
	bool FreeBlock(char* block)
	{	if(a.IsMyBlock(block))
		{	a.FreeBlock(block);
			return true;
		}
		if(b.IsMyBlock(block))
		{	b.FreeBlock(block);
			return true;
		}
		return false;
	}
	unsigned GetBlockSize(char* block)
	{	return HiddenMemPool::GetBlockSize(block);
	}
};

// Usage: Atomic::MemPoolBlock memPoolBlock(memPool,packetQueue.Pop());
//        char* p=memPoolBlock.get();
// Note: Destructor will automatically free block

class MemPoolBlock
{	MemPool& memPool;
	char* block;
public:
	~MemPoolBlock()
	{	memPool.FreeBlock(block);
	}
	MemPoolBlock(MemPool& memPool,char* block)
	:	memPool(memPool)
	,	block(block)
	{}
	char* get()
	{	return block;
	}
};

}

#endif