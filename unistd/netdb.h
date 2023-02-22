// netdb.h
// Provided by brimdata on 2022/10/19
// Copyright (c) 2019 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef netdb_h
#define netdb_h

#include <unistd.h>
#include "cfunc.h"

CFUNC struct hostent *gethostbyname2(const char *name, int af);

#endif
