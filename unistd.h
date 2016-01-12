/* win32/unistd.h: replaces *nix header of same name
// Windows emulation of common *nix functions
// Copyright Nov 10, 2002, Robin.Rowe@MovieEditor.com
// License MIT (http://opensource.org/licenses/mit-license.php)
*/

// link with Ws2_32.lib! 

#ifndef WIN32_UNISTD_H
#define WIN32_UNISTD_H

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
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
//#define inet_ntop InetNtop
#pragma comment(lib, "Ws2_32.lib")

#ifndef __cplusplus
#define inline __inline
#endif

#include "fix_vcast.h"

#ifdef Fl_H

#include <string.h>
inline 
int fl_strlen(const char* s)
{	return int (s ? strlen(s):0);
}

#define strlen fl_strlen

#endif

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
int mkdir(const char* path,int flags=0)
{	 return _mkdir(path);
}

typedef int pid_t;

inline
int kill(pid_t pid, int sig)
{	return -1;
}

#define F_OK 0
#define X_OK 0
#define W_OK 2
#define R_OK 1

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

/*
SIGABRT Abnormal termination 
SIGFPE Floating-point error 
SIGILL Illegal instruction 
SIGINT CTRL+C signal 
SIGSEGV Illegal storage access 
SIGTERM Termination request 
*/

#define SIGALRM NSIG+1
#define SIGHUP NSIG+2
#define SIGQUIT NSIG+3
#define SIGBUS NSIG+4
#define SIGPIPE NSIG+5
#define SIGKILL SIGTERM
#define SIGCHLD NSIG+6
#define SIGUSR1 NSIG+7
#define SIGUSR2 NSIG+8

#define WNOHANG 0

/* SIGTERM */
/* SIGFPE */
#if 0
/* 7-1-06 rsr
None of these POSIX signal handlers are implemented by libunistd.
Included here only as potential future implementation effort */
typedef int sigset_t;
typedef int siginfo_t;
#define SIG_SETMASK 0
#define SIGTSTP 0
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set, int signum);
int sigdelset(sigset_t *set, int signum);
int sigismember(const sigset_t *set, int signum);
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sigpending(sigset_t *set);
int sigsuspend(const sigset_t *mask);
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sigpending(sigset_t *set);
int sigsuspend(const sigset_t *mask);

struct sigaction 
{	void (*sa_handler)(int);
	void (*sa_sigaction)(int, siginfo_t *, void *);
	sigset_t sa_mask;
	int sa_flags;
	void (*sa_restorer)(void);
};
#endif

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

inline 
int close(int fd)
{	return _close(fd);
}

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
int fchmod(int fildes, mode_t mode)
{	return 0;
}

#define PATH_MAX 255

#pragma warning( error : 4013)
#pragma warning( error : 4047) 

#endif
