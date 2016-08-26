// portable/BsdSocketClient.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketClient_h
#define BsdSocketClient_h

#include "BsdSocket.h"

namespace portable 
{
struct PacketStatus
{	unsigned packetCount;
	unsigned printStatusMax;
	unsigned packetFragments;
	unsigned packetErrors;
	PacketStatus()
	:	packetCount(0)
	,	packetFragments(0)
	,	packetErrors(0)
	{	printStatusMax = 60*60;
		dumpFilename = "PacketDump.bin";
	}
	bool IsActive() const
	{	return 0 != printStatusMax;
	}
	const char* dumpFilename;
	void Print(unsigned id,int bytes,int packetSize, int capacity)
	{	//if(packetCount >= printStatusMax || packetErrors == 1)
		{	packetCount = 0;
			printf("id: %u packets: %i bytes: %i packetSize: %i capacity: %u fragments: %u errors: %u\n",
					id,    packetCount,bytes,   packetSize, capacity, packetFragments,packetErrors);	
	}	}
};

class BsdSocketClient
:	public BsdSocket
{	std::thread worker;
	const unsigned bufsize;
	PacketStatus status;
	int packetSize;
	static void Main(BsdSocketClient* self)
    {   self->Run();
    }
	bool ReadyStream(int bytes,portable::PacketReader& packet) 
	{	if(bytes<0)
		{	SocketReset("Socket closed",packet);
			return false;
		}
		if(bytes<sizeof(int))
		{	status.packetFragments++;
			return false;
		}
		packetSize = packet.GetPacketSize();
		if(packetSize < bytes)
		{	status.packetFragments++;
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
	BsdSocketClient(unsigned bufsize)
	:	bufsize(bufsize)
	,	packetSize(0)
	{}
	void PrintStatusMax(unsigned maxPackets)
	{	status.printStatusMax = maxPackets;
	}
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
		puts("Started socket thread");
	}
};

}

#endif

