// dirent.h
// Created by Robin Rowe on 2016/8/30
// Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef dirent_h
#define dirent_h

#include <io.h>
#include <string.h>

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
	dirent()
	:	d_ino(0)
	,	d_reclen(0)
	,	d_namlen(DT_UNKNOWN)
	,	d_name(nullptr)
	{}
};

struct DIR
{	_finddata_t	dd_dta;
	dirent dd_dir;
	long dd_handle;
	enum STATUS
	{	off_the_end = -1,
		next_entry_is_first_entry = 0,
		positive = 1 //0-based index of next entry
	};
	short dd_stat;
	unsigned char d_type;      
	char dd_name[1];
	DIR()
	:	dd_handle(-1)
	,	dd_stat(next_entry_is_first_entry)
	,	d_type(DT_UNKNOWN)
	{	dd_name[0] = 0;
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
void rewinddir (DIR*);
long telldir (DIR*);
void seekdir (DIR*, long);
int scandir(const char* buf, dirent*** namelist, scandir_f sf, scandir_alphasort af);

#endif
