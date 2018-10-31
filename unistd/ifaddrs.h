// ifaddrs.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT
// Creates a linked list of structures describing the network interfaces of the local system

#ifndef ifaddrs_h
#define ifaddrs_h

#include "../portable/stub.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

#define ifa_broadaddr ifa_ifu.ifu_broadaddr
#define ifa_dstaddr ifa_ifu.ifu_dstaddr

struct ifaddrs 
{	struct ifaddrs *ifa_next;
	char* ifa_name; 
	unsigned int ifa_flags; 
	struct sockaddr* ifa_addr; 
	struct sockaddr* ifa_netmask;
	union 
	{	struct sockaddr* ifu_broadaddr;
		struct sockaddr* ifu_dstaddr;
	} ifa_ifu;
	void* ifa_data;    
};

inline
int getifaddrs(struct ifaddrs **ifap)
{   STUB_NEG(getifaddrs);
}

inline
void freeifaddrs(struct ifaddrs *ifa)
{   STUB(freeifaddrs);
}

#ifdef __cplusplus
}
#endif

#endif
