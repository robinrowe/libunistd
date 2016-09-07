// sys/epoll.h
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_ioctl_h
#define sys_ioctl_h

#include "../../portable/stub.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

inline
int epoll_create(int size)
STUB0(epoll_create)

inline
int epoll_create1(int flags)
STUB0(epoll_create1)

typedef union epoll_data 
{	void *ptr;
	int fd;
	uint32_t u32;
	uint64_t u64;
} epoll_data_t;

struct epoll_event 
{	uint32_t events;
	epoll_data_t data;
};

enum
{	EPOLL_CTL_ADD,
	EPOLL_CTL_MOD,
	EPOLL_CTL_DEL
};

enum
{	EPOLLIN,
	EPOLLOUT,
	EPOLLRDHUP,
	EPOLLPRI,
	EPOLLERR,
	EPOLLHUP,
	EPOLLET,
	EPOLLONESHOT,
	EPOLLWAKEUP,
	EPOLLEXCLUSIVE
};

inline
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
STUB0(epoll_ctl)

int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout)
STUB0(epoll_wait)

int epoll_pwait(int epfd, struct epoll_event *events,int maxevents, int timeout,const sigset_t *sigmask)
STUB0(epoll_pwait)

#ifdef __cplusplus
}
#endif

#endif

