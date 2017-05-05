// BsdSocketClient.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#include "BsdSocketClient.h"
#include <portable/Logger.h>

namespace portable 
{

bool BsdSocketClient::Open(const char* serverName,int serverPort)
{	puts("libunistd 1.1 " __DATE__ " " __TIME__);
	if(!serverName || !*serverName || !serverPort)
	{	errorMsg.Set("No server to open specified");
		return false;
	}
	socketfd=OpenSocket();
	if(socketfd == -1)			
	{	puts("OpenSocket failed");
		errorMsg.GetLastError();
		return false;
	}
	memset((char *) &server_sockaddr, 0, sizeof(server_sockaddr));
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons((u_short) serverPort);  
//		server_sockaddr.sin_addr.S_un.S_addr = inet_addr(serverName);
	std::string hostname;
	if(!GetIp(serverName,hostname))
	{	hostname = serverName;
	}
	if(1!=inet_pton(AF_INET,hostname.c_str(),&server_sockaddr.sin_addr))
	{	puts("inet_pton failed");
		errorMsg.GetLastError();
		return false;
	}
	const int ok = connect(socketfd, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr));
	if(ok<0) 
	{	puts("connect failed");
		errorMsg.GetLastError();
		isGo=false;
		return false;
	}
	SetReuse(socketfd);
	Start();
	return true;
}

void BsdSocketClient::Run()
{	isGo = true;
	std::unique_ptr<char[]> buffer(new char[bufsize]);
	PacketReader packet(buffer.get(),bufsize);
	unsigned offset=0;
	while(isGo)
	{	const int bytes = RecvFrom(buffer.get(),bufsize,offset);
		if(bytes<=0)
		{	//if(!OnBadPacket(bytes))
			offset = 0;
			continue;
		}
		packet.Init();
		offset=OnPacket(bytes+offset,packet);
	}
	OnStop();
}

unsigned BsdSocketClient::OnPacket(unsigned bytes,portable::PacketReader& packet)
{	if(!ReadyStream(bytes,packet))
	{	return bytes;
	}
#if 0
	printf("bytes: %i packetSize: %i\n",bytes,packetSize);
#endif
	unsigned packetId = 0;
	//LogMsg("Receive packet");
	for(;;)
	{	if(!bytes)
		{	puts("No bytes at middle");
			return 0;
		}
		//status.Print(packetId,bytes,packetSize);
		if(bytes<packetSize)
		{	stats.fragments++;
			//printf("Packet fragment %u < %u\n",bytes,packetSize);
			//SocketReset("Packet size overflow bytes",packet);
			return bytes;
		}
		const unsigned capacity = packet.GetCapacity();
		if(packetSize > capacity)
		{	stats.errors++;
			stats.Print(packetId,bytes,packetSize, capacity);
			SocketReset("Packet size corrupted",packet);
			return 0;
		}
		packetId = 0;
		packet>>packetId;
		packet.SetPacketId(packetId);
		if(!stats.GetLast())
		{	printf("reading packet #%u\n",packetId);
		}
		if(0==packetId)
		{	LogMsg("Reading header");
			if(!ReadHeader(packet))
			{	stats.Print(packetId,bytes,packetSize, capacity);
				SocketReset("Packet header corrupted",packet);
				return 0;
		}	}
		else
		{//	LogMsg("Reading frame");
			ReadFrame(packet,packetId);
#if _DEBUG
			printf(".");
#endif
		}
		packet.SkipHash();
		stats.Received(packetId);
		const unsigned readOffset=packet.GetReadOffset();
		if(readOffset!=packetSize || bytes<packetSize)
		{	std::string s("readOffset/packetSize = ");
			s+=std::to_string(readOffset);
			s+="/";
			s+=std::to_string(packetSize);
			puts(s.c_str());
		}
		bytes-=packetSize;
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
		{	stats.Print(packetId,bytes,packetSize, capacity);
			SocketReset("Packet receive underflow bytes",packet);
			return 0;
		}
//		packet>>packetSize;
		packetSize=packet.GetPacketSize();
#if 0
		msg="Pipelined packetSize = ";
		msg+=std::to_string(packetSize);
		puts(msg.c_str());
#endif
		if(packetSize<sizeof(packetSize))
		{	stats.Print(packetId,bytes,packetSize, capacity);
			SocketReset("Packet size underflow packet",packet);
			return 0;
		}	
	}
}

void BsdSocketClient::SocketReset(const char* msg,portable::PacketReader& packet)
{	packet.Dump();
	BsdSocket bsdSocket(socketfd);
	bsdSocket.Close();
	socketfd = 0;
	Stop();
}

}