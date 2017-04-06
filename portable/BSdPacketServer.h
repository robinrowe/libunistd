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
	static const unsigned bufSize = 64*1024;
	std::vector<char> headerBuffer;
	bool SendTo(PacketWriter& packet,SOCKET fd)
	{	BsdSocket bsdSocket(fd);
		if(bsdSocket.SendTo(packet))
		{	return true;
		}
		LogSocketError(bsdSocket);
		return false;
	}
	bool IsInvalid(SOCKET fd)
	{	return fd<=0;
	}
	void LogSocketError(const BsdSocket& bsdSocket);
	virtual bool Login(SOCKET* slot,SOCKET fd) override;
	bool Subscribe(SOCKET fd);
	BsdMulticast multicast;
	bool isStreaming;
public:
	PacketWriter headerPacket;
	PacketQueue<bufSize> framePacket;
	BsdPacketServer(const char* programVersion)
	:	isVerbose(false)
	,	BsdSocketServer(bufsize)
	,	programVersion(programVersion)
	,	headerBuffer(bufSize)
	,	headerPacket(&headerBuffer[0],bufSize)
	,	multicast(pool)
	,	isStreaming(false)
	{}
	~BsdPacketServer()
	{}
	void MulticastHeaderPacket()
	{	
		multicast.SetHeaderPacket(&headerPacket);
		MulticastFramePacket();
	}
	void MulticastFramePacket()
	{	if(pool.IsEmpty())
		{	return;
		}
		multicast.SetFramePacket(&framePacket.GetBaked());
		multicast.Wake();
	}
#if 0
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
		printf("Send Frame #%d\n",packetId);
		return SendTo(framePacket.GetBaked(),fd);
	}
	bool SendHeaderPacket(SOCKET fd)
	{	if(headerPacket.GetPacketSize()<=4)
		{	puts("Packet not ready");
			return false;
		}
		if(!SendTo(headerPacket,fd))
		{	puts("Paket send failed");
			return false;
		}
		std::string ip;
		BsdSocket::GetPeerName(fd,ip);
		printf("Sent header packet to %s\n",ip.c_str());
		return SendFramePacket(fd);
	}
#endif
	void SetIsStreaming(bool isStreaming = true)
	{	this->isStreaming = isStreaming;
	}
	bool Start(int serverPort,unsigned maxStreams);
	//virtual void OnStop() const;
};

}

#endif
