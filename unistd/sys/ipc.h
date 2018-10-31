// sys/ipc.h

#ifndef sys_ipc_h
#define sys_ipc_h

#include <sys/sys_types.h>
#include <sys/stat.h>
#include "../../portable/stub.h"

struct ipc_perm 
{	uid_t uid;
	gid_t gid;
	uid_t cuid;
	gid_t cgid;
	mode_t mode;
	ipc_perm()
	{	memset(this,0,sizeof(*this));
	}
};

enum
{	IPC_CREAT,
	IPC_EXCL,
	IPC_NOWAIT,
	IPC_PRIVATE,
	IPC_RMID,
	IPC_SET,
	IPC_STAT
};

key_t ftok(const char*, int)
{	STUB_NEG(ftok);
}

#endif

 