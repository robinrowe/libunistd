// arpa/inet.h
// Copyright 2016 Robin.Rowe\@CinePaint.org
// License open source MIT

#ifndef arpa_inet_h
#define arpa_inet_h

#include "../unistd.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

typedef unsigned in_addr_t;

inline 
in_addr_t uni_inet_addr(const char* ip)
{	in_addr_t out;
	if(inet_pton(AF_INET,ip,&out)<=0)
	{	return INADDR_NONE;
	}
	return out;
}

#define inet_addr uni_inet_addr

#ifdef __cplusplus
}
#endif

#endif
