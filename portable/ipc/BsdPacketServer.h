// portable/BsdPacketServer.h
// Created by Robin Rowe on 2016/06/24
// License MIT Open Source

#ifndef PacketServer_h
#define PacketServer_h

#include "Packet.h"
#include "BsdSocketServer.h"
#include "BsdMulticast.h"
#include "PacketQueue.h"

namespace portable 
{

class BsdPacketServer
:	public BsdSocketServer
{	bool isVerbose;
	const char* programVersion;
	std::vector<char> headerBuffer;
	bool SendTo(PacketWriter& packet,SOCKET fd)
	{	PacketSocket bsdSocket(fd);
		if(bsdSocket.SendTo(packet))
		{	return true;
		}
		LogSocketError(bsdSocket);
		return false;
	}
	bool IsInvalid(SOCKET fd)
	{	return fd<=0;
	}
	void LogSocketError(const PacketSocket& bsdSocket);
	bool Login(SOCKET fd) override
	{	return SendHeaderPacket(fd);
	}
	bool Subscribe(SOCKET fd);
	BsdMulticast multicast;
//	bool isStreaming;
	static const unsigned bufSize = 64*1024;
public:
	PacketWriter headerPacket;
	PacketQueue<bufSize> framePacket;
	BsdPacketServer(const char* programVersion)
	:	BsdSocketServer(bufSize)
	,	isVerbose(false)
	,	programVersion(programVersion)
	,	headerBuffer(bufSize)
	,	headerPacket(&headerBuffer[0],bufSize)
	,	multicast(pool)
//	,	isStreaming(false)
	{}
	~BsdPacketServer()
	{	Close();
	}
	void Close() override
	{	BsdSocketServer::Close();
		multicast.Close();
	}
#if 0
	void MulticastHeaderPacket()
	{	multicast.SetHeaderPacket(&headerPacket);
		MulticastFramePacket();
	}
#endif
	void MulticastFramePacket()
	{	if(pool.IsEmpty())
		{	//puts("pool empty");
			return;
		}
		multicast.SetFramePacket(&framePacket.GetBaked());
		multicast.Wake();
	}
	bool SendFramePacket(SOCKET fd)
	{	if(framePacket.GetBaked().GetPacketSize()<=4)
		{	puts("Packet not ready");
			return false;
		}
		const unsigned packetId = framePacket.GetBaked().GetPacketId();
		if(!packetId)
		{	puts("Frame packet has invalid packetID, not sent");
			return false;
		}
		printf("Send Frame #%d size=%u\n",packetId, framePacket.GetBaked().header.packetSize);
		return SendTo(framePacket.GetBaked(),fd);
	}
	bool SendHeaderPacket(SOCKET fd)
	{	if(headerPacket.GetPacketSize()<=sizeof(unsigned))
		{	puts("Packet not ready");
			return false;
		}
		if(!SendTo(headerPacket,fd))
		{	puts("Paket send failed");
			return false;
		}
		std::string ip;
		PacketSocket::GetPeerName(fd,ip);
		printf("Send header packet to %s size=%u\n",ip.c_str(), headerPacket.header.packetSize);
		return SendFramePacket(fd);
	}
	void SetIsStreaming(bool isStreaming = true)
	{	multicast.SetIsStreaming(isStreaming);
	}
	void AddSkipped(unsigned skipped)
	{	multicast.AddSkipped(skipped);
	}
	void PrintStats()
	{	multicast.PrintStats();
	}
	bool Start(int serverPort,unsigned maxStreams);
	//virtual void OnStop() const;
};

}

#endif
