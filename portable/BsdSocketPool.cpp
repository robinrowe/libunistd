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

void BsdSocketPool::ReleaseSlots()
{	for (unsigned i = 0; i<socketfd.size(); i++)
	{	ReleaseSlot(i);
}	}

int BsdSocketPool::DirectMulticast(Packet* framePacket)
{	if(!framePacket || !framePacket->IsGood())
	{	return -1;
	}
	if(!counter)
	{	return 0;
	}
	SoftLock softlock(framePacket->ownership);
	if(!softlock)
	{	puts("Can't lock frame packet");
		return -1;
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
		{	continue;
		}
		count++;
	}
	const unsigned packetSize = framePacket->GetPacketSize();
	const unsigned packetId = framePacket->GetPacketId();
	const PacketHeader::hash_t hash = framePacket->GetHash();
	printf("%u:%u #%u size=%u hash=%llx\n",count,(unsigned) counter,packetId,packetSize,hash);
	return count;
}

void BsdSocketPool::ReleaseSlot(unsigned slot)
{	if (slot >= socketfd.size())
	{	return;
	}
	if (0 >= socketfd[slot])
	{	return;
	}
	BsdSocket bsdSocket(socketfd[slot]);
	bsdSocket.SendClose();
	socketfd[slot] = 0;
	counter--;
	printf("Released slot %u, connections = %u\n", slot, (unsigned)counter);
}


}