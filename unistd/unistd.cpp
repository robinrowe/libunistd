/* unistd.h: replaces *nix header of same name
// Windows emulation of common *nix functions
// Copyright Nov 10, 2002, Robin.Rowe@CinePaint.org
// License MIT (http://opensource.org/licenses/mit-license.php)
*/

#include "unistd.h"
#include "sys/sys_types.h"

pid_t getpgrp() /* POSIX.1 version */
{	STUB_0(getpgrp);
}

pid_t getpgrp(pid_t pid) /* BSD version */
{	(void)pid;
	STUB_0(getpgrp);
}

int setpgrp() /* System V version */
{	STUB_0(setpgrp);
}

int setpgrp(pid_t pid, pid_t pgid) /* BSD version */ 
{	(void)pid;
	(void)pgid;
	STUB_0(setpgrp);
}

#pragma warning(disable : 4996)

int read(int fh,void* buf,unsigned count)
{	return _read(fh,buf,count);
}

int pipe(int pipes[2])
{	return _pipe((pipes), 8*1024, _O_BINARY);
}

int snprintb(char *buf, size_t buflen, const char *fmt, uint64_t val)
{	(void)buf;
	(void)buflen;
	(void)fmt;
	(void)val;
	STUB_0(snprintb);
}

int snprintb_m(char *buf, size_t buflen, const char *fmt, uint64_t val,size_t max)
{	(void)buf;
	(void)buflen;
	(void)fmt;
	(void)val;
	(void)max;
	STUB_0(snprintb_m);
}

int uni_open(const char* filename,unsigned oflag,int mode)
{	return _open(filename,oflag,mode);
}

int mkdir2(const char* path, int mask)
{	(void) mask;
	return _mkdir(path);
}

int uni_open(const char* filename, unsigned oflag,...)
{	return _open(filename, oflag, 0);
}

int fcntl(int handle, int mode,...)
{	(void)handle;
	(void)mode;
	STUB_0(fcntl);
}

#if 0
int fcntl(int handle,int mode,int mode2)
{	(void)handle;
	(void)mode;
	(void)mode2;
	STUB_0(fcntl);
}
#endif

size_t unistd_safe_strlen(const char* s)
{	if(!s)
	{	puts("ERROR: strlen(null)");
		return 0;
	}
	return (s ? strlen(s):0);
}

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

#undef MAX_PRIORITY /* remove winspool.h warning */
#if 0
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
#endif

FILE *popen(const char *command, const char *type)
{	
#ifdef _DEBUG
	printf("popen(%s,%s)\n",command,type);
#endif
	return _popen(command,type);
}

int pclose(FILE *stream)
{	return stream ? _pclose(stream):-1;
}

int kill(pid_t p, int x)
{	(void)p;
	(void)x;
	return -1;
}

int S_ISCHR(int v) 
{	(void)v;
	return 0; 
}

int S_ISBLK(int v) 
{	(void)v;
	return 0;
}

int S_ISFIFO(int v) 
{	(void)v;
	return 0;
}

int S_ISSOCK(int v) 
{	(void)v;
	return 0;
}

pid_t gettid()
{	HANDLE h = GetCurrentThread();
	return (intptr_t) h;
}

int setgid(gid_t g)
{	(void)g;
	return -1;
}

int setuid(uid_t g)
{	(void)g;
	return -1;
}

const char* getsysconfdir()
{	STUB_0(getsysconfdir);
}

int mkstemp(char *filename)
{	char buffer[MAX_PATH];
	unsigned filenameNo = GetTempFileNameA(".",filename,0,buffer);
	return (int) filenameNo;
}

int fchmod(int a, mode_t b)
{	(void)a;
	(void)b;
	STUB_NEG(fchmod);
}

uid_t getuid()
{	STUB_NEG(getuid);
}

uid_t geteuid()
{	STUB_NEG(geteuid);
}

gid_t getgid()
{	STUB_NEG(getgid);
}

gid_t getegid()
{	STUB_NEG(getegid);
}

char* realpath(const char *path, char *resolved_path)
{	if(!resolved_path)
	{	return 0;
	}
	const DWORD  err = GetFullPathNameA(path,(DWORD) PATH_MAX,resolved_path,0);
	if(err == 0)
	{	return 0;
	}
	return resolved_path;
}

ssize_t readlink(const char *path, char *buf, size_t bufsize)
{	(void)path;
	(void)buf;
	(void)bufsize;
	STUB_0(readlink);
}

#if _MSC_VER <= 1900

int gethostname(char *name, size_t len)
{   DWORD bufsize = (DWORD) len;
    BOOL ok = GetComputerNameA(name,&bufsize);
    return ok? 0:-1;
}

#endif

char *getlogin()
{	STUB_0(getlogin);
}

int getlogin_r(char *buf, size_t len)
{   DWORD bufsize = (DWORD) len;
    BOOL ok = GetUserNameA(buf,&bufsize);
    return ok? 0:-1;
}

void PrintDirectory()
{	const char* path = _getcwd(0,0);
	if(!path)
	{	perror("getcwd() error");
		return;
	}
	printf("pwd = %s\n", path);
	free((void*)path);
}

unsigned int alarm(unsigned int seconds)
{	(void)seconds;
	STUB_0(alarm);
}

int chown(const char *path, uid_t owner, gid_t group)
{	(void)path;
	(void)owner;
	(void)group;
	STUB_0(chown);
}

int fchown(int fd, uid_t owner, gid_t group)
{	(void)fd;
	(void)owner;
	(void)group;
	STUB_0(fchown);
}

