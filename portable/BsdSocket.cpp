// BsdSocket.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#pragma comment(lib, "libunistd.lib")

#include "BsdSocket.h"

namespace portable 
{

void BsdSocket::Close()
{	puts("Socket close");
	isGo=false;
	isConnected=false;
	if(socketfd)
	{	SendEmptyPacket();
		closesocket(socketfd);
		socketfd=0;
}	}

bool BsdSocket::GetIp(const char* hostname,std::string& ip)
{	struct hostent *he;
#pragma warning(disable:4996)
	he = gethostbyname(hostname);
	if(!he) 
	{	return false;
	}
	struct in_addr **addr_list; 
	addr_list = (struct in_addr **) he->h_addr_list;
	if(!addr_list[0])
	{	return false;
	}
	ip = inet_ntoa(*addr_list[0]);
#pragma warning(default:4996)
	return true;
}

void BsdSocket::GetPeerName(SOCKET sock,std::string& s)
{	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];
	socklen_t len = sizeof addr;
	getpeername(sock, (struct sockaddr*)&addr, &len);
	if (addr.ss_family == AF_INET) 
	{	struct sockaddr_in *sockin = (struct sockaddr_in *)&addr;
		inet_ntop(AF_INET, &sockin->sin_addr, ipstr, sizeof ipstr);
	}
	else 
	{ // AF_INET6
		struct sockaddr_in6 *sockin6 = (struct sockaddr_in6 *)&addr;
		inet_ntop(AF_INET6, &sockin6->sin6_addr, ipstr, sizeof ipstr);
	}
	s=ipstr;
}

}
