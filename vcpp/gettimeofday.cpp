// gettimeofday.cpp
// Copyright 2016/06/20 robin.rowe@cinepaint.org
// License open source MIT

#include "unistd.h"

int gettimeofday(struct timeval* tv, struct timezone* tz)
{	FILETIME ft;
	ULARGE_INTEGER t;
	ULONGLONG x;
	ULONGLONG m=1000000;
	GetSystemTimeAsFileTime(&ft);
	t.LowPart=ft.dwLowDateTime;
	t.HighPart=ft.dwHighDateTime;
	x=t.QuadPart/m;
	tv->tv_sec=(long) x;
	x=t.QuadPart%m;
	tv->tv_usec=(long) x;
	return 0;
}