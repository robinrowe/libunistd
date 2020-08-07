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
#define _CRT_NONSTDC_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <winnt.h>
#include <corecrt_io.h>
//#if _MSC_VER == 1900
#include <vcruntime.h>
#undef socklen_t
#include <WS2tcpip.h>
#include <windows.h>
#include <math.h>
#include <fcntl.h>
#include <process.h> // getpid()
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
#include <inttypes.h>
#include "sys/sys_types.h"
#include "../portable/bsd_string.h"
#include "uni_signal.h"
#include "../portable/stub.h"
#include "gettimeofday.h"

// Use: cmake -A x64 ..

#ifdef __cplusplus
	#define CFUNC extern "C"
#else
	#define CFUNC extern
#endif

CFUNC char *optarg;
CFUNC int optind;
CFUNC int opterr;
CFUNC int optopt;

#ifdef __cplusplus
#include "chrono.h"
#ifdef _M_X64
#include "int128/Int128.h"
#endif

inline
pid_t getpgrp() /* POSIX.1 version */
{	STUB_0(getpgrp);
}

inline
pid_t getpgrp(pid_t pid) /* BSD version */
{	(void)pid;
	STUB_0(getpgrp);
}

inline
int setpgrp() /* System V version */
{	STUB_0(setpgrp);
}

inline
int setpgrp(pid_t pid, pid_t pgid) /* BSD version */ 
{	(void)pid;
	(void)pgid;
	STUB_0(setpgrp);
}

#else
//#define inline __inline
	typedef long long useconds_t;
#endif

#pragma warning(disable : 4996)

/*
inline
int read(int fh,void* buf,unsigned count)
{	return _read(fh,buf,count);
}
*/

inline
int snprintb(char *buf, size_t buflen, const char *fmt, uint64_t val)
{	(void)buf;
	(void)buflen;
	(void)fmt;
	(void)val;
	STUB_0(snprintb);
}

inline
int snprintb_m(char *buf, size_t buflen, const char *fmt, uint64_t val,size_t max)
{	(void)buf;
	(void)buflen;
	(void)fmt;
	(void)val;
	(void)max;
	STUB_0(snprintb_m);
}

inline
int uni_open(const char* filename,unsigned oflag,int mode)
{	return _open(filename,oflag,mode);
}

#define mkdir mkdir2

inline
int mkdir2(const char* path, int mask)
{	(void) mask;
	return _mkdir(path);
}

#ifdef __cplusplus

inline
int uni_open(const char* filename, unsigned oflag)
{
	return _open(filename, oflag, 0);
}

inline
int fcntl(int handle, int mode)
{
	(void)handle;
	(void)mode;
	STUB_0(fcntl);
}

#endif

inline
int fcntl(int handle,int mode,int mode2)
{	(void)handle;
	(void)mode;
	(void)mode2;
	STUB_0(fcntl);
}

//#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
{	if(!input || !*input || !format)
	{	return 0;
	}
	const size_t length = strlen(input);
	va_list argList;
	va_start(argList,format);// BUG/BROKEN: should be count of args, not format
#pragma warning(disable:4996)
	const int retval = _snscanf(input,length,format,argList);
	va_end(argList);
	input[length-1]=0;
	return retval;
}

//#define sscanf uni_sscanf

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
{	return stream ? _pclose(stream):-1;
}

//#define send send2
#define lstat stat

inline
int kill(pid_t p, int x)
{	(void)p;
	(void)x;
	return -1;
}

inline
int S_ISCHR(int v) 
{	(void)v;
	return 0; 
}

inline
int S_ISBLK(int v) 
{	(void)v;
	return 0;
}

inline
int S_ISFIFO(int v) 
{	(void)v;
	return 0;
}

inline
int S_ISSOCK(int v) 
{	(void)v;
	return 0;
}

#define fileno _fileno
#define STDIN_FILENO _fileno(stdin)

// causes issues with math.h:
//#define rint(x) floor ((x) + 0.5)
//#define lround floor
//#define roundl floor

