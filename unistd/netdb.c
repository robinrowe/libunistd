// netdb.c
// Provided by brimdata on 2022/10/19
// Copyright (c) 2019 Robin.Rowe@CinePaint.org
// License open source MIT

#include "netdb.h"
#include <stdlib.h>

struct hostent *gethostbyname2(const char *name, int af)
	{
	// Winsock's gethostbyname() only returns AF_INET addresses.
	if ( af != AF_INET )
		return NULL;
	return gethostbyname(name);
	}
