// poll.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef poll_h
#define poll_h

#include "unistd.h"
#include "../portable/stub.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

typedef int nfds_t;

inline
int poll(struct pollfd *fds, nfds_t nfds, int mille_timeout)
{	struct timeval timeout;
	timeout.tv_sec=mille_timeout/1000;         
	timeout.tv_usec=1000000*mille_timeout%1000; 
//	std::vector<fd_set> fd(2*nfds);
	struct fd_set* fd=(fd_set*) malloc(2*nfds*sizeof(fd_set));
	if(!fd)
	{	return -1;
	}
	u_int* const readerCount=&fd[0].fd_count;
	*readerCount=0;
	SOCKET* fdReader=fd[0].fd_array;
	int writer=nfds;
	u_int* const writerCount=&fd[nfds].fd_count;
	*writerCount=0;
	SOCKET* fdWriter=fd[nfds].fd_array;
	for(int i=0;i<nfds;i++)
	{	if(fds[i].events & POLLIN)
		{	fdReader[*readerCount]=fds[i].fd;
			(*readerCount)++;
		}
		if(fds[i].events & POLLOUT)
		{	fdWriter[*writerCount]=fds[i].fd;
			(*writerCount)++;
	}	}
	fd_set fdExcept;
	fdExcept.fd_count=0;
	const int ok = select(nfds, &fd[0], &fd[nfds], &fdExcept, &timeout);
	free(fd);
	return ok;
}

inline
int ppoll(struct pollfd *fds, nfds_t nfds,const struct timespec *tmo_p, const sigset_t *sigmask)
{   STUB_NEG(ppoll);
}

#ifdef __cplusplus
}
#endif

#endif
