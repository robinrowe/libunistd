// BsdSocket.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

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

int BsdSocket::OpenSocket()
{	if(isTcp)
	{	return (int) socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	}
	else
	{	return (int) socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
}	}

#if 0
void BsdSocket::Resize(unsigned bufsize)
{	if(!bufsize)
	{	this->bufsize = 0;
		buffer.reset();
		return;
	}
	buffer=std::unique_ptr<char[]>(new char[bufsize]);
	this->bufsize = bufsize;
	//buffer=std::make_unique<char[]>(bufsize);
}
#endif

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
{	socketfd=OpenSocket();
	if(socketfd == -1)			
	{	puts(errorMsg.GetLastError());
		return false;
	}
	memset((char *) &server_sockaddr, 0, sizeof(server_sockaddr));
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons((u_short) serverPort);  
//		server_sockaddr.sin_addr.S_un.S_addr = inet_addr(serverName);
	if(1!=inet_pton(AF_INET,serverName,&server_sockaddr.sin_addr))
	{	puts(errorMsg.GetLastError());
		return false;
	}
	const int ok = connect(socketfd, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr));
	if(ok<0) 
	{	perror("ERROR connecting");
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

void BsdSocketServer::Run()
{	PacketReader packet(buffer.get(),bufsize);
	while(isGo)
	{	SOCKET fd = ListenAccept();
		if(fd<=0)
		{	continue;
		}
		Login(fd);
#if 0
		const int bytes = bug RecvFrom();
		packet.Rewind();
		OnPacket(bytes,packet);
#endif
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

void BsdSocketServer::Start()
{	worker=std::thread(Main,this);
#if 0
	if(isBlocking)
	{	worker.join();
	}
	else
#endif
	{	worker.detach();
}	}

void BsdSocketServer::OnPacket(int bytes,portable::PacketReader& packet)
{
#if 0	
//bug
	if(byt	<0	
	{	SocketReset("Socket closed")				return;
	}
	if(bytes<=sizeof(unsigned))
	{	SocketReset("Packet receive underflow");
		return;
	}
	int packetSize = (int) packet.GetPacketSize();
	if(packetSize<sizeof(packetSize))
	{	SocketReset("Packet size underflow");
		return;
	}	
	//LogMsg("Receive packet");
	for(;;)
	{	if(bytes<packetSize)
		{	SocketReset("Packet size overflow");
			return;
		}
		if(packetSize>64*1024)
		{	SocketReset("Packet size corrupted");
			return;
		}
		ReadPacket();
		bytes-=packetSize;
		if(!bytes)
		{	return;
		}
		if(bytes<=sizeof(unsigned))
		{	SocketReset("Packet receive underflow");
			return;
		}
		packet>>packetSize;
		if(packetSize<sizeof(packetSize))
		{	SocketReset("Packet size underflow");
			return;
		}	
	}
#endif
}

}
