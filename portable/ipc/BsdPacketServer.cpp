// portable/BsdPacketServer.cpp
// Created by Robin Rowe on 2016/06/24
// License MIT Open Source

#include "BsdPacketServer.h"
#include "../Logger.h"

namespace portable 
{
	
bool BsdPacketServer::Start(int serverPort,unsigned maxStreams)
{	const bool isReceive=false;
	if(!Open(serverPort,maxStreams,isReceive))
	{	return false;
	}
	const bool isBlocking=true;
	isVerbose = false;//true; //bug - should pass in as param
	BsdSocketServer::Start();
	return true;
}

void BsdPacketServer::LogSocketError(const PacketSocket& bsdSocket)
{	std::string msg;
	bsdSocket.GetPeerName(msg);
	msg+=" SendTo error: ";
	msg+=bsdSocket.errorMsg;
	error_msg(msg.c_str());
}

#if 0
bool BsdPacketServer::Login(SOCKET* slot,SOCKET fd) 
{	if(!slot)
	{	puts("No slot");
		return false;
	}
	for(unsigned i=0;i<pool.socketfd.size();i++)
	{	if(IsInvalid(pool.socketfd[i]))
		{	
#if 0
			if(!isStreaming)
			{	if(!SendHeaderPacket(fd))
				{	return false;
				}
				pool.isHeaderSent[i] = true;
			}
#endif
			pool.socketfd[i] = fd;
			pool.counter++;
#if 0
			std::string msg;
			BsdSocket bsdSocket(fd);
			bsdSocket.GetPeerName(msg);
			msg+=" Connected";
			LogMsg(msg);
#endif
			return 	SendHeaderPacket(fd);
	}	}
	LogMsg("Can't subscribe");
	return false;
}
#endif

}

