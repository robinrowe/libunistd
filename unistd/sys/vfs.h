// sys/vfs.h
// Libunistd Copyright 2017 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_vfs_h
#define sys_vfs_h

#include <sys/stat.h>
#include "../../portable/stub.h"

inline
int statfs(const char *path, struct statfs *buf)
{   STUB_NEG(statfs);
}

inline
int fstatfs(int fd, struct statfs *buf)
{   STUB_NEG(fstatfs);
}

typedef short __fsword_t;
typedef int fsblkcnt_t;
typedef int fsfilcnt_t;
typedef int fsid_t;

struct statfs
{   __fsword_t f_type;    /* Type of filesystem */
    __fsword_t f_bsize;   /* Optimal transfer block size */
    fsblkcnt_t f_blocks;  /* Total data blocks in filesystem */
    fsblkcnt_t f_bfree;   /* Free blocks in filesystem */
    fsblkcnt_t f_bavail;  /* Free blocks available to
                            unprivileged user */
    fsfilcnt_t f_files;   /* Total file nodes in filesystem */
    fsfilcnt_t f_ffree;   /* Free file nodes in filesystem */
    fsid_t     f_fsid;    /* Filesystem ID */
    __fsword_t f_namelen; /* Maximum length of filenames */
    __fsword_t f_frsize;  /* Fragment size (since Linux 2.6) */
    __fsword_t f_flags;   /* Mount flags of filesystem (since Linux 2.6.36) */
    __fsword_t f_spare[1]; /* Padding bytes reserved for future use */
};

#endif

