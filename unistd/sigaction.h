// sigaction.h
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT 

#ifndef uni_signal_h
#define uni_signal_h

#include <signal.h>
#include <time.h>
#include <memory.h>
#include "../portable/stub.h"
#include "sys/sys_types.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#else
//#define inline __inline
#endif

#define SIGRTMIN 32
#define SIGRTMAX 64

#define SIGALRM NSIG+1
#define SIGHUP NSIG+2
#define SIGQUIT NSIG+3
#define SIGBUS NSIG+4
#define SIGPIPE NSIG+5
#define SIGKILL NSIG+9
#define SIGCHLD NSIG+6
#define SIGUSR1 NSIG+7
#define SIGUSR2 NSIG+8
/* SIGTERM */
/* SIGFPE */

/*
SIGABRT Abnormal termination 
SIGFPE Floating-point error 
SIGILL Illegal instruction 
SIGINT CTRL+C signal 
SIGSEGV Illegal storage access 
SIGTERM Termination request 
*/

#define WNOHANG 0
#define SIG_UNBLOCK 1
#define SIG_BLOCK 2
#define WIFSIGNALED() false
#define WTERMSIG() 0

typedef struct siginfo_t siginfo_t;

struct siginfo_t {
    int      si_signo;     /* Signal number */
    int      si_errno;     /* An errno value */
    int      si_code;      /* Signal code */
    int      si_trapno;    /* Trap number that caused
                                hardware-generated signal
                                (unused on most architectures) */
    pid_t    si_pid;       /* Sending process ID */
    uid_t    si_uid;       /* Real user ID of sending process */
    int      si_status;    /* Exit value or signal */
    clock_t  si_utime;     /* User time consumed */
    clock_t  si_stime;     /* System time consumed */
    sigval_t si_value;     /* Signal value */
    int      si_int;       /* POSIX.1b signal */
    void    *si_ptr;       /* POSIX.1b signal */
    int      si_overrun;   /* Timer overrun count;
                                POSIX.1b timers */
    int      si_timerid;   /* Timer ID; POSIX.1b timers */
    void    *si_addr;      /* Memory location which caused fault */
    long     si_band;      /* Band event (was int in
                                glibc 2.3.2 and earlier) */
    int      si_fd;        /* File descriptor */
    short    si_addr_lsb;  /* Least significant bit of address
                                (since Linux 2.6.32) */
    void    *si_call_addr; /* Address of system call instruction
                                (since Linux 3.5) */
    int      si_syscall;   /* Number of attempted system call
                                (since Linux 3.5) */
    unsigned int si_arch;  /* Architecture of attempted system call
                                (since Linux 3.5) */
};

struct sigaction
{	void (*sa_handler)(int);
	void (*sa_sigaction)(int, struct siginfo_t *, void *);
	sigset_t sa_mask;
	int sa_flags;
	void (*sa_restorer)(void);
} ;

enum
{	SA_NOCLDSTOP,
	SA_NOCLDWAIT, 
	SA_NODEFER, 
	SA_ONSTACK, 
	SA_RESETHAND,
	SA_RESTART, 
	SA_SIGINFO
};


#define SIG_SETMASK 0
#define SIGTSTP 0

inline
int sigemptyset(sigset_t *set)
{	if(!set)
	{	return -1;
	}
	memset(set,sizeof(*set),0);
	return 0;	
}

inline
int sigfillset(sigset_t *set)
{	(void)set;
	STUB_0(sigfillset);
}

inline
int sigaddset(sigset_t *set, int signum)
{	(void)set;
	(void)signum;
	STUB_0(sigaddset);
}

inline
int sigdelset(sigset_t *set, int signum)
{	(void)set;
	(void)signum;
	STUB_0(sigdelset);
}

inline
int sigismember(const sigset_t *set, int signum)
{	(void)set;
	(void)signum;
	STUB_0(sigismember);
}

extern void (*CtrlCHandler)(int, struct siginfo_t *, void *);
BOOL WindowsCtrlCHandler(DWORD fdwCtrlType);
int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact);

inline
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{	(void)how;
	(void)set;
	(void)oldset;
	STUB_0(sigprocmask);
}

inline
int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset)
{	return sigprocmask(how, set, oldset);
}

inline
int sigpending(sigset_t *set)
{	(void)set;
	STUB_0(sigpending);
}

inline
int sigsuspend(const sigset_t *mask)
{	(void)mask;
	STUB_0(sigsuspend);
}

#ifdef __cplusplus
}
#endif

#endif
