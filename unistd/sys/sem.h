// sys/sem.h 
// 2018/10/29 Robin.Rowe@Cinepaint.org
// License open source MIT

#ifndef sys_sem_h
#define sys_sem_h

#include <sys/ipc.h>
#include <sys/types.h>
#include <vector>

class SemMap
:	public std::vector<HANDLE>
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
	HANDLE& operator[](int i)
	{	return std::vector<HANDLE>::operator[](i-1);
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

struct ipc_perm    
{	unsigned short sem_nsems;// number of semaphores in set
	time_t sem_otime;// last semop^) time
	time_t sem_ctime;// last time changed by semctl()
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

inline
int semget(key_t key, int nsems, int semflg)
{
	return -1;
}

inline
int semctl(int, int, int, ...)
{
	return -1;
}

inline
int semop(int, sembuf*, size_t)
{
	return -1;
}


#endif