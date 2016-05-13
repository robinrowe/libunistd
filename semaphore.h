// semaphore.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef semaphore_h
#define semaphore_h

#include "stub.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

typedef int sem_t;

inline
int sem_close(sem_t *)
STUB0(sem_close)

inline
int sem_destroy(sem_t *)
STUB0(sem_destroy)

inline
int sem_getvalue(sem_t *restrict, int *restrict2)
STUB0(sem_getvalue)

inline
int sem_init(sem_t *, int, unsigned)
STUB0(sem_init)

inline
sem_t *sem_open(const char *, int, ...)
STUB0(sem_open)

inline
int sem_post(sem_t *)
STUB0(sem_post)

inline
int sem_timedwait(sem_t *restrict, const struct timespec *restrict2)
STUB0(sem_timedwait)

inline
int sem_trywait(sem_t *)
STUB0(sem_trywait)

inline
int sem_unlink(const char *)
STUB0(sem_unlink)

inline
int sem_wait(sem_t *)
STUB0(sem_wait)

#ifdef __cplusplus
}
#endif

#endif