int lchown(const char *path, uid_t owner, gid_t group)
{	(void)path;
	(void)owner;
	(void)group;
	STUB_0(lchown);
}

int chroot(const char *path)
{	(void)path;
	STUB_NEG(chroot);
}

size_t confstr(int name, char *buf, size_t len)
{	(void)name;
	(void)buf;
	(void)len;
	STUB_0(confstr);
}

const char *ctermid(char *s)
{	const char* term = "/dev/tty";
	if(s)
	{	strcpy(s,term);
		return s;
	}
	return term;
}

/*
int chdir(const char *path)
{	return _chdir(path);
}*/

int fchdir(int fd)
{	(void)fd;
	STUB_NEG(fchdir);
}

pid_t fork()
{	STUB_NEG(fork);
}

inline
int getdtablesize()
{	STUB_0(getdtablesize);
}

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

void sync()
{	_flushall();
}

int syncfs(int fd)
{	fsync(fd);
	return 0;
}

int fdatasync(int fd)
{	(void)fd;
	STUB_NEG(fdatasync);
}

long fpathconf(int fd, int name)
{	(void)fd;
	(void)name;
	STUB_NEG(fpathconf);
}

long pathconf(const char *path, int name)
{	(void)path;
	(void)name;
	STUB_NEG(pathconf);
}

long gethostid()
{	STUB_NEG(gethostid);
}

int sethostid(long hostid)
{	(void)hostid;
	STUB_NEG(sethostid);
}

int getpagesize()
{	STUB_0(getpagesize);
}

char *getpass(const char *prompt)
{	(void)prompt;
	STUB_0(getpass);
}

int setpgid(pid_t pid, pid_t pgid)
{	(void)pid;
	(void)pgid;
	STUB_0(setpgid);
}

pid_t getpgid(pid_t pid)
{	(void)pid;
	STUB_0(getpgid);
}

/*

In process.h:

inline
pid_t getpid()
{	return _getpid();
}
*/

pid_t getppid()
{	STUB_0(getppid);
}

int link(const char *oldpath, const char *newpath)
{	(void)oldpath;
	(void)newpath;
	STUB_NEG(link);
}

int lockf(int fd, int cmd, off_t len)
{	(void)fd;
	(void)cmd;
	(void)len;
	STUB_NEG(lockf);
}

int nice(int inc)
{	(void)inc;
	STUB_NEG(nice);
}

int pause()
{	STUB_NEG(pause);
}

int brk(void *addr)
{	(void)addr;
	STUB_NEG(brk);
}

void *sbrk(intptr_t increment)
{	(void)increment;
	STUB_0(sbrk);
}

int setreuid(uid_t ruid, uid_t euid)
{	(void)ruid;
	(void)euid;
	STUB_NEG(setreuid);
}

int setregid(gid_t rgid, gid_t egid)
{	(void)rgid;
	(void)egid;
	STUB_NEG(setregid);
}

int setsid()
{	STUB_NEG(setsid);
}

int symlink(const char *target, const char *linkpath)
{	(void)target;
	(void)linkpath;
	STUB_NEG(symlink);
}

long sysconf(int name)
{	(void)name;
	STUB_NEG(sysconf);
}

pid_t tcgetpgrp(int fd)
{	(void)fd;
	STUB_NEG(tcgetpgrp);
}

int tcsetpgrp(int fd, pid_t pgrp)
{	(void)fd;
	(void)pgrp;
	STUB_NEG(tcsetpgrp);
}

char *ttyname(int fd)
{	(void)fd;
	STUB_0(ttyname);
}

int ttyname_r(int fd, char *buf, size_t buflen)
{	(void)fd;
	(void)buf;
	(void)buflen;
	STUB_NEG(ttyname_r);
}

useconds_t ualarm(useconds_t usecs, useconds_t interval)
{	(void)usecs;
	(void)interval;
	STUB_0(ualarm);
}

pid_t vfork()
{	STUB_NEG(vfork);
}

/* between 0.0 and 1.0 */
double drand48(void)
{	double r=(double)rand();
	r/=RAND_MAX;
	return r > 1.0? 1.0:r;
}

/*double srand48(time_t);*/

void srand48(long int seedval)
{	srand(seedval);
}

long int random()
{	return rand();
}

void srandom(unsigned int seed)
{ 	srand(seed);
}

#if 0
int sleep(useconds_t seconds)
{	Sleep((DWORD)(1000*seconds));
	return 0;
}

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

int ftruncate(int fd, off_t length)
{	return _chsize(fd,length);
}

int fseeko(FILE *stream, off_t offset, int whence)
{	return fseek(stream,offset,whence);
}

off_t ftello(FILE *stream)
{	return ftell(stream);
}

ssize_t pwrite(int fildes, const void *buf, size_t nbyte, off_t offset)
{	if (nbyte == 0)
	{	return 0;
	}
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.Offset = static_cast<DWORD>(offset);
	overlapped.OffsetHigh = offset >> 32;
	DWORD written;
	if (!WriteFile((HANDLE)_get_osfhandle(fildes), buf, static_cast<DWORD>(nbyte), &written, &overlapped))
	{	return -1;
	}
	return written;
}

int setlinebuf(FILE *stream)
{	return setvbuf(stream, NULL, _IONBF, 0);
}

int vasprintf(char **ptr, const char *format, va_list arg)
{	int n = _vscprintf(format, arg);
	if (n < 0)
		return -1;
	char *p = (char *)malloc(n+1);
	if (p == NULL)
		return -1;
	int rv = vsprintf_s(p, n+1, format, arg);
	if (rv < 0)
	{	free(p);
		return -1;
	}
	*ptr = p;
	return rv;
}
