// fnmatch.cpp
// Copyright (c) 2022/10/19 Tomer Lev <tomerlev@microsoft.com>
// License open source MIT

#include <Shlwapi.h>
#include "fnmatch.h"

int fnmatch(const char *pattern, const char *string, int flags)
{	BOOL match = PathMatchSpecA(string, pattern); // flags are not supported in Windows
	return match ? 0 : FNM_NOMATCH;
}
