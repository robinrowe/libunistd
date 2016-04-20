// Logger.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// Open source license MIT

#ifndef Logger_h
#define Logger_h

#ifdef UNREAL_ENGINE

#include "UnrealLogger.h"

#else

#include <stdio.h>

inline
void LogMsg(const char* msg)
{	puts(msg);
}

inline
void LogError(const char* msg)
{	printf("ERROR: %s",msg);
}

#endif

#ifdef LOGGER_QUIET
#undef LogMsg
#define LogMsg(msg)
#endif

#endif
