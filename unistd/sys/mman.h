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

struct ShmHandle
{	HANDLE h;
	void* p;
	ShmHandle(HANDLE h,void* p)
	:	h(h)
	,	p(p)
	{}
	void Clear()
	{	h=0;
		p=0;
	}
	bool operator!() const
	{	return !h;
	}
};

static std::vector<ShmHandle> shmMap;

inline
int shm_open(const char *name, int oflag, mode_t mode)
{	std::string s("Global\\");
	s += name+1;
	DWORD flag = 0;
	if(oflag|O_RDWR)
	{	flag = PAGE_READWRITE;
	}
	HANDLE hMapFile = 0;
	if(oflag|O_CREAT)
	{	hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE, // use paging file
		NULL, // default security
		flag, // read/write access
		0,  // maximum object size (high-order DWORD)
		100,  // maximum object size (low-order DWORD)
		s.c_str()); // name of mapping object
	}
	else
	{   hMapFile = OpenFileMapping(
                   FILE_MAP_ALL_ACCESS,   // read/write access
                   FALSE,                 // do not inherit the name
                   name);               // name of mapping object
	}	
	if(!hMapFile)
	{	int err = GetLastError();
		return -1;
	}
	shmMap.push_back(ShmHandle(hMapFile,0));
	return int(shmMap.size());
}

inline
void* mmap(void *addr, size_t len, int prot, int flags,int fd, off_t off)
{	if(0>=fd || shmMap.size() <= fd-1)
	{	return MAP_FAILED;
	}
	ShmHandle mh = shmMap[fd-1];
	void* p = MapViewOfFile(mh.h,FILE_MAP_ALL_ACCESS,0,0,len);
	if(!p)
	{	CloseHandle(mh.h);
		shmMap[fd-1].Clear();
		return MAP_FAILED;
	}
	return p;
}

inline
int shm_close(int fd)
{	if(0>=fd || shmMap.size() <= fd-1)
	{	return -1;
	}
	ShmHandle mh = shmMap[fd-1];
	if(mh.p)
	{	UnmapViewOfFile(mh.p);
	}	
	BOOL ok = CloseHandle(mh.h);
	shmMap[fd-1].Clear();
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
{	if(0>=fd || shmMap.size() <= fd-1)
	{	return -1;
	}
	ShmHandle mh = shmMap[fd-1];
	const BOOL ok = SetEndOfFile(mh.h);
	if(!ok)
	{	return -1;
	}	
	return 0;
}

#endif
