// portable/BsdSocketPool.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketPool_h
#define BsdSocketPool_h

#include "PacketSocket.h"
#include "PacketWriter.h"
#include "PacketStats.h"

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
	PacketStats stats;
	void Close()
	{	ReleaseSlots();
	}
	void Reset(unsigned size)
	{	socketfd.resize(size);
		socketfd.assign(size,0);
		counter=0;
		stats.Reset();
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
	{	PacketSocket bsdSocket(socketfd[i]);
		if(bsdSocket.SendTo(*packet))
		{	stats.Transmit(packet->GetPacketId()); 
			return true;	
		}
		ReleaseSlot(i);
		return false;
	}
	bool SetSlot(SOCKET sid);
	bool SetZombieSlot(SOCKET sid);
	bool ReleaseSlot(SOCKET* sock);
	void ReleaseSlot(SOCKET slot);
	int DirectMulticast(Packet* framePacket);
};

}

#endif
