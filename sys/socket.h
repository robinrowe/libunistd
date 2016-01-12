#ifndef socket_h
#define socket_h

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <Mstcpip.h>
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

typedef int socklen_t;
#if 0
inline
long inet_aton(const char* serverName, IN_ADDR* sin_addr)
{	LPCSTR p;
	return RtlIpv4StringToAddressA(serverName,1,&p,sin_addr);
}
#endif
#endif
