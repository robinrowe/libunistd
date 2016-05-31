// sys/types.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_types_h
#define sys_types_h

#include "../unistd.h"

typedef intptr_t pid_t;

inline
pid_t gettid()
{	HANDLE h = GetCurrentThread();
	return (intptr_t) h;
}

#endif
