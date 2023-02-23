// fts.c
// Copyright (c) 2022/10/19 Noah Treuhaft <noah@brimdata.io>
// License open source MIT

#include "fts.h"
#include <stddef.h>

#include "../portable/stub.h"

FTS *fts_open(char * const *path_argv, int options, int (*compar)(const FTSENT **, const FTSENT **))
{	STUB_0(fts_open);
}

FTSENT *fts_read(FTS *ftsp)
{	STUB_0(fts_read);
}

int fts_close(FTS *ftsp)
{	STUB_NEG(fts_close);
}
