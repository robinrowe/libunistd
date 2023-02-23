// syslog.h
// Copyright 2019/10/8 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef syslog_h
#define syslog_h

struct Syslog_data;

//extern Syslog_data Syslog_data;

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
{	LOG_AUTH = 1,
	LOG_AUTHPRIV = 2,
	LOG_CRON = 4,
	LOG_DAEMON = 8,
	LOG_FTP = 16,
	LOG_KERN = 32,
	LOG_LOCAL0 = 64,
	LOG_LOCAL1 = 128,
	LOG_LOCAL2 = 256,
	LOG_LOCAL3 = 512,
	LOG_LOCAL4 = 1024,
	LOG_LOCAL5 = 2*1024,
	LOG_LOCAL6 = 4*1024,
	LOG_LOCAL7 = 8*1024,
	LOG_LPR = 16*1024,
	LOG_MAIL = 32*1024,
	LOG_NEWS = 64*1024,
	LOG_SYSLOG = 128*1024,
	LOG_USER = 256*1024,
	LOG_UUCP = 512*1024
};

// level:
enum
{	LOG_EMERG,
	LOG_ALERT,
	LOG_CRIT,
	LOG_ERR,
	LOG_WARNING,
	LOG_NOTICE,
	LOG_INFO,
	LOG_DEBUG
};

void openlog(const char *programname, int option, int facility);
void syslog(int priority, const char *format, ...);
void closelog();
int setlogmask(int mask);
void vsyslog(int priority, const char* format, va_list argp);

inline
int LOG_UPTO(const int maxMask)
{	const int mask = ((1<<((maxMask)+1))-1);
	return mask;
}

/*

#include <syslog.h>

setlogmask (LOG_UPTO (LOG_NOTICE));
openlog ("exampleprog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
syslog (LOG_NOTICE, "Program started by User %d", getuid ());
syslog (LOG_INFO, "A tree falls in a forest");
closelog ();

openlog("programname", 0, LOG_USER); --> /var/log/messages
openlog("programname", 0, LOG_LOCAL0); --> /var/log/programname

*/

#endif
