// mman.h
// Copyright 2016/6/27 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef mman_h
#define mman_h

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#define MAP_FAILED (void *) -1

// On success, mmap() returns a pointer to the mapped area, on error, the value MAP_FAILED 
//         p = mmap(nullptr, UIMAGE_SIZE, PROT_READ, MAP_SHARED | MAP_LOCKED, fd, 0xFC0A0000));
//On success, munmap() returns 0, on failure -1

#define MAP_SHARED 0
#define MAP_PRIVATE 0
#define MAP_32BIT 0
#define MAP_ANON 0
#define MAP_ANONYMOUS 0
#define MAP_DENYWRITE 0
#define MAP_EXECUTABLE 0
#define MAP_FILE 0
#define MAP_FIXED 0
#define MAP_GROWSDOWN 0
#define MAP_HUGETLB 0
#define MAP_LOCKED 9
#define MAP_NONBLOCK 0
#define MAP_NORESERVE 0
#define MAP_POPULATE 0
#define MAP_STACK 0
#define MAP_UNINITIALIZED 0

#define PROT_EXEC PAGE_WRITECOPY
#define PROT_READ PAGE_READONLY
#define PROT_WRITE PAGE_READWRITE
#define PROT_NONE PAGE_NOACCESS

static std::vector<HANDLE> mapHandle;

inline
int shm_open(const char *name, int oflag, mode_t mode)
{	std::string s("Global\\");
	s += name+1;
	HANDLE hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE, // use paging file
		NULL, // default security
		oflag, // read/write access
		0,  // maximum object size (high-order DWORD)
		0,  // maximum object size (low-order DWORD)
		&s[0]); // name of mapping object
	if(!hMapFile)
	{	return -1;
	}
	mapHandle.push_back(hMapFile);
	return int(mapHandle.size());
}

inline
void* mmap(void *addr, size_t len, int prot, int flags,int fd, off_t off)
{	if(0>=fd || mapHandle.size() <= fd-1)
	{	return MAP_FAILED;
	}
	HANDLE hMapFile = mapHandle[fd-1];
	void* p = MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,len);
	if(!p)
	{	//CloseHandle(hMapFile);
		return MAP_FAILED;
	}
	return p;
}

inline
int shm_close(int fd)
{	if(0>=fd || mapHandle.size() <= fd-1)
	{	return -1;
	}
	HANDLE hMapFile = mapHandle[fd-1];
	BOOL ok = CloseHandle(hMapFile);
	if(!ok)
	{	return -1;
	}	
	return 0;
}

inline
int shm_unlink(const char *name)
{	BOOL ok = DeleteFileA(name);
	if(!ok)
	{	return -1;
	}	
	return 0;
}

inline
int shm_ftruncate(int fd, off_t length)
{	if(0>=fd || mapHandle.size() <= fd-1)
	{	return -1;
	}
	HANDLE hMapFile = mapHandle[fd-1];
	const BOOL ok = SetEndOfFile(hMapFile);
	if(!ok)
	{	return -1;
	}	
	return 0;
}

#endif
