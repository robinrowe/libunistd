// poll.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef poll_h
#define poll_h

#include "unistd.h"
#include "stub.h"
#include <vector>

#if 0
// see winsock2.h
enum
{	POLLIN=1,		// There is data to read.
	POLLPRI=2,		//urgent data to read 
	POLLOUT=4,		// Writing is now possible
	POLLRDHUP=8,	// Stream socket peer closed connection
	POLLERR=16,		// Error condition 
	POLLHUP=32,		// Hang up 
	POLLNVAL=64,	// Invalid request: fd not open 
	POLLRDNORM = POLLIN,
	POLLRDBAND=128,	// Priority band data can be read 
	POLLWRNORM = POLLOUT,
	POLLWRBAND=256,	// Priority data may be written
	POLLMSG=512		// Unused
};

struct pollfd 
{	int  fd;		// file descriptor 
	short events;	// requested events 
	short revents;	// returned events 
};

struct fd_set 
{	u_int fd_count;               /* how many are SET? */
	SOCKET  fd_array[FD_SETSIZE];   /* an array of SOCKETs */
}
#endif

typedef int nfds_t;

inline
int poll(struct pollfd *fds, nfds_t nfds, int mille_timeout)
{	timeval timeout;
	timeout.tv_sec=mille_timeout/1000;         
	timeout.tv_usec=1000000*mille_timeout%1000; 
	std::vector<fd_set> fd(2*nfds);
	u_int& readerCount=fd[0].fd_count;
	readerCount=0;
	SOCKET* fdReader=fd[0].fd_array;
	int writer=nfds;
	u_int& writerCount=fd[nfds].fd_count;
	writerCount=0;
	SOCKET* fdWriter=fd[nfds].fd_array;
	for(int i=0;i<nfds;i++)
	{	if(fds[i].events & POLLIN)
		{	fdReader[readerCount]=fds[i].fd;
			readerCount++;
		}
		if(fds[i].events & POLLOUT)
		{	fdWriter[writerCount]=fds[i].fd;
			writerCount++;
	}	}
	fd_set fdExcept;
	fdExcept.fd_count=0;
	return select(nfds, &fd[0], &fd[nfds], &fdExcept, &timeout);
}

inline
int ppoll(struct pollfd *fds, nfds_t nfds,const struct timespec *tmo_p, const sigset_t *sigmask)
STUB0(ppoll)

#endif
