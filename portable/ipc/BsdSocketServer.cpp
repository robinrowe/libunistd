// BsdSocketServer.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#include "BsdSocketServer.h"

namespace portable 
{

SOCKET BsdSocketServer::ListenAccept()
{   const int backlog = 1; //point-to-point, not SOMAXCONN;
	listen(socketfd,backlog); 
	sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);
#ifdef _DEBUG
	puts("\nAccepting connections...");
#endif
	SOCKET newsockfd = accept(socketfd, (struct sockaddr *)&cli_addr, &clilen);
	if (newsockfd < 0) 
	{	perror("ERROR on accept");
	}
#ifdef _DEBUG
	else
	{	puts("connected");
	}
#endif
	return newsockfd;
} 

void BsdSocketServer::ListenRun()
{	while(isGo)
	{	if(socketfd>0)
		{	SOCKET sock = ListenAccept();
			if(!pool.SetSlot(sock))
			{	puts("WARN: no socket slots");
				continue;
			}
			if(!Login(sock))
			{	puts("ERROR: socket connect failed");
				pool.ReleaseSlot(sock);
				continue;
			}
			OnConnect(sock);
		}
	}
	OnStop();
}

void BsdSocketServer::PacketRun()
{	std::unique_ptr<char[]> buffer(new char[bufsize]);
	PacketReader packet(buffer.get(),bufsize);
	unsigned offset=0;
	while(isGo)
	{	const int bytes = RecvFrom(buffer.get(),bufsize,offset);
		packet.Reset();
		offset=OnPacket(bytes,packet);
	}
	OnStop();
}

bool BsdSocketServer::Open(int serverPort,int maxStreams,bool isPacketRun)
{	this->isPacketRun=isPacketRun;
	pool.Reset(maxStreams); 
	socketfd=OpenSocket();
	if(socketfd == -1)
	{	puts(errorMsg.GetLastError());
		return false;
	}
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons((u_short) serverPort);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(::bind(socketfd, (struct sockaddr*)&server_sockaddr,sizeof(server_sockaddr)) == -1)
	{	puts(errorMsg.GetLastError());
		return false;
	}
	SetReuse(socketfd);
	isGo=true;
	return true;
}

}