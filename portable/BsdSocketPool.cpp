// BsdSocketPool.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#include "BsdSocketPool.h"
#include "SoftLock.h"
#include "VerboseCounter.h"

namespace portable 
{

SOCKET* BsdSocketPool::GetSlot()
{	for(unsigned i =0;i<socketfd.size();i++)
	{	if(!socketfd[i])
		{	return &socketfd[i];
	}	}
	return GetZombieSlot();
}

SOCKET* BsdSocketPool::GetZombieSlot()
{	SOCKET* s = 0;
	for(unsigned i =0;i<socketfd.size();i++)
	{	if(socketfd[i])
		{	BsdSocket bsdSocket(socketfd[i]);
			if(!bsdSocket.SendTo("",0))
			{	ReleaseSlot(i);
	}	}	}
	return s;
}


bool BsdSocketPool::ReleaseSlot(SOCKET* sock)
{	for(unsigned i =0;i<socketfd.size();i++)
	{	if(sock == &socketfd[i])
		{	ReleaseSlot(i);
			return true;
	}	}
	return false;
}

int BsdSocketPool::DirectMulticast(Packet* headerPacket,Packet* framePacket,unsigned verboseCount)
{	if(!headerPacket || !headerPacket->IsGood() || !framePacket || !framePacket->IsGood())
	{	return -1;
	}
	SoftLock softlock(framePacket->ownership);
	if(!softlock)
	{	return -1;
	}
	const unsigned maxShowPacketId = 10;
	if(verboseCount)
	{	static portable::VerboseCounter counter(600);
		counter++;
		if (counter)
		{	printf("Muticast packet #%d\n", framePacket->GetPacketId());
	}	}
	int count = 0;
	for(unsigned i=0;i<socketfd.size();i++)
	{	if(!socketfd[i])
		{	continue;
		}
		if (!isHeaderSent[i])
		{	if(SendPacket(headerPacket,i))
			{	SendPacket(framePacket,i);
			}
			count++;
			continue;
		}
		if(!isStreaming)
		{	continue;
		}
		SendPacket(framePacket,i);
		count++;
	}
	return count;
}

}