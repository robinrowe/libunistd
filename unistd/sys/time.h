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
//#define inline __inline
#define nullptr 0
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
{   (void)which;
    (void)curr_value;
    STUB_NEG(getitimer);
}

inline
int setitimer(int which, const struct itimerval *new_value,struct itimerval *old_value)
{   (void)which;
    (void)new_value;
    (void)old_value;
    STUB_NEG(setitimer);
}

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

inline
struct tm *gmtime_r(const time_t* t, struct tm* result)
{	const errno_t err = gmtime_s(result, t);
	return err ? nullptr:result;
}

inline
char *asctime_r(const struct tm *tm, char *result)
{	const unsigned bufsize = 26;//minimum
	const errno_t err = asctime_s(result,bufsize,tm);
	return err ? nullptr:result;
}

inline
char *ctime_r(const time_t *t, char *result)
{	const unsigned bufsize = 26;//minimum
	const errno_t err = ctime_s(result,bufsize,t);
	return err ? nullptr:result;
}

inline
struct tm *localtime_r(const time_t *t, struct tm *result)
{	const errno_t err = localtime_s(result,t);
	return err ? nullptr:result;
}

#define timegm _mkgmtime

#ifdef __cplusplus
}
#endif

#endif
