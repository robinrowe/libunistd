// ftw.h 
// Copyright 2022 CinePaint MIT Open Source
// 13 July 2022

#ifndef ftw_h
#define ftw_h

#include <time.h>
#include "../portable/stub.h"

inline
int nftw(const char *dirpath,
	int (*fn)(const char *fpath, 
	const struct stat *sb,
	int typeflag, struct FTW *ftwbuf),
	int nopenfd, int flags)
{	STUB_NEG(nftw);
}

inline
int ftw(const char *dirpath,
	int (*fn)(const char *fpath, 
	const struct stat *sb,
	int typeflag),
	int nopenfd)
{	STUB_NEG(ftw);
}

#endif
