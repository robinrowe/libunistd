// Logger.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// Open source license MIT

#ifndef Logger_h
#define Logger_h

#ifdef UNREAL_ENGINE
#include "UnrealLogger.h"
#else
#include <unistd.h>
#include <stdio.h>
#include <string.h>

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
	DebugBreak();
#endif
}
#endif

#ifdef LOGGER_QUIET
#define SYSLOG(msg) 
#define SYSERR(msg) 
#else
#define SYSLOG(msg) SysLogMsg(msg,__FUNCTION__)
#define SYSERR(msg) SysLogError(msg,__FUNCTION__)
#endif

#endif
