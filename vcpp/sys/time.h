// sys/time.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_time_h
#define sys_time_h

#include "../unistd.h"
#include <time.h>
#include "../portable/stub.h"

#ifdef __cplusplus
#include <thread>

extern "C" {
#else
#define inline __inline
#endif

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

inline
int getitimer(int which, struct itimerval *curr_value)
STUB0(getitimer)

inline
int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value)
STUB0(setitimer)

enum
{	ITIMER_REAL,
	ITIMER_VIRTUAL,
	ITIMER_PROF
};

#if 0
typedef struct _SYSTEMTIME {
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME;
#endif

#ifdef __cplusplus
}
#endif

#endif
