// BsdSocketClient.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#include "BsdSocketClient.h"

namespace portable 
{

bool BsdSocketClient::Open(const char* serverName,int serverPort)
{	if(!serverName || !*serverName || !serverPort)
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
	isGo=true;
	Start();
	return true;
}

void BsdSocketClient::Run()
{	std::unique_ptr<char[]> buffer(new char[bufsize]);
	PacketReader packet(buffer.get(),bufsize);
	unsigned offset=0;
	while(isGo)
	{	const int bytes = RecvFrom(buffer.get(),bufsize,offset);
#if 0
		if(bytes<=0)
		{	//if(!OnBadPacket(bytes))
			offset = 0;
			continue;
		}
#endif
		packet.Init();
		offset=OnPacket(bytes,packet);
	}
	OnStop();
}

int BsdSocketClient::OnPacket(int bytes,portable::PacketReader& packet)
{	if(bytes<0)
	{	SocketReset("Socket closed",packet);
		return 0;
	}
	if(bytes<=sizeof(unsigned))
	{	if(!bytes)
		{	SocketReset("Connection lost",packet);
			return 0;
		}
		SocketReset("Packet receive underflow",packet);
		return 0;
	}
	int packetSize = (int) packet.GetPacketSize();
	if(packetSize<sizeof(packetSize))
	{	SocketReset("Packet size underflow",packet);
		return 0;
	}	
	//LogMsg("Receive packet");
	for(;;)
	{	
#if 0			
		cout <<"bytes: "<<bytes<<" packet: "<<packetSize<<endl;
#endif
		if(bytes<packetSize)
		{	puts("Packet fragment");
			//SocketReset("Packet size overflow bytes",packet);
			return bytes;
		}
		if(packetSize > (int) packet.GetCapacity())
		{	SocketReset("Packet size corrupted",packet);
			return 0;
		}
		unsigned packetId = 0;
		packet>>packetId;
		if(0==packetId)
		{//	LogMsg("Reading header");
			if(!ReadHeader(packet))
			{	SocketReset("Packet header corrupted",packet);
				return 0;
		}	}
		else
		{//	LogMsg("Reading frame");
			if(!ReadFrame(packet,packetId))
			{	std::string s("Packet frame corrupted #");
				s+=std::to_string(packetId);
				SocketReset(s.c_str(),packet);
				return 0;
		}	}
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
		{	SocketReset("Packet receive underflow bytes",packet);
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
		{	SocketReset("Packet size underflow packet",packet);
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