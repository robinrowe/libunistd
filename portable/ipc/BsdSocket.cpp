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
	flags = isBlocking ? (flags&~O_NONBLOCK) : (flags | O_NONBLOCK);
	return (fcntl(socketfd, F_SETFL, flags) == 0) ? true : false;
#endif
}

bool BsdSocket::Open(const char* serverName, int serverPort)
{	puts("libunistd 1.1 " __DATE__ " " __TIME__);
	if (!serverName || !*serverName || !serverPort)
	{	errorMsg.Set("No server to open specified");
		return false;
	}
	socketfd = OpenSocket();
	if (socketfd == -1)
	{	puts("OpenSocket failed");
		errorMsg.GetLastError();
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
	{	puts("inet_pton failed");
		errorMsg.GetLastError();
		return false;
	}
	return true;
}

bool BsdSocket::Connect(bool isReuseSocket)
{	const int ok = connect(socketfd, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr));
	if (ok<0)
	{	puts("connect failed");
		errorMsg.GetLastError();
		return false;
	}
	if(!SetReuse(socketfd,isReuseSocket))
	{	puts("Can't reuse socket");
	}
	return true;
}

bool BsdSocket::Bind(bool isReuseSocket)
{    const int ok = bind(socketfd, (const struct sockaddr *)&server_sockaddr, 
        sizeof(server_sockaddr));
	if(ok<0)
	{	perror("bind failed");
		return false;
	}
	if(!SetReuse(socketfd,isReuseSocket))
	{	puts("Can't reuse socket");
	}
	return true;
}

}
