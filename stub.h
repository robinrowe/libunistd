// stub.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef stub_h
#define stub_h

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

inline
void Bug(const char* msg)
{	puts(msg);
}

#define TODO_TEST(x)	puts("TO-DO TEST: " #x)

#define STUB(functionName) { puts("STUB:" #functionName); }
#define STUB0(functionName) { puts("STUB:" #functionName); return 0; }

#ifdef __cplusplus
}
#endif

#endif

