// dirent.cpp
// Created by Robin Rowe on 2016/8/30
// Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include "unistd.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <list>
#include <string>
#include "dirent.h"

#define SUFFIX	'*'
#define	SLASH	'\\'

DIR* opendir(const char *path)
{	errno = 0;
	if(!path)
	{	errno = EFAULT;
		return nullptr;
	}
	if(!path[0])
	{	errno = ENOTDIR;
		return nullptr;
	}
    const unsigned int rc = GetFileAttributesA(path);
	if(rc == -1)
	{	errno = ENOENT;
		return nullptr;
	}
	if (!(rc & FILE_ATTRIBUTE_DIRECTORY))
	{	errno = ENOTDIR;
		return nullptr;
	}
	size_t pathLength = strlen (path);
	const size_t size = sizeof (DIR) + pathLength + 2;
	DIR* dir = (DIR *) malloc(size);
	if (!dir)
	{	errno = ENOMEM;
		return nullptr;
	}
	strcpy (dir->dd_name,path);
	if(pathLength)
	{	const char c = dir->dd_name[pathLength-1];
		if(c != '/' && c != '\\')
		{	dir->dd_name[pathLength++] = SLASH;
			dir->dd_name[pathLength] = 0;
	}	}
	dir->dd_name[pathLength++] = SUFFIX;
	dir->dd_name[pathLength] = 0;
	dir->dd_handle = -1;
	dir->dd_stat = 0;
	dir->dd_dir.d_ino = 0;
	dir->dd_dir.d_reclen = 0;
	dir->dd_dir.d_namlen = 0;
	dir->dd_dir.d_name = dir->dd_dta.name;
	return dir;
}

struct dirent* readdir(DIR* dir)
{	errno = 0;
	if(!dir)
	{	errno = EFAULT;
		return nullptr;
	}
	if(dir->dd_dir.d_name != dir->dd_dta.name)
	{	errno = EINVAL;
		return nullptr;
	}
	if(dir->dd_stat < 0)
	{	return nullptr;	
	}
	if(dir->dd_stat == 0)
	{	dir->dd_handle = (long) _findfirst (dir->dd_name, &(dir->dd_dta));
		if(dir->dd_handle == -1)
		{	dir->dd_stat = -1;
			return nullptr;
		}
		dir->dd_stat = 1;
	}
	else
	{	if(_findnext(dir->dd_handle,&(dir->dd_dta)))
		{	_findclose (dir->dd_handle);
			dir->dd_handle = -1;
			dir->dd_stat = -1;
			return nullptr;
		}
		dir->dd_stat++;
	}
	dir->dd_dir.d_namlen = (unsigned short) strlen(dir->dd_dir.d_name);
	return &dir->dd_dir;
}

int readdir_r(DIR *dir, struct dirent *entry, struct dirent** result)
{	(void)entry;
    dirent* d = readdir(dir);
	if(!d)
	{	return -1;
	}
	*result = d;
	return 0;
}

int closedir(DIR* dir)
{	errno = 0;
	if (!dir)
	{	errno = EFAULT;
		return -1;
	}
	int rc = 0;
	if(dir->dd_handle != -1)
	{	rc = _findclose (dir->dd_handle);
	}
	free (dir);
	return rc;
}

void rewinddir(DIR * dir)
{	errno = 0;
	if (!dir)
	{	errno = EFAULT;
		return;
	}
	if(dir->dd_handle != -1)
	{	_findclose (dir->dd_handle);
	}
	dir->dd_handle = -1;
	dir->dd_stat = 0;
}

long telldir(DIR * dir)
{	errno = 0;
	if (!dir)
	{	errno = EFAULT;
		return -1;
	}
	return dir->dd_stat;
}

void seekdir(DIR * dir, long lPos)
{	errno = 0;
	if (!dir)
	{	errno = EFAULT;
		return;
	}
	if(lPos < -1)
	{	errno = EINVAL;
		return;
	}
	if(lPos == -1)
	{	if(dir->dd_handle != -1)
		{	_findclose (dir->dd_handle);
		}
		dir->dd_handle = -1;
		dir->dd_stat = -1;
	}
	else
	{	rewinddir (dir);
		while ((dir->dd_stat < lPos) && readdir (dir))
		;	
	}
}

int alphaqsort(const void* a,const void* b)
{	void* a2 = const_cast<void*>(a);
	void* b2 = const_cast<void*>(b);
	return alphasort(reinterpret_cast<const dirent**>(a2),reinterpret_cast<const dirent**>(b2));
}

unsigned GetFileCount(DIR* dir,scandir_f selector)
{	unsigned count = 0;
	for(;;)
	{	dirent* d = readdir(dir);
		if(!d)
		{	break;
		}
		if(selector != NULL && !(*selector)(d))
		{	continue;
		}
		count++;
	}
	rewinddir(dir);
	return count;
}

int scandir(const char* dirname, dirent*** namesList, scandir_f selector, scandir_alphasort sorter)
{	DIR* dir = opendir(dirname);
	if(!dir)
	{	return -1;
	}
	const unsigned count = GetFileCount(dir,selector);
	if(!count)
	{	return -1;
	}
	dirent** names = (dirent **) malloc(count * sizeof(dirent*));
	if(!names)
	{	return -1;
	}
	*namesList = names;
	int matches = 0;
	for(unsigned i = 0; i < count; i++)
	{	dirent* d = readdir(dir);
		if(!d)
		{	break;
		}
		if(selector != NULL && !(*selector)(d))
		{	continue;
		}
		matches++;
		**names = *d;
		const size_t length = strlen(d->d_name);
		names[i]->d_name = (char*) malloc(length+1);
		if(names[i]->d_name)
		{	strcpy(names[i]->d_name,d->d_name);
		}
		names++;
	}
	closedir(dir);
	if(sorter) 
	{	qsort(dir, matches, sizeof(*dir),alphaqsort);
	}
	return matches;
}

