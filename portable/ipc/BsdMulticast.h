// portable/BsdMulticast.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdMulticast_h
#define BsdMulticast_h

#include "../pump/TimerPump.h"
#include "Packet.h"
#include "BsdSocketPool.h"

namespace portable 
{

class BsdMulticast
:	public TimerPump
{	Packet* headerPacket;
	Packet* framePacket;
	BsdSocketPool& socketPool;
protected:
#if 1
	virtual void Action() override
	{	if(framePacket)
		{	socketPool.DirectMulticast(framePacket);
		}
//		puts("Unexpected call to pool Action");
	}
#endif
public:
	BsdMulticast(BsdSocketPool& socketPool)
	:	TimerPump("BsdMultiPump")
	,	headerPacket(nullptr)
	,	framePacket(nullptr)
	,	socketPool(socketPool)
	{	TimerPump::Start();
	}
	~BsdMulticast()
	{	Close();
	}
	void Close()
	{	socketPool.Close();
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
	void AddSkipped(unsigned skipped)
	{	socketPool.stats.AddSkipped(skipped);
	}
	void PrintStats()
	{	socketPool.stats.Print();
	}
};

}

#endif
