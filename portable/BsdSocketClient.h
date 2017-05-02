// portable/BsdSocketClient.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketClient_h
#define BsdSocketClient_h

#include "BsdSocket.h"
#include "PacketStats.h"

namespace portable 
{

class BsdSocketClient
:	public BsdSocket
{	std::thread worker;
	const unsigned bufsize;
	int packetSize;
	static void Main(BsdSocketClient* self)
    {   self->Run();
    }
	bool ReadyStream(int bytes,portable::PacketReader& packet) 
	{	if(bytes<0)
		{	SocketReset("Socket closed",packet);
			return false;
		}
		if(0==bytes)
		{	return false;
		}
		if(bytes<sizeof(unsigned))
		{	stats.fragments++;
			return false;
		}
		packetSize = packet.GetPacketSize();
		if(packetSize > bytes)
		{	stats.fragments++;
			return false;
		}
		return true;
	}
protected:
	void Run() override;
	virtual bool ReadHeader(portable::PacketReader& packet) = 0;
	virtual bool ReadFrame(portable::PacketReader& packet,unsigned packetId) = 0;
	virtual void SocketReset(const char* msg,portable::PacketReader& packet);
	int OnPacket(int bytes,portable::PacketReader& packet) override;
public:
	PacketStats stats;
	BsdSocketClient(unsigned bufsize)
	:	bufsize(bufsize)
	,	packetSize(0)
	{}
	void Close()
	{	Stop();
		if(socketfd)
		{	closesocket(socketfd);
			socketfd=0;
	}	}
	bool Open(const char* serverName,int serverPort);
	void Start() override
	{	worker=std::thread(Main,this);
		worker.detach();
//		puts("Started socket thread");
	}
};

}

#endif

