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
{	bool isStreaming;
	std::vector<SOCKET> socketfd;
	AtomicCounter<unsigned> counter;
public:
	void Reset(unsigned size)
	{	socketfd.resize(size);
		socketfd.assign(size,0);
		counter=0;
	}
	unsigned GetCount() const
	{	return counter;
	}
	bool IsEmpty() const
	{	return 0==counter;
	}
	bool IsStreaming() const
	{	return isStreaming;
	}
	void SetIsStreaming(bool isStreaming)
	{	this->isStreaming = isStreaming;
	}
	SOCKET* GetSlot();
	SOCKET* GetZombieSlot();
	bool ReleaseSlot(SOCKET* sock);
	void BsdSocketPool::ReleaseSlot(unsigned slot)
	{	if(slot >= socketfd.size())
		{	return;
		}
		if(!socketfd[slot])
		{	return;
		}
		socketfd[slot]=0;
		counter--;
		printf("Released slot %u, connections = %u\n",slot,(unsigned) counter);
	}
	bool SendPacket(Packet* packet,unsigned i)
	{	BsdSocket bsdSocket(socketfd[i]);
		if(bsdSocket.SendTo(*packet))
		{	return true;
		}
		ReleaseSlot(i);
		return false;
	}
	int DirectMulticast(Packet* framePacket);
};

}

#endif
