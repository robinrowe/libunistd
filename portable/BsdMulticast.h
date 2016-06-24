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
protected:
    virtual void Action(bool isTimeout) override
	{	if(headerPacket)
		{	socketPool.DirectMulticast(*headerPacket);
			headerPacket=nullptr;
		}
		if(framePacket)
		{	socketPool.DirectMulticast(*framePacket);
			framePacket=nullptr;
		}
	}
public:
	BsdMulticast(BsdSocketPool& socketPool)
	:	headerPacket(nullptr)
	,	framePacket(nullptr)
	,	socketPool(socketPool)
	{	TimerPump::Start();
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
