// Logger.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// Open source license MIT

#ifndef Logger_h
#define Logger_h

#ifdef NO_UNREAL

#include <stdio.h>

inline
void Log(const char* msg)
{	puts(msg);
}

inline
void LogError(const char* msg)
{	printf("ERROR: %s",msg);
}

#else

#include "UnrealLogger.h"

extern UnrealLogger logger;

#define Log(msg) logger.Log(__FILE__, __LINE__,msg)
#define LogError(msg) logger.Error(__FILE__, __LINE__,msg)

#endif
#endif
