// sys/sem.h 
// 2018/10/29 Robin.Rowe@Cinepaint.org
// License open source MIT

#ifndef sys_sem_h
#define sys_sem_h

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "../../portable/stub.h"
#include "../cfunc.h"

#ifndef SEMS_MAX_COUNT
#define SEMS_MAX_COUNT 255
#endif
#define SEMMSL 128

struct semid_ds 
{	ipc_perm sem_perm;  /* Ownership and permissions */
    time_t          sem_otime; /* Last semop time */
    time_t          sem_ctime; /* Last change time */
    unsigned long   sem_nsems; /* No. of semaphores in set */
};

enum
{	SEM_UNDO,
	GETNCNT,
	GETPID,
	GETVAL,
	GETALL,
	GETZCNT,
	SETVAL,
	SETALL,
};

struct semaphore 
{	unsigned short int semval;//  semaphore value
	pid_t sempid; //process ID of last operation
	unsigned short int semncnt; //number of processes waiting for semval to become greater than current value
	unsigned short int semzcnt; //number of processes waiting for semval to become 0
};

struct sembuf 
{	unsigned short int sem_num; // semaphore number
	short int sem_op; // semaphore operation
	short int sem_flg; // operation flags
};

/*
The argument nsems can be 0 (a don't care) when a semaphore set is
not being created.  Otherwise, nsems must be greater than 0 and less
than or equal to the maximum number of semaphores per semaphore set
(SEMMSL).
*/

CFUNC int semget(key_t key, int nsems, int semflg);
CFUNC int semop(int semid, struct sembuf *sops, size_t nsops);

inline
int semtimedop(int semid, struct sembuf *sops, size_t nsops,const struct timespec *timeout)
{	STUB_NEG(semtimedop);
}

inline
int semctl(int, int, int, ...)
{	STUB_NEG(semctl);
}

#endif