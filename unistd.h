/* unistd.h: replaces *nix header of same name
// Windows emulation of common *nix functions
// Copyright Nov 10, 2002, Robin.Rowe@MovieEditor.com
// License MIT (http://opensource.org/licenses/mit-license.php)
*/

// link with Ws2_32.lib! 

#ifndef WIN32_UNISTD_H
#define WIN32_UNISTD_H
#define _CRT_SECURE_NO_DEPRECATE 
#undef _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#if _MSC_VER == 1900
#include <vcruntime.h>
#include <corecrt_io.h>
#endif
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <winnt.h>
#undef socklen_t
#include <WS2tcpip.h>
#include <windows.h>
#include <math.h>
#include <fcntl.h>
#include <process.h>
#include <io.h>
#include <malloc.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <direct.h>
#include <winerror.h>
#include <memory.h>
#include <mswsock.h> //for SO_UPDATE_ACCEPT_CONTEXT
#include <Ws2tcpip.h>//for InetNtop
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "uni_signal.h"
#include "stub.h"

#pragma comment(lib, "Ws2_32.lib")

#pragma warning (disable : 4996)

#define _WINSOCK_DEPRECATED_NO_WARNINGS

typedef long long useconds_t;

#ifdef __cplusplus
#include <thread>
#include <string>

#if 0
inline
std::string Now()
{	time_t now;
	now = time(NULL);
	return ctime(&now);
}
#endif

inline
std::string Now()
{	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft,&st);
// 2014-11-22 12:45:34.001
	const unsigned BUFSIZE=30;
	char buffer[BUFSIZE];
	sprintf_s(buffer,BUFSIZE,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
		st.wYear,
		st.wMonth,
//		st.wDayOfWeek,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);
	return buffer;
}

inline 
int mkdir(const char* path,int)
{	 return _mkdir(path);
}

inline
int uni_open(const char* filename,unsigned oflag)
{	return _open(filename,oflag,0);
}

inline
int uni_open(const char* filename,unsigned oflag,int mode)
{	return _open(filename,oflag,mode);
}

inline
int fcntl(int handle,int mode,int mode2)
STUB0(fcntl)

inline
int usleep(useconds_t delay)
{	std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
	return 0;
}

