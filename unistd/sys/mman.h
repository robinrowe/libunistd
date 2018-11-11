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
#include <WinIoCtl.h>

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
{	HANDLE fh;
	HANDLE mh;
	void* p;
	size_t length;
	ShmHandle(HANDLE fh = 0)
	:	fh(fh)
	,	mh(0)
	,	p(0)
	,	length(0)
	{}
	void Close()
	{	Flush();
		if(fh)
		{	CloseHandle(fh);
			fh=0;
		}
		if(mh)
		{	CloseHandle(mh);
			mh=0;
		}
		if(p)
		{	UnmapViewOfFile(p);
			p=0;
		}
		length = 0;
	}
	bool operator!() const
	{	return !fh;
	}
	bool Flush()
	{	if(!p || !length)
		{	return false;
		}
		BOOL ok = FlushViewOfFile(p,length);
		if(!ok)
		{	return false;
		}
		// FlushViewOfFile does not set file time!
		SYSTEMTIME st;
		GetSystemTime(&st);
		FILETIME ft;
		SystemTimeToFileTime(&st,&ft);
		const FILETIME* creationTime = 0;
		const FILETIME* lastAccessTime = &ft;
		const FILETIME* lastWriteTime = &ft;
		ok = SetFileTime(fh,creationTime,lastAccessTime,lastWriteTime);
//		FlushFileBuffers(fh);
		if(!ok)
		{	return false;
		}
		return true;
	}
};

class ShmMap
:	public std::vector<ShmHandle>
{public:
	bool IsInvalid(int i)
	{	if(0>=i)
		{	return true;
		}
		if(size() <= i-1)
		{	return true;
		}
		return false;
	}
	ShmHandle& operator[](int i)
	{	return std::vector<ShmHandle>::operator[](i-1);
	}
};

static ShmMap shmMap;

inline
int shm_open(const char *name, int oflag, mode_t mode)
{	DWORD flag = 0;
	if(oflag & O_RDWR)
	{	flag = GENERIC_READ | GENERIC_WRITE;
	}
	DWORD shareFlag = FILE_SHARE_READ | FILE_SHARE_WRITE;
	SECURITY_ATTRIBUTES* security = 0;
	HANDLE accessMode = 0;
	DWORD createFlag = OPEN_EXISTING;
	if(oflag & O_CREAT)
	{	createFlag = CREATE_ALWAYS;
	}
	DWORD attributes = FILE_ATTRIBUTE_NORMAL;//FILE_FLAG_DELETE_ON_CLOSE;
#pragma warning(disable : 4996)
// Normal user can't write to root dir in Windows!
	std::string filename = getenv("USERPROFILE");
#pragma warning(default : 4996)
	filename += "\\";
	filename += name+1;
/*
    printf("USERPROFILE = %s\n", getenv("USERPROFILE"));
    printf("HOMEDRIVE   = %s\n", getenv("HOMEDRIVE"));
    printf("HOMEPATH    = %s\n", getenv("HOMEPATH"));
USERPROFILE = C:\Users\rower
HOMEDRIVE   = C:
HOMEPATH    = \Users\rower
*/
	HANDLE hFile = CreateFileA(filename.c_str(),
			flag,
			shareFlag,
			security,
			createFlag,
			attributes,
			accessMode);
	if(INVALID_HANDLE_VALUE == hFile)
	{	int err = GetLastError();
		return -1;
	}
	shmMap.push_back(ShmHandle(hFile));
	return int(shmMap.size());
}

