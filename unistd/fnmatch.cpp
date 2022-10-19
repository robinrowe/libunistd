// fnmatch.cpp
// 2022/10/19 tomerlev@microsoft.com
// License open source MIT

#include <Shlwapi.h>
#include "fnmatch.h"
#pragma comment(lib, "shlwapi.lib")
int fnmatch(const char *pattern, const char *string, int flags)
{
	BOOL match = PathMatchSpecA(string, pattern); // flags are not supported in Windows
	return match ? 0 : FNM_NOMATCH;
}
