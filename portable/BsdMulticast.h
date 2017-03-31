// portable/BsdMulticast.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdMulticast_h
#define BsdMulticast_h

#include "TimerPump.h"
#include "Packet.h"
#include "BsdSocketPool.h"

namespace portable 
{

class BsdMulticast
:	public TimerPump
{	Packet* headerPacket;
	Packet* framePacket;
	BsdSocketPool& socketPool;
	unsigned verboseCount;
protected:
#if 1
	virtual void Action(bool isTimeout) override
	{	if(framePacket)
		{	socketPool.DirectMulticast(headerPacket, framePacket, verboseCount);
		}
//		puts("Unexpected call to pool Action");
	}
#endif
public:
	BsdMulticast(BsdSocketPool& socketPool)
	:	headerPacket(nullptr)
	,	framePacket(nullptr)
	,	socketPool(socketPool)
	{	TimerPump::Start();
		verboseCount = 60*10;
	}
	void SetIsStreaming(bool isStreaming = true)
	{	socketPool.SetIsStreaming(isStreaming);
	}
	void SetHeaderPacket(Packet* packet)
	{	headerPacket=packet;
	}
	void SetFramePacket(Packet* packet)
	{	framePacket=packet;
	}
};

}

#endif
