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

#if defined(_WIN32) && defined(_DEBUG)
#include <crtdbg.h>
#endif

#ifdef QT
#include <QStringView>
#endif

namespace portable {

#ifdef OLD_SYSLOG

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
{	printf("ERROR: %s\n",msg.c_str());
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
	buffer[0] = 0;
	const int wsaError = WSAGetLastError();
	const DWORD num = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		wsaError,
		0,//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&buffer,
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

inline
unsigned IsArg(const char* mode,int argc,char* argv[],bool isTerse = true)
{	if(!mode)
	{	return 0;
	}
	for(unsigned i=1; i < (unsigned) argc; i++)
	{	const char* arg = argv[i];
		if((mode[0] != arg[0]) || (mode[1] != arg[1]))
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

#define IsArgMode(mode) portable::IsArg(mode,argc,argv)

inline
void tty_msg(const char* tag,const char* msg,const char* functionName=0,int lineNo=0)
{	static int tty_state = -1;
	if(-1 == tty_state)
	{	tty_state = isatty(fileno(stdout));
	}
	if(!tty_state)
	{	return;
	}
	if(functionName)
	{	printf("%s: %s (%s:%i)\n",tag,msg,functionName,lineNo);
		return;
	}	
	printf("%s: %s\n",tag,msg);
}

inline 
const char* Safe(const char* msg)
{	if(!msg)
	{	return "unknown";
	}
	return msg;
}

inline 
void syslog_function(int level,const char* tag,const char* msg,const char* function,int lineno)
{	if(!function)
	{	syslog(level,"%s: %s",Safe(tag),Safe(msg));
		return;
	}
	syslog(level,"%s: %s (%s:%i)",Safe(tag),Safe(msg),function,lineno);
}

inline
void status_msg(const char* msg,const char* function=0,int lineno=0) 
{	syslog_function(LOG_NOTICE,"Status",msg,function,lineno);
	tty_msg("Status",msg,function,lineno);
}

inline
void warning_msg(const char* msg,const char* function=0,int lineno=0) 
{	syslog(LOG_WARNING,"Warning: %s",msg);
	tty_msg("Warning",msg,function,lineno);
}

inline
void trace_msg(const char* msg,const char* function=0,int lineno=0) 
{	tty_msg("Trace",msg,function,lineno);
}

inline
void error_msg(const char* msg,const char* function,int lineno) 
{	syslog(LOG_ERR,"Error: %s, %s:%i",msg,function,lineno);
	tty_msg("Error",msg,function,lineno);
}

}

#ifdef TRACE_MSG_FUNCTIONS
#define status_msg(msg) portable::status_msg(msg,__FUNCTION__,__LINE__)
#define warning_msg(msg) portable::warning_msg(msg,__FUNCTION__,__LINE__)
#define trace_msg(msg) portable::trace_msg(msg,__FUNCTION__,__LINE__)
#else
#define status_msg(msg) portable::status_msg(msg)
#define warning_msg(msg) portable::warning_msg(msg)
#define trace_msg(msg) portable::trace_msg(msg)
#endif
#define error_msg(msg) portable::error_msg(msg,__FUNCTION__,__LINE__)
#define return_msg(enum_name) status_msg(#enum_name);return int(enum_name)
#define show_msg(enum_name) status_msg(#enum_name)

inline
void signal_safe_puts(const char* msg)
{	if(msg)
	{	write(fileno(stdout),msg,(int) strlen(msg));
}	}

#endif
