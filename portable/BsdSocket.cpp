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
{	if(!serverName || !*serverName || !serverPort)
	{	puts("No server to open specified");
		return false;
	}
	socketfd=OpenSocket();
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

void BsdSocketClient::Run()
{	PacketReader packet(buffer,bufsize);
	if(!bufsize)
	{	return;
	}
	while(isGo)
	{	const int bytes = RecvFrom(buffer,bufsize);
		packet.Rewind();
		OnPacket(bytes,packet);
	}
	OnStop();
}

void BsdSocketClient::OnPacket(int bytes,portable::PacketReader& packet)
{	if(bytes<0)	
	{	SocketReset("Socket closed");		
		return;
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
{	while(isGo)
	{	SOCKET fd = ListenAccept();
		if(fd<=0)
		{	continue;
		}
		Login(fd);
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

void BsdSocketClient::Start()
{	worker=std::thread(Main,this);
#if 0
	if(isBlocking)
	{	worker.join();
	}
	else
#endif
	{	worker.detach();
}	}


void BsdSocketServer::Start()
{	worker=std::thread(Main,this);
	worker.detach();
}

void BsdSocketServer::OnPacket(int bytes,portable::PacketReader& packet)
{	if(bytes<0)	
	{	SocketReset("Socket closed");		
		return;
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
}

}
