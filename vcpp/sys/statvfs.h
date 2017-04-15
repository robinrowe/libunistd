// sys/statvfs.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_statvfs_h
#define sys_statvfs_h

#include <sys/vfs.h>
#include "../portable/stub.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

struct statvfs 
{	unsigned long  f_bsize;    /* Filesystem block size */
	unsigned long  f_frsize;   /* Fragment size */
	fsblkcnt_t     f_blocks;   /* Size of fs in f_frsize units */
	fsblkcnt_t     f_bfree;    /* Number of free blocks */
	fsblkcnt_t     f_bavail;   /* Number of free blocks for unprivileged users */
	fsfilcnt_t     f_files;    /* Number of inodes */
	fsfilcnt_t     f_ffree;    /* Number of free inodes */
	fsfilcnt_t     f_favail;   /* Number of free inodes for unprivileged users */
	unsigned long  f_fsid;     /* Filesystem ID */
	unsigned long  f_flag;     /* Mount flags */
	unsigned long  f_namemax;  /* Maximum filename length */
};

enum
{	ST_MANDLOCK,	// Mandatory locking is permitted on the filesystem (see fcntl(2)).
	ST_NOATIME,		// Do not update access times; see mount(2).
	ST_NODEV,		// Disallow access to device special files on this filesystem.
	ST_NODIRATIME,	// Do not update directory access times; see mount(2).
	ST_NOEXEC,		// Execution of programs is disallowed on this filesystem.
	ST_NOSUID,		// The set-user-ID and set-group-ID bits are ignored by exec(3) for executable files on this filesystem
	ST_RDONLY,		// This filesystem is mounted read-only.
	ST_RELATIME,	// Update atime relative to mtime/ctime; see mount(2).
	ST_SYNCHRONOUS,	// Writes are synched to the filesystem immediately (see the description of O_SYNC in open(2)).
};

// On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.
#if 0
int statvfs(const char *path, struct statvfs *buf)
{   STUB0(statvfs);
}

int fstatvfs(int fd, struct statvfs *buf)
{   STUB0(fstatvfs);
}
#endif
#ifdef __cplusplus
}
#endif

#endif