// Bug: Should honor prot and flags:
// PROT_READ|PROT_WRITE, MAP_SHARED, 
inline
void* mmap(void *addr, size_t length, int prot, int flags,int fd, off_t off)
{	if(shmMap.IsInvalid(fd))
	{	return MAP_FAILED;
	}
	ShmHandle& sh = shmMap[fd];
	sh.mh = CreateFileMapping( sh.fh,          // current file handle
		NULL,           // default security
		PAGE_READWRITE, // read/write permission
		0,              // size of mapping object, high
		(DWORD)length,  // size of mapping object, low
		NULL);          // name of mapping object
	if(!sh.mh)
	{	sh.Close();
		return MAP_FAILED;
	}
	sh.p = MapViewOfFile(sh.mh,
		FILE_MAP_ALL_ACCESS, // read/write
        0, // high-order 32 bits of file offset
        0, // low-order 32 bits of file offset
		length); // number of bytes to map
	if(!sh.p)
	{	sh.Close();
		return MAP_FAILED;
	}
	sh.length = length;
	return sh.p;
}

inline
int shm_close(int fd)
{	if(shmMap.IsInvalid(fd))
	{	return -1;
	}
	ShmHandle& sh = shmMap[fd];
	sh.Close();
	return 0;
}

inline
int shm_unlink(const char* name)
{	BOOL ok = DeleteFileA(name);
	if(!ok)
	{	return -1;
	}	
	return 0;
}

inline
int GetFilePointer(HANDLE hFile) 
{	return SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
}

inline
int shm_ftruncate(int fd, off_t length)
{	if(shmMap.IsInvalid(fd))
	{	return -1;
	}
	ShmHandle& sh = shmMap[fd];
	DWORD bytesReturned = 0;
	LPOVERLAPPED overlapped = 0;
#if 0
// This doesn't do anything, don't know why...
	DWORD control = FSCTL_SET_ZERO_DATA;
	FILE_ZERO_DATA_INFORMATION info;
	LARGE_INTEGER large;
	large.QuadPart = 0;
	info.FileOffset = large;
	large.LowPart = length;
	info.BeyondFinalZero = large;
	void* in = &info;
	DWORD inSize = sizeof(info);
	void* out = 0;
	DWORD outSize = 0;
	BOOL ok = DeviceIoControl(sh.fh,
		control,
		in,
		inSize,
		out,
		outSize,
		&bytesReturned,
		overlapped ); 
	if(!ok)
	{	return -1;
	}
	SetEndOfFile(sh.fh);
#else
	std::string s(length,0);
	BOOL ok = WriteFile(sh.fh,s.c_str(),length,&bytesReturned,overlapped);
//	printf("GetFilePointer = %d\n",GetFilePointer(sh.fh));
	if(!ok)
	{	return -1;
	}
#endif
	ok = FlushFileBuffers(sh.fh);
	if(!ok)
	{	return -1;
	}
	return 0;
}

inline
size_t shm_size(int fd)
{	if(shmMap.IsInvalid(fd))
	{	return 0;
	}
	ShmHandle& sh = shmMap[fd];
	DWORD size = GetFileSize(sh.fh,0);
	sh.length = size_t(size);
	return size_t(size);
}

inline
bool shm_flush(int fd)
{	if(shmMap.IsInvalid(fd))
	{	return false;
	}
	ShmHandle& sh = shmMap[fd];
	return sh.Flush();
}
#endif