extern "C" {
#else
#define inline __inline

inline
int usleep(useconds_t delay)
STUB0(usleep)

#endif

inline 
size_t safe_strlen(const char* s)
{	if(!s)
	{	puts("ERROR: strlen(null)");
		return 0;
	}
	return (s ? strlen(s):0);
}

#define strlen safe_strlen

//#define inet_ntop InetNtop
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

#ifndef strcasecmp
#define strcasecmp _stricmp
#endif
#define strncasecmp _strnicmp

#define strtok_r strtok_s

#define popen _popen
#define pclose _pclose

//#define send send2
typedef int ssize_t;

#define lstat stat
typedef unsigned short mode_t;

inline 
int mkdir(const char* path)
{	 return _mkdir(path);
}

typedef int pid_t;

inline
int kill(pid_t p, int x)
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
#define S_IXOTH S_IEXEC
#define S_IXGRP S_IEXEC
#define S_IRWXU _S_IEXEC|_S_IREAD|_S_IWRITE
#define S_IRWXG _S_IEXEC|_S_IREAD|_S_IWRITE
#define S_IROTH S_IREAD
#define S_IRGRP S_IREAD
#define S_IRWXO _S_IEXEC|_S_IREAD|_S_IWRITE
#define S_IWGRP S_IWRITE
#define S_IWOTH S_IWRITE
#define O_CLOEXEC 0
#define O_DIRECTORY _O_OBTAIN_DIR


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

typedef intptr_t pid_t;
typedef int gid_t;
typedef int uid_t;

#define fileno _fileno
#define STDIN_FILENO _fileno(stdin)

// causes issues with math.h:
//#define rint(x) floor ((x) + 0.5)
//#define lround floor
//#define roundl floor

#define getpid _getpid

inline 
int setgid(gid_t g)
{	return -1;
}

inline 
int setuid(uid_t g)
{	return -1;
}

#if 0
#define open uni_open
#define close _close
#define read uni_read
#define write uni_write
#define lseek _lseek
#define creat _creat
#define chdir _chdir

inline
int open(const char* filename,int oflag)
{	return _open(filename,oflag,0);
}

inline
int read(int fd,void *buffer,unsigned int count)
{	return _read(fd,buffer,count);
}

inline
int write(int fd,const void *buffer,unsigned int count)
{	return _write(fd,buffer,count);
}

#endif

enum
{	F_GETFL,
	F_SETFL,
	O_NONBLOCK
};


typedef int Atom;

inline
const char* getsysconfdir()
STUB0(getsysconfdir)

inline 
int mkstemp(char *filename)
{	char buffer[MAX_PATH];
	unsigned filenameNo = GetTempFileNameA(".",filename,0,buffer);
	return (int) filenameNo;
}

inline
int fchmod(int a, mode_t b)
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

inline
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

// seconds and nanosecond count of the time since the Epoch (00:00 1 January, 1970 UTC).

inline
int clock_gettime(clockid_t clk_id, struct timespec *tp)
{	if(CLOCK_REALTIME != clk_id)
	{	return -1;
	}
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);
	FILETIME fileTime;
	SystemTimeToFileTime(&systemTime,&fileTime);
	ULARGE_INTEGER now = *(ULARGE_INTEGER*) &fileTime;
	memset(&systemTime,0,sizeof(systemTime));
	systemTime.wYear = 1970;
	systemTime.wMonth = 1;
	systemTime.wDay = 1;
	SystemTimeToFileTime(&systemTime,&fileTime);
	ULARGE_INTEGER in1970 = *(ULARGE_INTEGER*) &fileTime;
	long long diff = now.QuadPart - in1970.QuadPart; //100-nanosecond intervals since January 1, 1970
	diff *= 100;//nanoseconds
	const long long nanosPerSecond = 1000*1000*1000;
	tp->tv_sec = diff/nanosPerSecond; // 31557600 seconds/year, * 35 years = 1,420,092,000, says 1,464,129,754
	tp->tv_nsec = diff%nanosPerSecond; // says 126,000,000, / 1,000,000,000 = .126
	return 0;
}

inline
int clock_settime(clockid_t clk_id, const struct timespec *tp)
STUB0(clock_settime)

inline
int fsync (int fd)
{	HANDLE h = (HANDLE) _get_osfhandle(fd);
	if (h == INVALID_HANDLE_VALUE)
	{	return -1;
	}
	if (!FlushFileBuffers (h))
	{	return -1;
	}
	return 0;
}

inline
void sync()
{	_flushall();
}

inline
int syncfs(int fd)
{	fsync(fd);
}

inline
int fcntl(int handle,int mode)
STUB0(fcntl)

// std::chrono::high_resolution_clock

#if _MSC_VER == 1800
struct timespec {
        time_t   tv_sec;        /* seconds */
        long     tv_nsec;       /* nanoseconds */
};
#endif

inline
int nanosleep(const struct timespec *req, struct timespec *rem)
{	long long delay = req->tv_sec;
	const long long billion = 1000000000LL;
	delay*=billion;
    delay+=req->tv_nsec;
	return usleep(delay);
}

inline
int gettimeofday(struct timeval* tv, struct timezone* tz)
{	FILETIME ft;
	ULARGE_INTEGER t;
	ULONGLONG x;
	ULONGLONG m=1000000;
	GetSystemTimeAsFileTime(&ft);
	t.LowPart=ft.dwLowDateTime;
	t.HighPart=ft.dwHighDateTime;
	x=t.QuadPart/m;
	tv->tv_sec=(long) x;
	x=t.QuadPart%m;
	tv->tv_usec=(long) x;
	return 0;
}

inline
int settimeofday(const struct timeval *tv, const struct timezone *tz)
STUB0(0)

inline
struct tm* localtime_r(const time_t* t,struct tm* result)
{	struct tm* r=localtime(t);
	*result=*r;
	return result;
}

#define O_NOCTTY 0
#define EBADFD 200
#define ESHUTDOWN 201
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH

#define MSG_NOSIGNAL 0
#define TCP_KEEPCNT 0
#define access _access

#pragma warning( error : 4013)
#pragma warning( error : 4047) 
#pragma warning(default:4996)

#ifdef __cplusplus
}
#endif

#endif
