// sys/sem.h 
// 2018/10/29 Robin.Rowe@Cinepaint.org
// License open source MIT

#ifndef sys_sem_h
#define sys_sem_h

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <vector>
#include <map>
#include "../../portable/stub.h"

#ifndef SEMS_MAX_COUNT
#define SEMS_MAX_COUNT 255
#endif
#define SEMMSL 128

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
{	key_t key;
	semid_ds semid;
	std::vector<SysSem> sem;
	SysSems(size_t size)
	{	sem.resize(size);
	}
};

static std::map<key_t,SysSems> semMap;

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

inline
int semget(key_t key, int nsems, int semflg)
{	auto id = semMap.find(key);
	if(semflg&IPC_CREAT&IPC_EXCL)
	{	if(id != semMap.end())
		{	errno = EEXIST;
			return -1;
	}	}
	if(key!=IPC_PRIVATE || IPC_CREAT&semflg)
	{	if(id == semMap.end())
		{	return -1;
		}
		return key;
	}
	if(nsems <= 0 || nsems > SEMMSL)
	{	return -1;
	}
	SysSems sysSems(nsems);
	LPSECURITY_ATTRIBUTES lpSemaphoreAttributes = 0;
	for(int i=0;i<nsems;i++)
	{	LONG lInitialCount = 0;
		LONG lMaximumCount = SEMS_MAX_COUNT;
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
		sysSems.key = key;
	}
	return 0;
}

inline
int semop(int semid, struct sembuf *sops, size_t nsops)
{
/*
//	if(sem_flg == IPC_NOWAIT and SEM_UNDO.
	for(SysSems& sems : semMap)
	{	if(sems.sem_op>0)
		{	sems.semval += sems.sem_op);
			if(SEM_UNDO)
			{	sems.semadj -=sems.sem_op);
			}
			continue;
		}
		if(0==sem_op)
		{	if(!sems.semval)
			{	if(IPC_NOWAIT&sem_flg)
				errno = EAGAIN;//        none of the operations in sops is performed
				return -1;
		}	}
*/
	STUB_NEG(semop);
}

inline
int semtimedop(int semid, struct sembuf *sops, size_t nsops,const struct timespec *timeout)
{	STUB_NEG(semtimedop);
}

inline
int semctl(int, int, int, ...)
{	STUB_NEG(semctl);
}

#endif