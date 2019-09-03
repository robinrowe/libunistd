// portable/BsdSocketClient.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketClient_h
#define BsdSocketClient_h

#include "PacketSocket.h"
#include "PacketStats.h"

namespace portable 
{

class BsdSocketClient
:	public PacketSocket
{	std::thread worker;
	const unsigned bufsize;
	static void Main(BsdSocketClient* self)
    {   self->Run();
    }
protected:
	void Run() override;
	virtual bool ReadHeader(portable::PacketReader& packet) = 0;
	virtual bool ReadFrame(portable::PacketReader& packet,unsigned packetId) = 0;
	virtual void SocketReset(const char* msg,portable::PacketReader& packet);
	unsigned OnPacket(unsigned bytes,portable::PacketReader& packet) override;
public:
	PacketStats stats;
	BsdSocketClient(unsigned bufsize)
	:	bufsize(bufsize)
	{}
	void Close() override
	{	Stop();
		if(socketfd)
		{	closesocket(socketfd);
			socketfd=0;
	}	}
	bool Open(const char* serverName,int serverPort)
	{	if(!PacketSocket::Open(serverName,serverPort))
		{	return false;
		}
		Start();
		return true;
	}
	void Start() override
	{	worker=std::thread(Main,this);
		worker.detach();
//		puts("Started socket thread");
	}
};

}

#endif

