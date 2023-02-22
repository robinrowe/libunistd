// fts.h
// Provided by brimdata on 2022/10/19
// Copyright (c) 2019 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef fts_h
#define fts_h

#include "cfunc.h"

#define FTS_F 0
#define FTS_NOCHDIR 0

typedef struct {
	int unused;
} FTS;

typedef struct {
	unsigned short fts_info;
	char * fts_path;
} FTSENT;

CFUNC FTS *fts_open(char * const *path_argv, int options, int (*compar)(const FTSENT **, const FTSENT **));
CFUNC FTSENT *fts_read(FTS *ftsp);
CFUNC int fts_close(FTS *ftsp);

#endif