// portable/SystemLog.h
// Copyright 2018/4/3 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef SystemLog_h
#define SystemLog_h

#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdio.h>

#ifdef _WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#endif

#ifdef QT
#include <QStringView>
#endif

namespace portable {

#ifdef SYSTEM_LOGFILE
extern FILE* systemLogfile;
#endif

inline
void SysLogMsg(const char* msg,const char* function)
{	printf("TRACE: %s, %s\n",msg,function);
}

#pragma warning(disable:4996)
inline
void SysLogError(const char* msg,const char* function)
{	printf("ERROR: %s, %s (%s)\n",msg,function,strerror(errno));
#pragma warning(default:4996)
#ifdef _DEBUG
#ifdef _MSC_VER
	DebugBreak();
#else
    raise(SIGTRAP);
#endif
#endif
}

inline
void LogMsg(const char* msg)
{	puts(msg);
}

inline
void LogMsg(const std::string& msg)
{	puts(msg.c_str());
}

inline
void LogError(const std::string& msg)
{	printf("ERROR: %s",msg.c_str());
}

#endif

#ifdef LOGGER_QUIET
#define SYSLOG(msg) 
#define SYSERR(msg) 
#else
#define SYSLOG(msg) SysLogMsg(msg,__FUNCTION__)
#define SYSERR(msg) SysLogError(msg,__FUNCTION__)
#endif

/*
 __attribute__((__format__(__printf__, 2,3)))
void debugLogf(const char * functionName, const char * format, ...)
{   va_list ap;
    va_start(ap, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer) / sizeof(*buffer), format, ap);
    va_end(ap);
}
*/

inline
unsigned IsArgModeX(const char* mode,int argc,char* argv[],bool isTerse = true)
{	if(!mode)
	{	return 0;
	}
	for(unsigned i=1; i < (unsigned) argc; i++)
	{	if(('-' != argv[i][0]) || (*mode != argv[i][1]))
		{	continue;
		}
		if(isTerse)
		{	return i;
		}
		if(!strcmp(mode,argv[i]+1))
		{	return i;
	}	}
	return 0;
}

inline
void SystemLog(const char* filename,int lineNo,const char* msg)
{/*OutputDebugStringA("My output string.");
	_RPTF2(_CRT_WARN, "In NameOfThisFunc( )," " someVar= %d, otherVar= %d\n", someVar, otherVar );
	_CRT_WARN, _CRT_ERROR, and _CRT_ASSERT.*/
	if(!filename)
	{	filename = "unknown";
	}
#ifdef _WIN32
	enum {len = 60};
	char buffer[len];
	const DWORD num = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		WSAGetLastError(),
		0,
		buffer,
		len-1,
		NULL);
	if(!msg && *buffer)
	{	msg = buffer;
		buffer[len-1] = 0;
	}
#endif
#pragma warning(disable:4996)
	if(!msg || !*msg)
	{	msg = strerror(errno);
	}
	printf("%s@%i: %s\n",filename,lineNo,msg);
#ifdef _DEBUG
#ifdef QT
	qDebug() << filename << "@" << lineNo << ": " << msg;
#else
	_CrtDbgReport( _CRT_WARN,filename,lineNo,msg,NULL);
#endif
#ifdef DEBUG_BREAK
	DebugBreak();
#endif
#endif
#ifdef SYSTEM_LOGFILE
	if(systemLogfile)
	{	fprintf(systemLogfile,"%s@%s: %s\n",filename,lineNo,msg);
	}
#endif
}

inline
void StatusMsg(const char* msg)
{
#ifdef _DEBUG
#ifdef QT
	qDebug() << msg;
#else
	OutputDebugStringA(msg);
#endif
#endif
	printf("Status: %s\n",msg);
}

#ifdef _DEBUG
#define TRACE(msg) portable::SystemLog(__FILE__,__LINE__,msg)
#else
#define TRACE(msg) 
#endif
#pragma warning(default:4996)

}

inline
void tty_msg(const char* tag,const char* msg)
{	static int tty_state = -1;
	if(-1 == tty_state)
	{	tty_state = isatty(fileno(stdout));
	}
	if(!tty_state)
	{	return;
	}
	printf("%s: %s\n",tag,msg);
}

inline
void status_msg(const char* msg)
{	syslog(LOG_NOTICE,"Status: %s",msg);
	tty_msg("Status",msg);
}

inline
void warning_msg(const char* msg) 
{	syslog(LOG_WARNING,"Warning: %s",msg);
	tty_msg("Warning",msg);
}

inline
void trace_msg(const char* msg) 
{	tty_msg("Trace",msg);
}

inline
void error_msg(const char* msg) 
{	syslog(LOG_ERR,"Error: %s",msg);
	tty_msg("Error",msg);
}

#define return_msg(x) status_msg(#x);return int(x)
#define IsArgMode(mode) portable::IsArgModeX(mode,argc,argv)

#endif
