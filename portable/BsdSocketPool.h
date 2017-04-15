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
public:
	std::vector<SOCKET> socketfd;
	std::vector<bool> isHeaderSent;
	AtomicCounter<unsigned> counter;	
	void Reset(unsigned size)
	{	socketfd.resize(size);
		socketfd.assign(size,0);
		isHeaderSent.resize(size);
		isHeaderSent.assign(size, false);
		counter=0;
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
	{	if(slot < socketfd.size())
		{	socketfd[slot]=0;
			isHeaderSent[slot]=false;
	}	}
	bool SendPacket(Packet* packet,unsigned i)
	{	BsdSocket bsdSocket(socketfd[i]);
		if(bsdSocket.SendTo(*packet))
		{	isHeaderSent[i] = true;
			return true;
		}
		ReleaseSlot(i);
		return false;
	}
	int DirectMulticast(Packet* headerPacket, Packet* framePacket, unsigned verboseCount );
};

}

#endif
