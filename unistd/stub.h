// stub.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef stub_h
#define stub_h

#include <stdio.h>
#include <stdlib.h>
#include "cfunc.h"

CFUNC inline
void StubBug(const char* msg)
{	printf("WARNING: Missing %s\n",msg);
}

#define STUB(functionName) StubBug("Bug! " #functionName)
#define STUB_0(functionName) StubBug("Bug! " #functionName); return 0
#define STUB_NEG(functionName) StubBug("Bug! " #functionName); return -1
#define STUB_RETVAL(functionName,retval) StubBug("Bug! " #functionName); return retval
#define STUB_BLANK(functionName) StubBug("Bug! " #functionName); return ""
#define STUB_MSG(functionName,msg) StubBug("Bug! " #functionName msg)

#endif

