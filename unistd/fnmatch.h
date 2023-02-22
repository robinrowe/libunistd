#ifndef fnmatch_h
#define fnmatch_h

#include "cfunc.h"

#define	FNM_NOMATCH 1

CFUNC int fnmatch(const char *pattern, const char *string, int flags);

#endif
