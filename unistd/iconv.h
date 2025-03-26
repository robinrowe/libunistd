// iconv.h
// Copyright 2024 Robin.Rowe@cinepaint.org
// License MIT open source

#ifndef iconv_h
#define iconv_h

#include "cfunc.h"
#include "stub.h"

typedef int* iconv_t;

CFUNC inline
iconv_t iconv_open(const char* tocode, const char* fromcode)
{	STUB_0(iconv_open);
}

CFUNC inline
size_t iconv(iconv_t cd, const char** inbuf, size_t* inbytesleft,char** outbuf, size_t* outbytesleft)
{	STUB_0(globfree);
}

CFUNC int iconv_close(iconv_t cd)
{	STUB_NEG(globfree);
}

#endif
