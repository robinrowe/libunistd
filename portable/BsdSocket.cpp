// BsdSocket.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#pragma comment(lib, "libunistd.lib")

#include "BsdSocket.h"

namespace portable 
{

SocketStartup::SocketStartup()
{
#ifdef _WIN32
	WORD version_requested=MAKEWORD(2,0);
	WSADATA data;
	WSAStartup(version_requested,&data);
#endif
}

SocketStartup::~SocketStartup()
{	
#ifdef _WIN32
	WSACleanup();
#endif
}

void BsdSocket::GetPeerName(std::string& s) const
{	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];
	int port;
	socklen_t len = sizeof addr;
	getpeername(socketfd, (struct sockaddr*)&addr, &len);
	if (addr.ss_family == AF_INET) 
	{	struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	}
	else 
	{ // AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
		port = ntohs(s->sin6_port);
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}
	s=ipstr;
}

bool BsdSocketClient::Open(const char* serverName,int serverPort)
{	if(!serverName || !*serverName || !serverPort)
	{	errorMsg.Set("No server to open specified");
		return false;
	}
	socketfd=OpenSocket();
	if(socketfd == -1)			
	{	errorMsg.GetLastError();
		return false;
	}
	memset((char *) &server_sockaddr, 0, sizeof(server_sockaddr));
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons((u_short) serverPort);  
//		server_sockaddr.sin_addr.S_un.S_addr = inet_addr(serverName);
	if(1!=inet_pton(AF_INET,serverName,&server_sockaddr.sin_addr))
	{	errorMsg.GetLastError();
		return false;
	}
	const int ok = connect(socketfd, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr));
	if(ok<0) 
	{	errorMsg.GetLastError();
		return false;
	}
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
		packet.Init();
		offset=OnPacket(bytes,packet);
	}
	OnStop();
}

void BsdSocketServer::Run()
{	std::unique_ptr<char[]> buffer(new char[bufsize]);
	PacketReader packet(buffer.get(),bufsize);
	unsigned offset=0;
	while(isGo)
	{	if(socketfd<=0)
		{	ListenAccept();
		}
		const int bytes = RecvFrom(buffer.get(),bufsize,offset);
		packet.Init();
		offset=OnPacket(bytes,packet);
	}
	OnStop();
}

bool BsdSocketServer::Open(int serverPort,int maxStreams)
{	//bug maxStreams = 
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
	isGo=true;
	return true;
}

SOCKET BsdSocketServer::ListenAccept()
{   const int backlog = 1; //point-to-point, not SOMAXCONN;
	listen(socketfd,backlog); 
	sockaddr_in cli_addr;
	int clilen = sizeof(cli_addr);
#ifdef _DEBUG
	puts("\nListening...");
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

}
