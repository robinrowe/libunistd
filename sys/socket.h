#ifndef socket_h
#define socket_h

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <Mstcpip.h>
#include <Ws2tcpip.h>
#include <stdint.h>

typedef uint32_t sa_family_t;


// The iovec structure shall be defined as described in <sys/uio.h> .

struct iovec
{	int junk;
};

struct msghdr 
{	void* msg_name;
	socklen_t msg_namelen;
	struct iovec* msg_iov;
	int msg_iovlen;
	void* msg_control;
	socklen_t msg_controllen;
	int msg_flags;
};


typedef int caddr_t;

#endif
