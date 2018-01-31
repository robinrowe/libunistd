// sys/poll.h
// Copyright Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_poll_h
#define sys_poll_h

#if _MSC_VER <= 1900
#include <unistd.h>

typedef WSAPOLLFD pollfd;
typedef unsigned long nfds_t;

inline
int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{	return WSAPoll(fds,nfds,timeout);
}

#endif
#endif
