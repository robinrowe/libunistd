// netdb.c
// Copyright (c) 2022/10/19 Noah Treuhaft <noah@brimdata.io>
// License open source MIT

#include "netdb.h"
#include <stdlib.h>

#pragma warning(disable:4996)

struct hostent *gethostbyname2(const char *name, int af)
{	// Winsock's gethostbyname() only returns AF_INET addresses.
	if ( af != AF_INET )
	{	return 0;
	}
	return gethostbyname(name);
}
