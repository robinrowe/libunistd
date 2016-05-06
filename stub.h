// stub.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef stub_h
#define stub_h

#include <stdio.h>

#define STUB(functionName) { puts("STUB:" #functionName); }
#define STUB0(functionName) { puts("STUB:" #functionName); return 0; }

#endif

