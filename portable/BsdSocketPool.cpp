// BsdSocketPool.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#include "BsdSocketPool.h"
#include "SoftLock.h"

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
			{	socketfd[i]=0;
				if(!s)
				{	s = &socketfd[i];
	}	}	}	}
	return s;
}


bool BsdSocketPool::ReleaseSlot(SOCKET* sock)
{	for(unsigned i =0;i<socketfd.size();i++)
	{	if(sock == &socketfd[i])
		{	socketfd[i]=0;
			return true;
	}	}
	return false;
}

int BsdSocketPool::DirectMulticast(Packet& packet)
{	if(!packet.IsGood())
	{	return -1;
	}
	SoftLock softlock(packet.ownership);
	if(!softlock)
	{	return -1;
	}
	const unsigned maxShowPacketId = 10;
//	if(counter<maxShowPacketId)
	printf("Muticast packet #%d\n",packet.GetPacketId());
	int count = 0;
	for(unsigned i=0;i<socketfd.size();i++)
	{	if(!socketfd[i])
		{	continue;
		}
		BsdSocket bsdSocket(socketfd[i]);
		if(!bsdSocket.SendTo(packet))
		{	ReleaseSlot(i);
			continue;
		}
		count++;
	}
	return count;
}

}