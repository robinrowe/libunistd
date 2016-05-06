// sys/time.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_time_h
#define sys_time_h

#include <time.h>
#include "../stub.h"

struct itimerval 
{	struct timeval it_interval; /* Interval for periodic timer */
	struct timeval it_value;    /* Time until next expiration */
};
#if 0
typedef int suseconds_t;

struct timeval 
{	time_t      tv_sec;         /* seconds */
	suseconds_t tv_usec;        /* microseconds */
};
#endif
int getitimer(int which, struct itimerval *curr_value)
STUB0(getitimer)

int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value)
STUB0(setitimer)

enum
{	ITIMER_REAL,
	ITIMER_VIRTUAL,
	ITIMER_PROF
};

#endif
