// sys/sem.h 
// 2018/10/29 Robin.Rowe@Cinepaint.org
// License open source MIT

#ifndef sys_sem_h
#define sys_sem_h

#include <sys/ipc.h>
#include <sys/types.h>
#include <vector>
#include "../../portable/stub.h"

struct semid_ds 
{	ipc_perm sem_perm;  /* Ownership and permissions */
    time_t          sem_otime; /* Last semop time */
    time_t          sem_ctime; /* Last change time */
    unsigned long   sem_nsems; /* No. of semaphores in set */
	semid_ds()
	{	memset(this,0,sizeof(*this));
	}
};

struct SysSem
{	HANDLE h;
};

struct SysSems
{	std::vector<SysSem> sem;
	semid_ds semid;
	SysSems(size_t size)
	{	sem.resize(size);
	}
};

class SemMap
:	public std::vector<SysSems>
{public:
	bool IsInvalid(int i)
	{	if(0>=i)
		{	return true;
		}
		if(size() <= i-1)
		{	return true;
		}
		return false;
	}
	SysSems& operator[](int i)
	{	return std::vector<SysSems>::operator[](i-1);
	}
};

static SemMap semMap;

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
A new set of nsems semaphores is created if key has the value
IPC_PRIVATE or if no existing semaphore set is associated with key
and IPC_CREAT is specified in semflg.

If semflg specifies both IPC_CREAT and IPC_EXCL and a semaphore set
already exists for key, then semget() fails with errno set to EEXIST.
(This is analogous to the effect of the combination O_CREAT | O_EXCL
for open(2).)

When creating a new semaphore set, semget() initializes the set's
associated data structure, semid_ds (see semctl(2)), as follows:

    sem_perm.cuid and sem_perm.uid are set to the effective user
    ID of the calling process.

    sem_perm.cgid and sem_perm.gid are set to the effective group
    ID of the calling process.

    The least significant 9 bits of sem_perm.mode are set to the
    least significant 9 bits of semflg.

    sem_nsems is set to the value of nsems.

    sem_otime is set to 0.

    sem_ctime is set to the current time.

The argument nsems can be 0 (a don't care) when a semaphore set is
not being created.  Otherwise, nsems must be greater than 0 and less
than or equal to the maximum number of semaphores per semaphore set
(SEMMSL).
*/

inline
int semget(key_t key, int nsems, int semflg)
{	int id = (int) semMap.size();
	SysSems sysSems(nsems);
	LPSECURITY_ATTRIBUTES lpSemaphoreAttributes = 0;
	for(int i=0;i<nsems;i++)
	{	LONG lInitialCount = 0;
		LONG lMaximumCount = 1;
		std::string s = std::to_string(key);
		LPCSTR lpName = s.c_str();
		sysSems.sem[i].h = CreateSemaphoreA(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
		if(INVALID_HANDLE_VALUE == sysSems.sem[i].h)
		{	return -1;
		}
		sysSems.semid.sem_perm.cuid = GetCurrentProcessId();
		const unsigned mask = 1<<9;
		sysSems.semid.sem_perm.mode = semflg & mask;
		sysSems.semid.sem_otime = time(0); 
		sysSems.semid.sem_ctime = sysSems.semid.sem_otime; 
		sysSems.semid.sem_nsems = nsems; 

	}
	return 0;
}

inline
int semop(int, sembuf*, size_t)
{	STUB_NEG(semop);
}

inline
int semctl(int, int, int, ...)
{	STUB_NEG(semctl);
}

#endif