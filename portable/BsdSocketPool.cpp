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
		{	counter++;
			return &socketfd[i];
	}	}
	return GetZombieSlot();
}

SOCKET* BsdSocketPool::GetZombieSlot()
{	for(unsigned i =0;i<socketfd.size();i++)
	{	if(socketfd[i])
		{	BsdSocket bsdSocket(socketfd[i]);
			if(!bsdSocket.SendTo("",0))
			{	ReleaseSlot(i);
				return &socketfd[i];
	}	}	}
	return nullptr;
}

bool BsdSocketPool::ReleaseSlot(SOCKET* sock)
{	for(unsigned i =0;i<socketfd.size();i++)
	{	if(sock == &socketfd[i])
		{	ReleaseSlot(i);
			puts("released socket");
			return true;
	}	}
	return false;
}

int BsdSocketPool::DirectMulticast(Packet* framePacket)
{	if(!framePacket || !framePacket->IsGood())
	{	return -1;
	}
	if(!counter)
	{	return 0;
	}
	SoftLock softlock(framePacket->ownership);
	if(!softlock)
	{	return -1;
	}
#if 0
	const unsigned maxShowPacketId = 10;
	if(verboseCount)
	{	static portable::VerboseCounter counter(600);
		counter++;
		if (counter)
		{	printf("Multicast packet #%d\n", framePacket->GetPacketId());
	}	}
#endif
	int count = 0;
	for(unsigned i=0;i<socketfd.size();i++)
	{	if(!socketfd[i])
		{	continue;
		}
		if(!isStreaming)
		{	continue;
		}
		if(!SendPacket(framePacket,i))
		{	ReleaseSlot(i);
			continue;
		}
		count++;
	}
	printf("%u:%u>",count,(unsigned) counter);
	return count;
}

}