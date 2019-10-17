// BsdSocketClient.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#include "BsdSocketClient.h"
#include "../Logger.h"

namespace portable 
{

void BsdSocketClient::Run()
{	isGo = true;
	std::unique_ptr<char[]> buffer(new char[bufsize]);
	PacketReader packet(buffer.get(),bufsize);
	unsigned totalBytes = 0;
	while(isGo)
	{	const int recvBytes = RecvFrom(buffer.get(),bufsize-totalBytes,totalBytes);
		if(recvBytes<=0)
		{	printf("ERROR: socket received %i bytes (closed)\n",recvBytes);
			Stop();
			continue;
		}
		totalBytes += recvBytes;
		const unsigned consumedBytes = OnPacket(totalBytes,packet);
		if(!consumedBytes)
		{	continue;
		}
		if(consumedBytes==totalBytes)
		{	totalBytes = 0;
		}
		else
		{	const unsigned remainingBytes = totalBytes - consumedBytes;
			// printf("memmove(buffer,buffer+%u,%u)\n",consumedBytes,remainingBytes);
			memmove(buffer.get(),buffer.get()+consumedBytes,remainingBytes);
			totalBytes = remainingBytes;
		}
	}	
	OnStop();
}

unsigned BsdSocketClient::OnPacket(unsigned bytes,portable::PacketReader& packet)
{	
#if 0
	printf("bytes: %i packetSize: %i\n",bytes,packetSize);
#endif
	//LogMsg("Receive packet");
	while(bytes)
	{	if(!packet.ReadPacketHeader(bytes))
		{	stats.fragments++;
			return bytes;
		}
#if 1
		if(!stats.GetLast())
#endif
		{	printf("reading packet #%u\n",packet.header.packetId);
		}
		if(!packet.header.packetSize)
		{	error_msg("invalid packet");
			return bytes;
		}
		if(0==packet.header.packetId)
		{	trace_msg("Reading header");
			if(!ReadHeader(packet))
			{	//stats.Print(packet.header.packetId,bytes,packet.header.packetSize, capacity);
				SocketReset("Packet header corrupted",packet);
				return 0;
		}	}
		else
		{//	LogMsg("Reading frame");
			ReadFrame(packet,packet.header.packetId);
		}
		stats.Transmit(packet.header.packetId);
#if 0
		const unsigned readOffset=packet.GetReadOffset();
		if(readOffset!=packetSize || bytes<packetSize)
		{	std::string s("readOffset/packetSize = ");
			s+=std::to_string(readOffset);
			s+="/";
			s+=std::to_string(packetSize);
			puts(s.c_str());
		}
#endif
		bytes-=packet.header.packetSize;
		if(!bytes)
		{	return 0;
		}
		stats.pipelined++;
		packet.NextInPipeline();
#if 0
		std::string msg("Pipelining #");
		msg+=std::to_string(packetId);
		msg+=": ";
		msg+=std::to_string(bytes);
		msg+=" of ";
		msg+=std::to_string(packetSize);
		puts(msg.c_str());
#endif
		if(bytes<=sizeof(unsigned))
		{//	stats.Print(packetId,bytes,packetSize, capacity);
			SocketReset("Packet receive underflow bytes",packet);
			return bytes;
		}
//		packet>>packetSize;
//		packetSize=packet.GetPacketSize(bytes);
#if 0
		msg="Pipelined packetSize = ";
		msg+=std::to_string(packetSize);
		puts(msg.c_str());
#endif
	}
	return 0;
}

void BsdSocketClient::SocketReset(const char* msg,portable::PacketReader& packet)
{	(void)msg;
    packet.Dump();
	PacketSocket bsdSocket(socketfd);
	bsdSocket.Close();
	socketfd = 0;
	Stop();
}

}
