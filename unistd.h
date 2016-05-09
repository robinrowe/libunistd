/* unistd.h: replaces *nix header of same name
// Windows emulation of common *nix functions
// Copyright Nov 10, 2002, Robin.Rowe@MovieEditor.com
// License MIT (http://opensource.org/licenses/mit-license.php)
*/

// link with Ws2_32.lib! 

#ifndef WIN32_UNISTD_H
#define WIN32_UNISTD_H

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <winnt.h>
#undef socklen_t
#include <WS2tcpip.h>
#include <windows.h>
#include <fcntl.h>
#include <process.h>
#include <io.h>
#include <malloc.h>
#include <time.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <sys/utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <math.h>
#include <memory>
#include <Winerror.h>
#include <mswsock.h> //for SO_UPDATE_ACCEPT_CONTEXT
#include <Ws2tcpip.h>//for InetNtop
#include <iostream>
#include <ctype.h>
#include <thread>
#include "uni_signal.h"
#include "stub.h"

#pragma comment(lib, "Ws2_32.lib")

#ifndef __cplusplus
#define inline __inline
#endif

#ifdef Fl_H

#include <string.h>
inline 
int fl_strlen(const char* s)
{	return int (s ? strlen(s):0);
}

#define strlen fl_strlen

#endif

//#define inet_ntop InetNtop

inline
int nanosleep(const struct timespec *req, struct timespec *rem)
{	long long delay = req->tv_sec;
	const long long billion = 1000000000LL;
	delay*=billion;
    delay+=req->tv_nsec;
	std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
	return 0;
}

#define sleep(x) Sleep(x)
#define bzero(address,size) memset(address,0,size)
#define pow10(x) pow(x,10)
#define alloca _alloca

/* use with limits.h */
#define LONG_LONG_MAX LLONG_MAX     
#define LONG_LONG_MIN LLONG_MIN     
/* #define ULLONG_MAX */

#define snprintf _snprintf
#define strdup _strdup
/*#define snwprintf _snwprintf*/
#define vsnprintf _vsnprintf

#undef MAX_PRIORITY /* remove winspool.h warning */

#define strcasecmp _stricmp
#define strncasecmp _strnicmp

#define popen _popen
#define pclose _pclose

//#define send send2
typedef int ssize_t;

#define lstat stat
typedef unsigned short mode_t;

inline 
int mkdir(const char* path,int=0)
{	 return _mkdir(path);
}

typedef int pid_t;

inline
int kill(pid_t, int)
{	return -1;
}

#define R_OK 4
#define W_OK 2
#define X_OK 0
#define F_OK 0

#ifndef S_ISREG
#define S_ISREG(x) (_S_IFREG & x)
#endif

#define S_ISLNK(x) 0

#ifndef S_ISDIR
#define S_ISDIR(x) (_S_IFDIR & x)
#endif

#define S_IXUSR _S_IEXEC
#define S_IRUSR _S_IREAD
#define S_IWUSR _S_IWRITE
#define S_IXOTH 0
#define S_IXGRP 0
#define S_IRWXU 0
#define S_IRWXG 0
#define S_IROTH 0
#define S_IRGRP 0

/*

From WIN32 sys/stat.h:

#define S_IFMT   _S_IFMT
#define S_IFDIR  _S_IFDIR
#define S_IFCHR  _S_IFCHR
#define S_IFREG  _S_IFREG
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC  _S_IEXEC

*/

/* strings.h */
int ffs(int i);

#define STDIN_FILENO fileno(stdin)
#define rint(x) floor ((x) + 0.5)
#define getpid _getpid

typedef int pid_t;
typedef int gid_t;
typedef int uid_t;

inline 
int setgid(gid_t )
{	return -1;
}

inline 
int setuid(uid_t )
{	return -1;
}

#define lround floor
#define roundl floor

// Call this function before using Winsock in a console app!

#pragma warning (disable : 4996)

#define close _close

inline
int uni_open(const char* filename,int oflag)
{	return _open(filename,oflag,0);
}

inline
int uni_open(const char* filename,unsigned oflag)
{	return _open(filename,oflag,0);
}

#define open uni_open
#define read uni_read

inline
int uni_read(int fd,void *buffer,unsigned int count)
{	return _read(fd,buffer,count);
}

inline
int write(int fd,const void *buffer,unsigned int count)
{	return _write(fd,buffer,count);
}

int fcntl(int handle,int mode,int mode2=0)
{	return 0;
}

enum
{	F_GETFL,
	F_SETFL,
	O_NONBLOCK
};

typedef int Atom;

inline
const char* getsysconfdir()
{	return 0;
}

inline 
int mkstemp(char *filename)
{	char buffer[MAX_PATH];
	unsigned filenameNo = GetTempFileNameA(".",filename,0,buffer);
	return (int) filenameNo;
}

inline
int fchmod(int, mode_t)
STUB0(fchmod)

inline
uid_t getuid()
STUB0(getuid)

inline
uid_t geteuid()
STUB0(geteuid)

#define PATH_MAX 255

inline
char* realpath(const char *path, char *resolved_path)
{	if(!resolved_path)
	{	return 0;
	}
	const DWORD  err = GetFullPathNameA(path,(DWORD) PATH_MAX,resolved_path,0);
	if(err)
	{	return 0;
	}
	return resolved_path;
}

#define strtok_r strtok_s
#define strcasecmp _stricmp

inline
int strncasecmp(const char *s1, const char *s2, size_t n)
{	for(unsigned i=0;i<n;i++)
	{	if(s1[i] == 0 && s2[i] == 0)
		{	return 0;
		}
		if(s1[i] == 0 || s2[i] == 0)
		{	return s1[i]<s2[i] ? -1:1;
		}
		if(tolower(s1[i])!=tolower(s2[i]))
		{	return s1[i]<s2[i] ? -1:1;
	}	}
	return 0;
}

ssize_t readlink(const char *path, char *buf, size_t bufsiz)
STUB0(readlink)

typedef int clockid_t;

enum
{	CLOCK_REALTIME,
	CLOCK_MONOTONIC,
	CLOCK_PROCESS_CPUTIME_ID,
	CLOCK_THREAD_CPUTIME_ID
};

inline
int clock_getres(clockid_t clk_id, struct timespec *res)
STUB0(clock_getres)

inline
int clock_gettime(clockid_t clk_id, struct timespec *tp)
STUB0(clock_gettime)

inline
int clock_settime(clockid_t clk_id, const struct timespec *tp)
STUB0(clock_settime)





#pragma warning( error : 4013)
#pragma warning( error : 4047) 

#endif
