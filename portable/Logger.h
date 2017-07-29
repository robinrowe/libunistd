// Logger.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// Open source license MIT

#ifndef Logger_h
#define Logger_h

#ifdef UNREAL_ENGINE
#include "UnrealLogger.h"
#else
#ifdef _WIN32
#include "../unistd/unistd.h"
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <string>

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
#endif