inline
pid_t gettid()
{	HANDLE h = GetCurrentThread();
	return (intptr_t) h;
}

inline 
int setgid(gid_t g)
{	(void)g;
	return -1;
}

inline 
int setuid(uid_t g)
{	(void)g;
	return -1;
}

inline
const char* getsysconfdir()
{	STUB_0(getsysconfdir);
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
	STUB_NEG(fchmod);
}

inline
uid_t getuid()
{	STUB_NEG(getuid);
}

inline
uid_t geteuid()
{	STUB_NEG(geteuid);
}

inline
gid_t getgid()
{	STUB_NEG(getgid);
}

inline
gid_t getegid()
{	STUB_NEG(getegid);
}

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
	STUB_0(readlink);
}

#if _MSC_VER <= 1900

inline
int gethostname(char *name, size_t len)
{   DWORD bufsize = (DWORD) len;
    BOOL ok = GetComputerNameA(name,&bufsize);
    return ok? 0:-1;
}

#endif

inline
char *getlogin()
{	STUB_0(getlogin);
}

inline
int getlogin_r(char *buf, size_t len)
{   DWORD bufsize = (DWORD) len;
    BOOL ok = GetUserNameA(buf,&bufsize);
    return ok? 0:-1;
}

CFUNC int getopt(int argc, char * const argv[],const char *optstring);

inline
void PrintDirectory()
{	const char* path = _getcwd(0,0);
	if(!path)
	{	perror("getcwd() error");
		return;
	}
	printf("pwd = %s\n", path);
	free((void*)path);
}

inline
unsigned int alarm(unsigned int seconds)
{	(void)seconds;
	STUB_0(alarm);
}

inline
int chown(const char *path, uid_t owner, gid_t group)
{	(void)path;
	(void)owner;
	(void)group;
	STUB_0(chown);
}

inline
int fchown(int fd, uid_t owner, gid_t group)
{	(void)fd;
	(void)owner;
	(void)group;
	STUB_0(fchown);
}

inline
int lchown(const char *path, uid_t owner, gid_t group)
{	(void)path;
	(void)owner;
	(void)group;
	STUB_0(lchown);
}

inline
int chroot(const char *path)
{	(void)path;
	STUB_NEG(chroot);
}

inline
size_t confstr(int name, char *buf, size_t len)
{	(void)name;
	(void)buf;
	(void)len;
	STUB_0(confstr);
}

inline
char *ctermid(char *s)
{	char* term = "/dev/tty";
	if(s)
	{	strcpy(s,term);
		return s;
	}
	return term;
}

// The POSIX name for this item is deprecated by MSVC:

#define write _write
#define unlink _unlink
#define rmdir _rmdir
#define read _read
#define lseek _lseek
#define isatty _isatty
#define getcwd _getcwd
#define dup2 _dup2
#define dup _dup
#define close _close
#define chdir _chdir

/*
inline
int chdir(const char *path)
{	return _chdir(path);
}*/

inline
int fchdir(int fd)
{	(void)fd;
	STUB_NEG(fchdir);
}

inline
pid_t fork()
{	STUB_NEG(fork);
}

