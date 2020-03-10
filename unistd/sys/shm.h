// sys/shm.h
// Robin Rowe 2020/3/7

#ifndef sys_shm_h
#define sys_shm_h

#include <sys/sys_types.h>
#include <sys/stat.h>
#include <memory.h>
#include "../../portable/stub.h"

typedef int shmatt_t;

struct shmid_ds 
{	struct ipc_perm shm_perm;    /* Ownership and permissions */
	size_t          shm_segsz;   /* Size of segment (bytes) */
	time_t          shm_atime;   /* Last attach time */
	time_t          shm_dtime;   /* Last detach time */
	time_t          shm_ctime;   /* Last change time */
	pid_t           shm_cpid;    /* PID of creator */
	pid_t           shm_lpid;    /* PID of last shmat(2)/shmdt(2) */
	shmatt_t        shm_nattch;  /* No. of current attaches */
};

inline
int shmget(key_t key, size_t size, int shmflg)
{	STUB_NEG(shmget);
}

inline
int shmctl(int shmid, int cmd, struct shmid_ds *buf)
{	STUB_NEG(shmctl);
}

inline
void *shmat(int shmid, const void *shmaddr, int shmflg)
{	STUB_0(shmat);
}

inline
int shmdt(const void *shmaddr)
{	STUB_NEG(shmdt);
}

#endif

 