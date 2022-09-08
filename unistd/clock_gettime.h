// clock_gettime.h 
// Copyright 2022 CinePaint MIT Open Source
// 13 July 2022 Robin.Rowe@CinePaint.org

#ifndef clock_gettime_h
#define clock_gettime_h

#include "cfunc.h"

typedef int clockid_t;
typedef long long useconds_t;

enum
{	CLOCK_REALTIME,
	CLOCK_MONOTONIC,
	CLOCK_PROCESS_CPUTIME_ID,
	CLOCK_THREAD_CPUTIME_ID
};

CFUNC int clock_getres(clockid_t clk_id, struct timespec *res);
CFUNC int clock_gettime(clockid_t clk_id, struct timespec *tp);
CFUNC int clock_settime(clockid_t clk_id, const struct timespec *tp);
CFUNC int sleep(useconds_t delay);
CFUNC int usleep(useconds_t delay);
CFUNC int nanosleep(const struct timespec *req, struct timespec *rem);

#endif