inline
int getdtablesize()
{	STUB_0(getdtablesize);
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
int fdatasync(int fd)
{	(void)fd;
	STUB_NEG(fdatasync);
}

inline
long fpathconf(int fd, int name)
{	(void)fd;
	(void)name;
	STUB_NEG(fpathconf);
}

inline
long pathconf(const char *path, int name)
{	(void)path;
	(void)name;
	STUB_NEG(pathconf);
}

inline
long gethostid()
{	STUB_NEG(gethostid);
}

inline
int sethostid(long hostid)
{	(void)hostid;
	STUB_NEG(sethostid);
}

// char *cuserid(char *string); 

inline
int getpagesize()
{	STUB_0(getpagesize);
}

inline
char *getpass(const char *prompt)
{	(void)prompt;
	STUB_0(getpass);
}

inline
int setpgid(pid_t pid, pid_t pgid)
{	(void)pid;
	(void)pgid;
	STUB_0(setpgid);
}

inline
pid_t getpgid(pid_t pid)
{	(void)pid;
	STUB_0(getpgid);
}

#define getpid _getpid

/*

In process.h:

inline
pid_t getpid()
{	return _getpid();
}
*/

inline
pid_t getppid()
{	STUB_0(getppid);
}

inline
int link(const char *oldpath, const char *newpath)
{	(void)oldpath;
	(void)newpath;
	STUB_NEG(link);
}

#define F_LOCK 1
#define F_TLOCK 2
#define F_ULOCK 3
#define F_TEST 4

inline
int lockf(int fd, int cmd, off_t len)
{	(void)fd;
	(void)cmd;
	(void)len;
	STUB_NEG(lockf);
}

inline
int nice(int inc)
{	(void)inc;
	STUB_NEG(nice);
}

inline
int pause()
{	STUB_NEG(pause);
}

inline
int brk(void *addr)
{	(void)addr;
	STUB_NEG(brk);
}

inline
void *sbrk(intptr_t increment)
{	(void)increment;
	STUB_0(sbrk);
}

inline
int setreuid(uid_t ruid, uid_t euid)
{	(void)ruid;
	(void)euid;
	STUB_NEG(setreuid);
}

inline
int setregid(gid_t rgid, gid_t egid)
{	(void)rgid;
	(void)egid;
	STUB_NEG(setregid);
}

inline
int setsid()
{	STUB_NEG(setsid);
}

inline
int symlink(const char *target, const char *linkpath)
{	(void)target;
	(void)linkpath;
	STUB_NEG(symlink);
}

inline
long sysconf(int name)
{	(void)name;
	STUB_NEG(sysconf);
}

inline
pid_t tcgetpgrp(int fd)
{	(void)fd;
	STUB_NEG(tcgetpgrp);
}

inline
int tcsetpgrp(int fd, pid_t pgrp)
{	(void)fd;
	(void)pgrp;
	STUB_NEG(tcsetpgrp);
}

inline
char *ttyname(int fd)
{	(void)fd;
	STUB_0(ttyname);
}

inline
int ttyname_r(int fd, char *buf, size_t buflen)
{	(void)fd;
	(void)buf;
	(void)buflen;
	STUB_NEG(ttyname_r);
}

inline
useconds_t ualarm(useconds_t usecs, useconds_t interval)
{	(void)usecs;
	(void)interval;
	STUB_0(ualarm);
}

inline
pid_t vfork()
{	STUB_NEG(vfork);
}

/* between 0.0 and 1.0 */
inline
double drand48(void)
{	double r=(double)rand();
	r/=RAND_MAX;
	return r > 1.0? 1.0:r;
}

/*double srand48(time_t);*/
inline
void srand48(long int seedval)
{	srand(seedval);
}

inline
long int random(void)
{	return rand();
}

#define RETSIGTYPE void

#ifdef __cplusplus
extern "C"
{
#else
inline
unsigned sleep(unsigned seconds)
{	Sleep(1000*seconds);
	return 0;
}

inline
int usleep(useconds_t usec)
{	LARGE_INTEGER time1;
	LARGE_INTEGER time2;
	LARGE_INTEGER freq;
	time1.QuadPart = 0;
	time2.QuadPart = 0;
	freq.QuadPart = 0;
	QueryPerformanceCounter(&time1);
	QueryPerformanceFrequency(&freq);
	do 
	{	QueryPerformanceCounter(&time2);
	} while((time2.QuadPart-time1.QuadPart) < usec);
	return 0;
}
#endif

int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);
int truncate(const char *path, off_t length);

inline
int ftruncate(int fd, off_t length)
{	return _chsize(fd,length);
}

#ifdef __cplusplus
}
#endif

#define access _access

#pragma warning( error : 4013)
#pragma warning( error : 4047) 
#pragma warning(default : 4996)
#define   __attribute__(x)
//__attribute__((format (printf, 1, 2)));
#endif

