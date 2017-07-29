// sys/select.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_select_h
#define sys_select_h

#include "../unistd.h"

/* Open Groups says timeval should be this...

struct timeval {
time_t         tv_sec      // Seconds. 
suseconds_t    tv_usec     // Microseconds. 
};

However, Winsock2.h has this...

struct timeval {
        long    tv_sec;    // seconds 
        long    tv_usec;   // and microseconds 
};

The time_t and suseconds_t types shall be defined as described in <sys/types.h>.
The sigset_t type shall be defined as described in <signal.h>.
The timespec structure shall be defined as described in <time.h>.



inline
void FD_CLR(int fd, fd_set *fdset)
STUB(FD_CLR)

inline
int FD_ISSET(int fd, fd_set *fdset)
STUB0(FD_ISSET)

inline
void FD_SET(int fd, fd_set *fdset)
STUB(FD_SET)

inline
void FD_ZERO(fd_set *fdset)
STUB(FD_ZERO)

//FD_SETSIZE

inline
int pselect(int, fd_set *restrict, fd_set *restrict, fd_set *restrict,const struct timespec *restrict, const sigset_t *restrict)
STUB0(pselect)

inline
int select(int, fd_set *restrict, fd_set *restrict, fd_set *restrict,struct timeval *restrict)
STUB0(select)
*/
#endif
