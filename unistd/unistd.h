/* unistd.h: replaces *nix header of same name
// Windows emulation of common *nix functions
// Copyright Nov 10, 2002, Robin.Rowe@CinePaint.org
// License MIT (http://opensource.org/licenses/mit-license.php);
*/

#ifndef unistd_h
#define unistd_h

#if  ((defined(_WINDOWS_) || defined(_INC_WINDOWS))) && !defined(WIN32_LEAN_AND_MEAN)
#error unistd.h must be included before Windows.h!
#endif

//#define _CRT_SECURE_NO_DEPRECATE 
//#undef _CRT_SECURE_NO_WARNINGS
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#endif
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
#include <memory.h>
#include <signal.h>
#include <sys/utime.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <inttypes.h>
#include <io.h>
#include "../portable/bsd_string.h"
#include "sigaction.h"
#include "gettimeofday.h"
#include "clock_gettime.h"
#include "cfunc.h"
//#include "int128/Int128.h"
#include "../portable/stub.h"

CFUNC const char* optarg;
CFUNC int optind;
CFUNC int opterr;
CFUNC int optopt;

typedef long long useconds_t;

enum 
{	F_LOCK=1,
	F_TLOCK,
	F_ULOCK,
	F_TEST 
};

//CFUNC pid_t getpgrp(...); /* POSIX.1 version */
CFUNC pid_t getpgrp(pid_t pid); /* BSD version */
CFUNC int setpgrp(pid_t pid, pid_t pgid); 
//CFUNC int uni_open(const char* filename,unsigned oflag,int mode);
CFUNC int uni_open(const char* filename, unsigned oflag,...);
CFUNC int fcntl(int handle, int mode,...);
//CFUNC int fcntl(int handle,int mode,int mode2);

CFUNC int mkdir2(const char* path, int mask);
CFUNC int snprintb(char *buf, size_t buflen, const char *fmt, uint64_t val);
CFUNC int snprintb_m(char *buf, size_t buflen, const char *fmt, uint64_t val,size_t max);
CFUNC size_t unistd_safe_strlen(const char* s);
CFUNC int uni_sscanf(char* input,const char* format,...);
CFUNC int strncasecmp(const char *s1, const char *s2, size_t n);
CFUNC FILE *popen(const char *command, const char *type);
CFUNC int pclose(FILE *stream);
CFUNC int kill(pid_t p, int x);
CFUNC int S_ISCHR(int v); 
CFUNC int S_ISBLK(int v); 
CFUNC int S_ISFIFO(int v); 
CFUNC int S_ISSOCK(int v); 
CFUNC pid_t gettid();
CFUNC int setgid(gid_t g);
CFUNC int setuid(uid_t g);
CFUNC const char* getsysconfdir();
CFUNC int mkstemp(char *filename);
CFUNC int fchmod(int a, mode_t b);
CFUNC uid_t getuid();
CFUNC uid_t geteuid();
CFUNC gid_t getgid();
CFUNC gid_t getegid();
CFUNC char* realpath(const char *path, char *resolved_path);
CFUNC ssize_t readlink(const char *path, char *buf, size_t bufsize);
CFUNC char *getlogin();
CFUNC int getlogin_r(char *buf, size_t len);
CFUNC int getopt(int argc, char * const argv[],const char *optstring);
CFUNC void PrintDirectory();
CFUNC unsigned int alarm(unsigned int seconds);
CFUNC int chown(const char *path, uid_t owner, gid_t group);
CFUNC int fchown(int fd, uid_t owner, gid_t group);
CFUNC int lchown(const char *path, uid_t owner, gid_t group);
CFUNC int chroot(const char *path);
CFUNC size_t confstr(int name, char *buf, size_t len);
CFUNC const char *ctermid(char *s);
CFUNC int fchdir(int fd);
CFUNC pid_t fork();
CFUNC int getdtablesize();
CFUNC int fsync (int fd);
CFUNC void sync();
CFUNC int syncfs(int fd);
CFUNC int fdatasync(int fd);
CFUNC long fpathconf(int fd, int name);
CFUNC long pathconf(const char *path, int name);
CFUNC long gethostid();
CFUNC int sethostid(long hostid);
CFUNC int getpagesize();
CFUNC char *getpass(const char *prompt);
CFUNC int setpgid(pid_t pid, pid_t pgid);
CFUNC pid_t getpgid(pid_t pid);
CFUNC pid_t getppid();
CFUNC int link(const char *oldpath, const char *newpath);
CFUNC int lockf(int fd, int cmd, off_t len);
CFUNC int nice(int inc);
CFUNC int pause();
CFUNC int brk(void *addr);
CFUNC void *sbrk(intptr_t increment);
CFUNC int setreuid(uid_t ruid, uid_t euid);
CFUNC int setregid(gid_t rgid, gid_t egid);
CFUNC int setsid();
CFUNC int symlink(const char *target, const char *linkpath);
CFUNC long sysconf(int name);
CFUNC pid_t tcgetpgrp(int fd);
CFUNC int tcsetpgrp(int fd, pid_t pgrp);
CFUNC char *ttyname(int fd);
CFUNC int ttyname_r(int fd, char *buf, size_t buflen);
CFUNC useconds_t ualarm(useconds_t usecs, useconds_t interval);
CFUNC pid_t vfork();
/* between 0.0 and 1.0 */
CFUNC double drand48();
CFUNC void srand48(long int seedval);
CFUNC long int random(void);
CFUNC int setenv(const char *name, const char *value, int overwrite);
CFUNC int unsetenv(const char *name);
CFUNC int truncate(const char *path, off_t length);
CFUNC int ftruncate(int fd, off_t length);
CFUNC int fseeko(FILE *stream, off_t offset, int whence);
CFUNC off_t ftello(FILE *stream);
CFUNC char* strptime(const char* s, const char* format,struct tm* tm);
CFUNC ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);


//#define strlen unistd_safe_strlen
//#define inet_ntop InetNtop
#define bzero(address,size) memset((address),0,size)
#define bcmp(s1, s2, n)	memcmp ((s1), (s2), (n))
#define bcopy(s, d, n)	memcpy ((d), (s), (n))
#define pow10(x) pow(x,10)
#define alloca _alloca
/* use with limits.h */
#define LONG_LONG_MAX LLONG_MAX     
#define LONG_LONG_MIN LLONG_MIN     
#define strdup _strdup
#define vsnprintf _vsnprintf
//#define sscanf uni_sscanf
#undef MAX_PRIORITY /* remove winspool.h warning */
#ifndef strcasecmp
#define strcasecmp _stricmp
#endif
#define strncasecmp _strnicmp
#define strtok_r strtok_s
//#define send send2
#define lstat stat
#define fileno _fileno
#define STDIN_FILENO _fileno(stdin)
// causes issues with math.h:
//#define rint(x) floor ((x) + 0.5)
//#define lround floor
//#define roundl floor
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
#define getpid _getpid
#define RETSIGTYPE void
#define access _access
#define pipe(pipes) _pipe((pipes),8*1024,_O_BINARY)
#define   __attribute__(x)
//__attribute__((format (printf, 1, 2)))
#define mkdir mkdir2

#endif

