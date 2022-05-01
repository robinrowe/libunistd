// dirent.cpp
// Created by Robin Rowe on 2016/8/30
// Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <io.h>
#include <direct.h>
//#include <list>
//#include <string>
#include "dirent.h"
#include "../portable/Finder.h"

#define SUFFIX	'*'
#define	SLASH	'\\'

typedef portable::Finder Finder;

extern "C" {

DIR* opendir(const char *path)
{	errno = 0;
	if(!path)
	{	errno = EFAULT;
		return 0;
	}
	if(!path[0])
	{	errno = ENOTDIR;
		return 0;
	}
	Finder* finder = (Finder*) malloc(sizeof(Finder));
	if (!finder)
	{	errno = ENOMEM;
		return 0;
	}
	finder->Reset();
	if(!finder->Open(path))
	{	errno = ENOENT;
		//	errno = ENOTDIR;
		free(finder);
		return 0;
	}
	return (DIR*) finder;
}

int readdir_r(DIR *dir, struct dirent*, struct dirent** entry)
{	errno = 0;
	if(!dir)
	{	errno = EFAULT;
		return 0;
	}	
	Finder* finder = (Finder*) dir;
	if(!finder->Read())
	{	if(finder->IsEof())
		{	*entry = 0;
			return 0;
		}
		errno = EBADF;
		return EBADF;
	}
	finder->Set(*entry);
	return 0;
}

struct dirent* readdir(DIR* dir)
{	static dirent entry;
	dirent* p = &entry;
	const int error = readdir_r(dir,0,&p);
	if(0!=error)
	{	return 0;
	}
	return p;
}

void rewinddir(DIR* dir)
{	errno = 0;
	if (!dir)
	{	errno = EFAULT;
		return;
	}
	Finder* finder = (Finder*) dir;
	finder->Rewind();
}

long telldir(DIR* dir)
{	errno = 0;
	if (!dir)
	{	errno = EFAULT;
		return -1;
	}
	Finder* finder = (Finder*) dir;
	return finder->Tell();
}

void seekdir(DIR* dir,long lPos)
{	errno = 0;
	if (!dir)
	{	errno = EFAULT;
		return;
	}
	if(lPos < -1)
	{	errno = EINVAL;
		return;
	}
	Finder* finder = (Finder*) dir;
	if(finder->Tell() > lPos)
	{	finder->Rewind();
	}
	finder->Seek(lPos);
}

int closedir(DIR* dir)
{	errno = 0;
	if(!dir)
	{	errno = EFAULT;
		return -1;
	}
	Finder* finder = (Finder*) dir;
	finder->Close();
	free(dir);
	return 0;
}

int alphaqsort(const void* v1, const void* v2)
{	dirent** d1 = (dirent**)v1;
	dirent** d2 = (dirent**)v2;
	const char* name1 = (*d1)->d_name;
	const char* name2 = (*d2)->d_name;
	return strcmp(name1,name2);
}

int alphasort(const struct dirent** a,const struct dirent** b)
{	if(!a || !b)
	{	return 0;
	}
	return strcmp((*a)->d_name,(*b)->d_name);
}

int versionsort(const struct dirent** a,const struct dirent** b)
{	if(!a || !b)
	{	return 0;
	}
	return strcmp((*a)->d_name,(*b)->d_name);
}

unsigned GetFileCount(DIR* dir,scandir_f selector)
{	unsigned count = 0;
	dirent entry;
	dirent* p = &entry;
	for(;;)
	{	if(!readdir_r(dir,0,&p) || !p)
		{	break;
		}
		if(selector != NULL && !(*selector)(p))
		{	continue;
		}
		count++;
	}
	return count;
}

int scandir(const char* path, dirent*** namesList, scandir_f selector, scandir_alphasort sorter)
{	DIR* dir = opendir(path);
	if(!dir)
	{	return -1;
	}
	const unsigned count = GetFileCount(dir,selector);
	if(!count)
	{	return -1;
	}
	closedir(dir);
	dirent** names = (dirent**) malloc(count * sizeof(dirent*));
	if(!names)
	{	return -1;
	}
	dir = opendir(path);
	if (!dir)
	{	return -1;
	}
	*namesList = names;
	int matches = 0;
	for(unsigned i = 0; i < count; i++)
	{	dirent* entry = (dirent*) malloc(sizeof dirent);
		dirent** p = &entry;
		readdir_r(dir,0,p);
		if(!entry)
		{	break;
		}
		if(selector != NULL && !(*selector)(entry))
		{	continue;
		}
		names[matches] = entry;
		matches++;
	}
	closedir(dir);
	if(sorter) 
	{	qsort(names, matches, sizeof(dirent*),alphaqsort);
	}
	return matches;
}

}
