// sys/sys_types.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef sys_sys_types_h
#define sys_sys_types_h

#include <stdint.h>

typedef intptr_t pid_t;
typedef int gid_t;
typedef int uid_t;
typedef int sigval_t;
typedef int sigset_t;
typedef unsigned short ushort;
typedef	int key_t;
typedef int ssize_t;
typedef unsigned short mode_t;
typedef int gid_t;
typedef int uid_t;
typedef int Atom;

enum
{	F_DUPFD, 
	F_GETFD,
	F_SETFD, 
	F_GETFL, 
	F_SETFL, 
	F_GETLK, 
	F_SETLK,
	F_SETLKW,
	FD_CLOEXEC
};

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

#define _S_IFMT   0xF000 // File type mask
#define _S_IFDIR  0x4000 // Directory
#define _S_IFCHR  0x2000 // Character special
#define _S_IFIFO  0x1000 // Pipe
#define _S_IFREG  0x8000 // Regular
#define _S_IREAD  0x0100 // Read permission, owner
#define _S_IWRITE 0x0080 // Write permission, owner
#define _S_IEXEC  0x0040 // Execute/search permission, owner

#define S_IFMT   _S_IFMT
#define S_IFDIR  _S_IFDIR
#define S_IFCHR  _S_IFCHR
#define S_IFREG  _S_IFREG
#define S_IREAD  _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC  _S_IEXEC

*/
enum {
	S_IFSOCK = 1,
	S_IFLNK,
	S_IFBLK,
	S_IFIFO,
	S_ISUID,
	S_ISGID,
	S_ISVTX
};

#define PATH_MAX 255

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

#endif
