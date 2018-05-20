// sys/wait.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_wait_h
#define sys_wait_h

#include "../portable/stub.h"

#define WIFEXITED(wstatus) 1
#define WEXITSTATUS(wstatus) 0

inline
pid_t wait(int *status)
{	STUB0(wait);
}

inline
pid_t waitpid(pid_t pid, int *status, int options)
{	STUB0(waitpid);
}

typedef int idtype_t;
typedef int id_t;

inline
int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options)
{	STUB0(waitid);
}

#endif
