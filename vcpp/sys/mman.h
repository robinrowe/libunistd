// mman.h
// Copyright 2016/6/27 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef mman_h
#define mman_h

#include <sys/types.h>
#include <memory.h>
#include <io.h>

#define MAP_FAILED (void *) -1

// On success, mmap() returns a pointer to the mapped area, on error, the value MAP_FAILED 
//         p = mmap(nullptr, UIMAGE_SIZE, PROT_READ, MAP_SHARED | MAP_LOCKED, fd, 0xFC0A0000));
inline
void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset)
{	char* p = new char[length];
	memset(p,0,length);
	const int bytes = _read(fd,p,(unsigned) length);
	return p;
}

//On success, munmap() returns 0, on failure -1

int munmap(void* addr, size_t length)
{	(void) addr;
	(void) length;
	return 0;
}

#define PROT_EXEC 1
#define PROT_READ 2
#define PROT_WRITE 4
#define PROT_NONE 8

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

#endif
/*
#include <WinSock2.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <malloc.h>

#define PROT_EXEC PAGE_WRITECOPY
#define PROT_READ PAGE_READONLY
#define PROT_WRITE PAGE_READWRITE
#define PROT_NONE PAGE_NOACCESS

void *mmap(void *addr, size_t len, int prot, int flags,int fd, off_t off); 
{	const unsigned bufsize=80;
	char name[bufsize];
	sprintf_s(name,bufsize,"Global\\%u",(unsigned)fd);
	HANDLE hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE, // use paging file
		NULL, // default security
		prot, // read/write access
		0,    // maximum object size (high-order DWORD)
		len,  // maximum object size (low-order DWORD)
		szName); // name of mapping object
	if(!hMapFile)
	{	return MAP_FAILED;
	}
	void* p = MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,len);
	if(!p)
	{	CloseHandle(hMapFile);
		return MAP_FAILED;
	}
	return p;
}
*/