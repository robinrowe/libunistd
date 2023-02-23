// netdb.h
// Copyright (c) 2022/10/19 Noah Treuhaft <noah@brimdata.io>
// License open source MIT

#ifndef netdb_h
#define netdb_h

#include <unistd.h>
#include "cfunc.h"

CFUNC struct hostent *gethostbyname2(const char *name, int af);

#endif
