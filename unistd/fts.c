// fts.c
// Provided by brimdata on 2022/10/19
// Copyright (c) 2019 Robin.Rowe@CinePaint.org
// License open source MIT

#include "fts.h"
#include <stddef.h>

FTS *fts_open(char * const *path_argv, int options, int (*compar)(const FTSENT **, const FTSENT **))
{	STUB_0(fts_open);
}

FTSENT *fts_read(FTS *ftsp)
{	return STUB_0(fts_read);
}

int fts_close(FTS *ftsp)
{	return STUB_NEG(fts_close);
}
