// strings.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef strings_h
#define strings_h

#include "../portable/stub.h"
#include "cfunc.h"

inline
int ffs(int i)
{   STUB_NEG(ffs);
}

inline
int ffsl(long int i)
{   STUB_NEG(ffsl);
}

inline
int ffsll(long long int i)
{   STUB_NEG(ffsll);
}

#endif
