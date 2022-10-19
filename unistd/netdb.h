// netdb.h
// Provided by brimdata on 2022/10/19
// Copyright (c) 2019 Robin.Rowe@CinePaint.org
// License open source MIT

#pragma once

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

struct hostent *gethostbyname2(const char *name, int af);

#ifdef __cplusplus
}
#endif