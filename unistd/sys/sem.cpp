// sys/sem.cpp
// 2018/10/29 Robin.Rowe@Cinepaint.org
// License open source MIT

#include <unistd.h>
#include <vector>
#include <map>
#include <string>
#include "sem.h"

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
	SysSems()
	{	memset(&semid,0,sizeof(semid));
	}
};

static std::map<key_t,SysSems> semMap;

/*
The argument nsems can be 0 (a don't care) when a semaphore set is
not being created.  Otherwise, nsems must be greater than 0 and less
than or equal to the maximum number of semaphores per semaphore set
(SEMMSL).
*/

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
