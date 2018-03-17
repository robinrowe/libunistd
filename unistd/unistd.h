/* unistd.h: replaces *nix header of same name
// Windows emulation of common *nix functions
// Copyright Nov 10, 2002, Robin.Rowe@CinePaint.org
// License MIT (http://opensource.org/licenses/mit-license.php)
*/

#ifndef unistd_h
#define unistd_h

#pragma comment(lib, "Ws2_32.lib")

#if  ((defined(_WINDOWS_) || defined(_INC_WINDOWS))) && !defined(WIN32_LEAN_AND_MEAN)
#error unistd.h must be included before Windows.h or #define WIN32_LEAN_AND_MEAN
#endif

//#define _CRT_SECURE_NO_DEPRECATE 
//#undef _CRT_SECURE_NO_WARNINGS
//#define _CRT_NONSTDC_NO_WARNINGS
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
#include <assert.h>
#include "uni_signal.h"
#include "../portable/stub.h"
#include "gettimeofday.h"
// Use: cmake -A x64 ..
#ifdef _M_X64
#include "Int128.h"
#endif
#ifdef __cplusplus
#include "chrono.h"
#include <cinttypes>
#pragma warning(disable : 4996)

inline
int snprintb(char *buf, size_t buflen, const char *fmt, uint64_t val)
{	(void)buf;
	(void)buflen;
	(void)fmt;
	(void)val;
	STUB0(snprintb);
}

inline
int snprintb_m(char *buf, size_t buflen, const char *fmt, uint64_t val,size_t max)
{	(void)buf;
	(void)buflen;
	(void)fmt;
	(void)val;
	(void)max;
	STUB0(snprintb_m);
}

inline
int uni_open(const char* filename,unsigned oflag,int mode)
{	return _open(filename,oflag,mode);
}

//overloaded C++ functions:

inline 
int mkdir(const char* path,int)
{	 return _mkdir(path);
}

inline
int fcntl(int handle,int mode,int mode2)
{	(void)handle;
	(void)mode;
	(void)mode2;
	STUB0(fcntl);
}

#else
#pragma warning(disable : 4996)
#define inline __inline
#endif

//#define _WINSOCK_DEPRECATED_NO_WARNINGS

inline
int uni_open(const char* filename,unsigned oflag)
{	return _open(filename,oflag,0);
}

inline 
size_t safe_strlen(const char* s)
{	if(!s)
	{	puts("ERROR: strlen(null)");
		return 0;
	}
	return (s ? strlen(s):0);
}

//#define strlen safe_strlen

//#define inet_ntop InetNtop

#define bzero(address,size) memset(address,0,size)
#define pow10(x) pow(x,10)
#define alloca _alloca

/* use with limits.h */
#define LONG_LONG_MAX LLONG_MAX     
#define LONG_LONG_MIN LLONG_MIN     

#define strdup _strdup
#define vsnprintf _vsnprintf

inline
int uni_sscanf(char* input,const char* format,...)
{	if(!input || !format)
	{	return 0;
	}
	const size_t length = strlen(input);
	va_list argList;
	va_start(argList,format);
#pragma warning(suppress:4996)
	const int retval = _snscanf(input,length-1,format,argList);
	va_end(argList);
	input[length-1]=0;
	return retval;
}

#define sscanf uni_sscanf

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

inline
FILE *popen(const char *command, const char *type)
{	
#ifdef _DEBUG
	printf("popen(%s,%s)\n",command,type);
#endif
	return _popen(command,type);
}

inline
int pclose(FILE *stream)
{	return _pclose(stream);
}

//#define send send2
typedef int ssize_t;

#define lstat stat
typedef unsigned short mode_t;

inline 
int mkdir(const char* path)
{	 return _mkdir(path);
}

inline
int kill(pid_t p, int x)
{	(void)p;
	(void)x;
	return -1;
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
#define S_IRWXU S_IRUSR|S_IWUSR|S_IXUSR
#define S_IRWXG S_IRGRP|S_IWGRP|S_IXGRP
#define S_IRWXO S_IROTH|S_IWOTH|S_IXOTH
//#define S_IRWXU _S_IEXEC|_S_IREAD|_S_IWRITE
//#define S_IRWXO _S_IEXEC|_S_IREAD|_S_IWRITE
//#define S_IRWXG _S_IEXEC|_S_IREAD|_S_IWRITE
#define S_IROTH S_IREAD
#define S_IRGRP S_IREAD
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

#define fileno _fileno
#define STDIN_FILENO _fileno(stdin)

// causes issues with math.h:
//#define rint(x) floor ((x) + 0.5)
//#define lround floor
//#define roundl floor

#define getpid _getpid

typedef int gid_t;

inline 
int setgid(gid_t g)
{	(void)g;
	return -1;
}

typedef int uid_t;

inline 
int setuid(uid_t g)
{	(void)g;
	return -1;
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

inline
const char* getsysconfdir()
{	STUB0(getsysconfdir);
}

inline 
int mkstemp(char *filename)
{	char buffer[MAX_PATH];
	unsigned filenameNo = GetTempFileNameA(".",filename,0,buffer);
	return (int) filenameNo;
}

inline
int fchmod(int a, mode_t b)
{	(void)a;
	(void)b;
	STUB0(fchmod);
}

inline
uid_t getuid()
{	STUB0(getuid);
}

inline
uid_t geteuid()
{	STUB0(geteuid);
}

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
ssize_t readlink(const char *path, char *buf, size_t bufsize)
{	(void)path;
	(void)buf;
	(void)bufsize;
	STUB0(readlink);
}

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
{	(void)handle;
	(void)mode;
	STUB0(fcntl);
}

#define EBADFD 200
#define ESHUTDOWN 201
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#define SHUT_RDWR SD_BOTH

#define MSG_NOSIGNAL 0
#ifndef TCP_KEEPCNT
#define TCP_KEEPCNT 0
#endif
#define access _access

#define F_GETFL 0
#define F_SETFL 0
#define O_NONBLOCK 0
#define O_SYNC 0
#define O_NOCTTY 0

typedef int Atom;

#if _MSC_VER <= 1900

inline
int gethostname(char *name, size_t len)
{   DWORD bufsize = (DWORD) len;
    BOOL ok = GetComputerNameA(name,&bufsize);
    return ok? 0:-1;
}

#endif

inline
int getlogin_r(char *buf, size_t len)
{   DWORD bufsize = (DWORD) len;
    BOOL ok = GetUserNameA(buf,&bufsize);
    return ok? 0:-1;
}

#pragma warning( error : 4013)
#pragma warning( error : 4047) 


#endif
