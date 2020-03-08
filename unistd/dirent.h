// dirent.h
// Created by Robin Rowe on 2016/8/30
// Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef dirent_h
#define dirent_h

#include <io.h>
#include <string.h>
#include "unistd.h"

enum
{	DT_BLK,		// This is a block device.
	DT_CHR,		// This is a character device.
	DT_DIR,		// This is a directory.
	DT_FIFO,	// This is a named pipe (FIFO).
	DT_LNK,		// This is a symbolic link.
	DT_REG,		// This is a regular file.
	DT_SOCK,	// This is a UNIX domain socket.
	DT_UNKNOWN	// The file type could not be determined.
};

struct dirent
{	long d_ino;
	unsigned short d_reclen;
	unsigned short d_namlen;
	unsigned char d_type;
	char* d_name;
};

struct DIR
{	void* p;
};

typedef int scandir_f(const struct dirent* d);
typedef int scandir_alphasort(const struct dirent** a,const struct dirent** b);
int alphasort(const struct dirent** a,const struct dirent** b);
int versionsort(const struct dirent** a,const struct dirent** b);

typedef struct DIR DIR;
struct DIR* opendir(const char* path);
struct dirent* readdir(struct DIR* dir);
int readdir_r(struct DIR* dir,struct dirent* entry, struct dirent** result);
int	closedir(struct DIR* dir);
void rewinddir(struct DIR* dir);
long telldir(struct DIR* dir);
void seekdir(struct DIR* dir,long tell);
int scandir(const char* buf,struct dirent*** namelist,scandir_f sf,scandir_alphasort af);

#endif