/* NOTES: Why the implementation above doesn't use global shared memory...

Windows Insight Blog
Observations on Windows internals
Global CreateFileMapping under Vista UAC

By Christian Carrillo on September 24, 2007 7:50 AM 

Shared memory is an excellent substrate for inter-process communication (IPC), but in Vista when User Account Control things get more complicated -- processes running under user accounts can't create global file mappings, so old methods for IPC across different accounts don't work.

There are several solutions suggested online, including modifying the ObUnsecureGlobalNames registry key, changing account privileges and pre-creating the shared memory in a service.  These are unfortunately unacceptable for our needs -- customers don't want us to fiddle with their machine configurations or change their security settings (!!), and if these changes were undone we would encounter hard-to-debug issues.  Pre-creating doesn't work for SystemAI because we need an arbitrary number of IPC channels, and we don't want to hold application launches for a service to respond.

Fortunately there is a good solution: the Session namespace.  Rather than creating global file mappings (that is, in the Global kernel object namespace), we create our mappings in whatever session's namespace is most convenient and then use them from there.  It doesn't matter that a file mapping isn't in the Global namespace as long as its privileges are set to allow access from other sessions.

More specifically, Windows stores objects created in the Local namespace at \Session\[Current Session ID], and we can access them directly using the explicit Session path, even from other sessions.  So to open / create a global file mapping, we use the following procedure:

 

Read some global state somewhere (we use a file) to see in what session the mapping was last created.
Try opening the mapping in that session's local namespace (i.e. \Session\[Last Session ID]\[Mapping Name])
If the open failed, the last session ID is old.  Try CreateFileMapping in the global namespace (\Global\[Mapping Name]).
If this fails, then we don't have access to the global namespace and the mapping doesn't already exist in the global namespace (CreateFileMapping returns a handle to an existing mapping of the same name if there is one).  So we get the session ID (from ProcessIdToSessionId) and create the mapping in the local namespace (\Session\[Current Session ID]\[Mapping Name]).  Then we store the current session ID somewhere so other processes can open it from this session.
We did encounter an unexpected problem with step (2).  At first we were using OpenFileMapping as, necessarily, we only wanted to open an existing mapping in the local namespace, not create one if it didn't already exist.  But for some as-yet-unknown reason, MapViewOfFile would fail on anything returned from OpenFileMapping.  So we use CreateFileMapping for step (2) and check to make sure GetLastError() returns ERROR_ALREADY_EXISTS.  If not, we destroy the spurious mapping we just created.

Additionally, of course, we need to create everything with the appropriate security attributes.  Null Dacls don't grant global access in Vista, so we need to create a real security descriptior and add an ACE with GENERIC_ALL privileges for the world SID.

This procedure allows us to create globally accessible shared memory on Vista with UAC, without making any settings or security changes.

TrackBack URL for this entry: http://www.celceo.com/company/mt/mt-tb.cgi/4

*/
/*

GCC and Vista Incompatibility
Since ReactOS is still being built with GCC (unfortunately), some of our devs have started to report a problem when using the MinGW build under Windows Vista. The call to MapViewOfFileEx that the compiler users for precompiled header support fails, so the compilation fails for any project that uses a PCH.

This type of error might creep up in other system software as well, and it’s not really GCC’s fault for succumbing to it. If you look at the documentation for CreateFileMapping, you’ll notice this blurb in the Remarks section:

Creating a file mapping object from a session other than session zero requires the SeCreateGlobalPrivilege privilege. Note that this privilege check is limited to the creation of file mapping objects and does not apply to opening existing ones. For example, if a service or the system creates a file mapping object, any process running in any session can access that file mapping object provided that the caller has the required access rights.

Windows XP/2000: The requirement described in the previous paragraph was introduced with Windows Server 2003, Windows XP SP2 and Windows 2000 Server SP4.

Although this feature was added in SP2, the reason it doesn’t happen in Windows XP has to do with two changes in Vista. First, UAC means that programs don’t get the SeCreateGlobalPrivilege anymore, because they’re not running in administrator accounts anymore. Secondly, in Vista, Session 0 is now the SYSTEM account session, where the login screen and services are running. Therefore, any user processes will run in Session 1, even in a normal single-user system. These two factors combined mean that CreateFileMapping is now significantly reduced in functionality and that only services are allowed to create global shared memory.

There are three workarounds if you really need the functionality:

Use the Microsoft Management Console (MMC) and the Local Security Policy Snap-In to give SeCreateGlobalPrivilege to the limited account.
Write a wrapper program that executes with elevated rights and and uses RtlAcquire/AdjustPrivilege to get the privilege before running your target program (Such as gcc).
Use the HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager\Kernel\ObUnsecureGlobalNames string array to add the name of the section to the list. Hopefully your program isn’t randomizing the name. Adding this name will disable the kernel protection check.

http://www.alex-ionescu.com/?p=16

*/