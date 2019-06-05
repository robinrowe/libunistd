// BsdSocket.cpp
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#pragma comment(lib, "libunistd.lib")

#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include "BsdSocket.h"
#define ON_SOCKET_ERROR(msg) OnSocketError(__FILE__,__LINE__,msg)

namespace portable 
{

void BsdSocket::Close()
{	Trace("Socket close");
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

bool BsdSocket::SetAsyncMode(bool isAsync)
{	if (!IsOpen())
	{	return false;
	}
#ifdef _WIN32
	unsigned long mode = isAsync ? 1 : 0;
	return (ioctlsocket(socketfd, FIONBIO, &mode) == 0) ? true : false;
#else
	int flags = fcntl(socketfd, F_GETFL, 0);
	if (flags < 0)
	{	return false;
	}
	const bool isBlocking = !isAsync;
	flags = isBlocking ? (flags&~O_NONBLOCK) : (flags | O_NONBLOCK);
	return (fcntl(socketfd, F_SETFL, flags) == 0) ? true : false;
#endif
}

bool BsdSocket::Open(const char* serverName, int serverPort)
{	if (!serverName || !*serverName || !serverPort)
	{	ON_SOCKET_ERROR("No server to open specified");
		return false;
	}
	TraceOpen(serverName,serverPort);
	socketfd = OpenSocket();
	if (socketfd == -1)
	{	ON_SOCKET_ERROR("OpenSocket failed");
		return false;
	}
	memset((char *)&server_sockaddr, 0, sizeof(server_sockaddr));
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons((u_short)serverPort);
	//		server_sockaddr.sin_addr.S_un.S_addr = inet_addr(serverName);
	std::string hostname;
	if (!GetIp(serverName, hostname))
	{	hostname = serverName;
	}
	if (1 != inet_pton(AF_INET, hostname.c_str(), &server_sockaddr.sin_addr))
	{	ON_SOCKET_ERROR("inet_pton failed");
		return false;
	}
	return true;
}

bool BsdSocket::Connect()
{	const int ok = connect(socketfd, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr));
	if (ok<0)
	{	ON_SOCKET_ERROR("connect failed");
		return false;
	}
	return true;
}

bool BsdSocket::Bind()
{    const int ok = bind(socketfd, (const struct sockaddr *)&server_sockaddr, 
        sizeof(server_sockaddr));
	if(ok<0)
	{	ON_SOCKET_ERROR("bind failed");
		return false;
	}
	return true;
}
/*
bool BsdSocket::SetTimeout(unsigned timeout_in_seconds)
{
#ifdef _WIN32
	const DWORD timeout = timeout_in_seconds * 1000;
	const int ok = setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);
	return ok!=-1;
#else
	struct timeval tv;
	tv.tv_sec = timeout_in_seconds;
	tv.tv_usec = 0;
	const int ok = setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	return ok!=-1;
#endif
}
*/
bool BsdSocket::SendTo(const char* msg,unsigned len)
{	if(!IsOpen())
	{	return false;
	}
	socklen_t slen = sizeof(sockaddr_in);
	Trace("SendTo",msg,len);
	if(sendto(socketfd,msg,len,0,(struct sockaddr *)&server_sockaddr,slen)==-1)
	{	ON_SOCKET_ERROR("sendto");
		return false;
	}
	return true;
}

int BsdSocket::RecvFrom(char* buffer,unsigned bufsize,unsigned offset)
{	socklen_t slen = sizeof(sockaddr_in);
	if(socketfd<=0)
	{	ON_SOCKET_ERROR("Socket not open");
		return -1;
	}	
	bytesRead = recvfrom(socketfd,buffer+offset,bufsize-offset,0,(struct sockaddr *)&server_sockaddr,&slen);
	Trace("RecvFrom",buffer+offset,bytesRead);
	return bytesRead;
}

}
