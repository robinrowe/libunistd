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
	void ReleaseSlots();
	bool IsGood(SOCKET sid) const
	{	if(!sid || -1==sid)
		{	return false;
		}
		return true;
	}
public:
	void Close()
	{	ReleaseSlots();
	}
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
	bool SendPacket(Packet* packet,unsigned i)
	{	BsdSocket bsdSocket(socketfd[i]);
		if(bsdSocket.SendTo(*packet))
		{	return true;
		}
		ReleaseSlot(i);
		return false;
	}
	bool SetSlot(SOCKET sid);
	bool SetZombieSlot(SOCKET sid);
	bool ReleaseSlot(SOCKET* sock);
	void BsdSocketPool::ReleaseSlot(SOCKET slot);
	int DirectMulticast(Packet* framePacket);
};

}

#endif
