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

}