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
	unsigned short	d_reclen;
	unsigned short	d_namlen;
	unsigned char  d_type;
	char* d_name; // points to the name in the _finddata_t structure in the DIR
	char buffer[1];
};

struct DIR
{	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	struct dirent entry;
	int isFirst;
};

typedef int scandir_f(const struct dirent* d);
typedef int scandir_alphasort(const struct dirent** a,const struct dirent** b);

inline
int alphasort(const struct dirent** a,const struct dirent** b)
{	if(!a || !b)
	{	return 0;
	}
	return strcmp((*a)->d_name,(*b)->d_name);
}

inline
int versionsort(const struct dirent** a,const struct dirent** b)
{	if(!a || !b)
	{	return 0;
	}
	return strcmp((*a)->d_name,(*b)->d_name);
}

struct DIR* opendir (const char*);
struct dirent*	readdir (struct DIR*);
int readdir_r(struct DIR *dirp, struct dirent *entry, struct dirent **result);
int	closedir (struct DIR*);
#if 0
void rewinddir (DIR*);
long telldir (DIR*);
void seekdir (DIR*, long);
#endif
int scandir(const char* buf,struct dirent*** namelist, scandir_f sf, scandir_alphasort af);

#endif
