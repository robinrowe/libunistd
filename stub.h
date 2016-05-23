// stub.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef stub_h
#define stub_h

#include <stdio.h>
#include <process.h>

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

inline
void StubBug(const char* msg)
{	puts(msg);
}

#ifdef _DEBUG
#define BUG(x)	StubBug("BUG: " x)
#define TODO(x)	StubBug("TO-DO: " x)
#else
#define BUG(x)	"error" 
#define TODO(x)	"error" 
#endif

#define STUB(functionName) { StubBug("STUB:" #functionName); }
#define STUB0(functionName) { StubBug("STUB:" #functionName); return 0; }
#define STUB_MSG(functionName) StubBug("STUB:" #functionName)

inline
void StubExit(int errorlevel,const char* file,const char* function,int line)
{	printf("ERROR: %s exit(%i)\n%s:%i",function,errorlevel,file,line);
	exit(errorlevel);
}

#define exit(x) StubExit(x,__FILE__, __FUNCTION__, __LINE__)

#ifdef __cplusplus
}
#endif

#endif

