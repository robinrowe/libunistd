// sys/file.cpp
// Copyright (c) 2022/10/19 Tomer Lev <tomerlev@microsoft.com>
// License open source MIT

#include <unistd.h>
#include <errno.h>

#include "file.h"

BOOL file_size(HANDLE h, DWORD * lower, DWORD * upper)
{	*lower = GetFileSize(h, upper);
	return 1;
}

BOOL do_lock(HANDLE h, int non_blocking, int exclusive)
{	BOOL res;
	DWORD size_lower, size_upper;
	OVERLAPPED ovlp;
	int flags = 0;
	/* We're going to lock the whole file, so get the file size. */
	res = file_size(h, &size_lower, &size_upper);
	if ( ! res )
	{	return 0;
	}
	/* Start offset is 0, and also zero the remaining members of this struct. */
	memset(&ovlp, 0, sizeof ovlp);
	if ( non_blocking )
	{	flags |= LOCKFILE_FAIL_IMMEDIATELY;
	}
	if ( exclusive )
	{	flags |= LOCKFILE_EXCLUSIVE_LOCK;
	}
	return LockFileEx(h, flags, 0, size_lower, size_upper, &ovlp);
}

BOOL do_unlock(HANDLE h)
{	int res;
	DWORD size_lower, size_upper;
	res = file_size(h, &size_lower, &size_upper);
	if ( ! res )
	{	return 0;
	}
	return UnlockFile(h, 0, 0, size_lower, size_upper);
}

int flock(int fd, int operation)
{	HANDLE h = (HANDLE)_get_osfhandle(fd);
	DWORD res;
	int non_blocking;
	if ( h == INVALID_HANDLE_VALUE )
	{	errno = EBADF;
		return -1;
	}
	non_blocking = operation & LOCK_NB;
	operation &= ~LOCK_NB;
	switch ( operation )
	{	case LOCK_SH:
			res = do_lock(h, non_blocking, 0);
			break;
		case LOCK_EX:
			res = do_lock(h, non_blocking, 1);
			break;
		case LOCK_UN:
			res = do_unlock(h);
			break;
		default:
			errno = EINVAL;
			return -1;
	}
	/* Map Windows errors into Unix errnos.  As usual MSDN fails to
	 * document the permissible error codes.
	 */
	if ( ! res )
	{	DWORD err = GetLastError();
		switch ( err )
		{	case ERROR_LOCK_VIOLATION:
				errno = EAGAIN;
				break;
			case ERROR_NOT_ENOUGH_MEMORY:
				errno = ENOMEM;
				break;
			case ERROR_BAD_COMMAND:
				errno = EINVAL;
				break;
			default:
				errno = err;
				break;
		}
		return -1;
	}
	return 0;
}
