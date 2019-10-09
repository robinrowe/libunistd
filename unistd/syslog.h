// syslog.h
// Copyright 2019/10/8 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef syslog_h
#define syslog_h

#include <stdio.h>
#include <stdlib.h>
#ifdef _DEBUG
#include <crtdbg.h>
#endif

extern FILE* syslog_fp;
//extern int syslog_option;
//extern int syslog_facility;

inline
void openlog(const char *filename, int option, int facility)
{	const char* err = "Error: can't openlog()";
	if(!filename || !syslog_fp)
	{	puts(err);
		return;
	}
	syslog_fp = fopen(filename,"w+");
    if(!syslog_fp)
	{	puts(err);
		return;
	}
	// syslog_option = option;
	// syslog_facility = facility;
	(void) option;
	(void) facility;
}

inline
void syslog(int priority, const char *format, ...)
{	(void) priority;
	va_list argp;
	va_start(argp,format);
	vfprintf(syslog_fp,format,argp);
#ifdef _DEBUG
	char msg[80];
	vsprintf(msg,format,argp);
	OutputDebugStringA(msg);
#endif
	va_end(argp);
}

inline
void closelog() 
{	if(!syslog_fp)
	{	return;
	}
	fclose(syslog_fp);
	syslog_fp = 0;
}

inline
int setlogmask(int mask)
{	(void) mask;
	return 0;
}

#define LOG_UPTO(x) x

// option:
enum 
{	LOG_CONS = 1,
	LOG_NDELAY = 2,
	LOG_NOWAIT = 4,
	LOG_ODELAY = 8,
	LOG_PERROR = 16,
	LOG_PID = 32
};

// facility:
enum
{	LOG_AUTH,
	LOG_AUTHPRIV,
	LOG_CRON,
	LOG_DAEMON,
	LOG_FTP,
	LOG_KERN,
	LOG_LOCAL0,
	LOG_LOCAL1,
	LOG_LOCAL2,
	LOG_LOCAL3,
	LOG_LOCAL4,
	LOG_LOCAL5,
	LOG_LOCAL6,
	LOG_LOCAL7,
	LOG_LPR,
	LOG_MAIL,
	LOG_NEWS,
	LOG_SYSLOG,
	LOG_USER,
	LOG_UUCP
};

// level:
enum
{	LOG_DEBUG,
	LOG_INFO,
	LOG_NOTICE,
	LOG_WARNING,
	LOG_ERR,
	LOG_CRIT,
	LOG_ALERT,
	LOG_EMERG
};

/*

#include <syslog.h>

setlogmask (LOG_UPTO (LOG_NOTICE));
openlog ("exampleprog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
syslog (LOG_NOTICE, "Program started by User %d", getuid ());
syslog (LOG_INFO, "A tree falls in a forest");
closelog ();

*/

#pragma warning(default:4996)

#endif
