// sys/wait.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_wait_h
#define sys_wait_h

#include "sys/sys_types.h"
#include "../portable/stub.h"
#include "sigaction.h"

#define WIFEXITED(wstatus) 1
#define WEXITSTATUS(wstatus) 0

inline
pid_t wait(int *status)
{	STUB_NEG(wait);
}

inline
pid_t waitpid(pid_t pid, int *status, int options)
{	STUB_NEG(waitpid);
}

typedef int idtype_t;
typedef int id_t;

inline
int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options)
{	STUB_NEG(waitid);
}

#endif
