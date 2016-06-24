// portable/BsdSocketPool.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketPool_h
#define BsdSocketPool_h

#include "BsdSocket.h"
#include "PacketWriter.h"

namespace portable 
{
	
class BsdSocketPool
{	
public:
	std::vector<SOCKET> socketfd;
	AtomicCounter<unsigned> counter;	
	void Reset(unsigned size)
	{	socketfd.resize(size);
		socketfd.assign(size,0);
		counter=0;
	}
	SOCKET* GetSlot();
	SOCKET* GetZombieSlot();
	bool ReleaseSlot(SOCKET* sock);
	void BsdSocketPool::ReleaseSlot(unsigned slot)
	{	if(slot < socketfd.size())
		{	socketfd[slot]=0;
	}	}
	int DirectMulticast(Packet& packet);
};

}

#endif
