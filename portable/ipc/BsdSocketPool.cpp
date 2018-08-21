// BsdSocketPool.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#include "BsdSocketPool.h"
#include "../SoftLock.h"
#include "../VerboseCounter.h"

namespace portable 
{

bool BsdSocketPool::SetSlot(SOCKET sid)
{	for(unsigned i =0;i<socketfd.size();i++)
	{	if(!IsGood(socketfd[i]))
		{	socketfd[i] = sid;
			counter++;
			return true;
	}	}
	return SetZombieSlot(sid);
}

bool BsdSocketPool::SetZombieSlot(SOCKET sid)
{	for(unsigned i =0;i<socketfd.size();i++)
	{	if(IsGood(socketfd[i]))
		{	PacketSocket bsdSocket(socketfd[i]);
			if(!bsdSocket.SendEmptyPacket())
			{	socketfd[i] = sid;
				return true;
	}	}	}
	return false;
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
//	printf("%u:%u #%u size=%u hash=%llx\n",count,(unsigned) counter,packetId,packetSize,hash);
	return count;
}

void BsdSocketPool::ReleaseSlot(SOCKET slot)
{	if (slot >= socketfd.size())
	{	return;
	}
	if (0 >= socketfd[slot])
	{	return;
	}
	PacketSocket bsdSocket(socketfd[slot]);
	bsdSocket.Close();
	socketfd[slot] = 0;
	if(!counter)
	{	puts("Socket counter underflow");
	}
	else
	{	counter--;
	}
	printf("Released slot %u, connections = %u\n", (unsigned)slot, (unsigned)counter);
}


}