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
	dirent()
	:	d_ino(0)
	,	d_reclen(0)
	,	d_namlen(0)
	,	d_type(DT_UNKNOWN)
	,	d_name(0)
	{}
};

struct DIR
{	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	dirent entry;
	bool isFirst;
	DIR()
	{	hFind = INVALID_HANDLE_VALUE;
		memset(&findFileData,0,sizeof(findFileData));
		entry.d_name = findFileData.cFileName;
		isFirst = true;
	}
	bool operator!() const
	{	return hFind == INVALID_HANDLE_VALUE;
	}
	void Set()
	{	entry.d_namlen = (int) strlen(entry.d_name);
		if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{	entry.d_type = DT_DIR;
		}
		else
		{	entry.d_type = DT_REG;
	}	}
	bool FindFirst(const char* path)
	{	hFind = FindFirstFile(path, &findFileData);
		if (!*this)
		{	return false;
		}
		Set();
		return true;
	}
	dirent* Find()
	{	if(isFirst)
		{	isFirst = false;
			return &entry;
		}
		if(!FindNextFile(hFind,&findFileData))
		{	return 0;
		}
		Set();
		return &entry;
	}
	int Close()
	{	const BOOL ok = FindClose(hFind);
		return ok ? 0 : -1;
	}
};

typedef int scandir_f(const struct dirent* d);
typedef int scandir_alphasort(const dirent** a,const dirent** b);

inline
int alphasort(const dirent** a,const dirent** b)
{	if(!a || !b)
	{	return 0;
	}
	return strcmp((*a)->d_name,(*b)->d_name);
}

DIR* opendir (const char*);
dirent*	readdir (DIR*);
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
int	closedir (DIR*);
#if 0
void rewinddir (DIR*);
long telldir (DIR*);
void seekdir (DIR*, long);
#endif
int scandir(const char* buf, dirent*** namelist, scandir_f sf, scandir_alphasort af);

#endif
