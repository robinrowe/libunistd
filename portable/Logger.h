// Logger.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// Open source license MIT

#ifndef Logger_h
#define Logger_h

#ifdef UNREAL_ENGINE

#include "UnrealLogger.h"

#else

#include <stdio.h>
#include <string>

inline
void LogMsg(const char* msg)
{	puts(msg);
}

inline
void LogError(const char* msg)
{	printf("ERROR: %s",msg);
}

inline
void LogMsg(const std::string& msg)
{	puts(msg.c_str());
}

inline
void LogError(const ::string& msg)
{	printf("ERROR: %s",msg.c_str());
}

#endif

#ifdef LOGGER_QUIET
#undef LogMsg
#define LogMsg(msg)
#endif

#endif
