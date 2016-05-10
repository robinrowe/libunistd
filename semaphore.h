// semaphore.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef semaphore_h
#define semaphore_h

#include "stub.h"
#include <time.h>

typedef int sem_t;

int sem_close(sem_t *)
STUB0()

int sem_destroy(sem_t *)
STUB0()

int sem_getvalue(sem_t *restrict, int *restrict)
STUB0()

int sem_init(sem_t *, int, unsigned)
STUB0()

sem_t *sem_open(const char *, int, ...)
STUB0()

int sem_post(sem_t *)
STUB0()

int sem_timedwait(sem_t *restrict, const struct timespec *restrict)
STUB0()

int sem_trywait(sem_t *)
STUB0()

int sem_unlink(const char *)
STUB0()

int sem_wait(sem_t *)
STUB0()

#endif