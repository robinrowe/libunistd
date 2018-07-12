// publib.h
// Copyright 2018/05/20 Robin.Rowe@CinePaint.org
// License MIT open source

#ifndef publib_h
#define publib_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define QUIT(f) puts("Failed: " #f); exit(-1)

inline
void *xmalloc(size_t bytes)
{	void* p = malloc(bytes);
	if(!p)
	{	QUIT(xmalloc);
	}
	return p;
}

inline
void *xrealloc(void *ptr, size_t bytes)
{	void* p = realloc(ptr,bytes);
	if(!p)
	{	QUIT(xrealloc);
	}
	return p;
}

inline
void xfree(void *ptr)
{	free(ptr);
}

inline
char *xstrdup(const char *s)
{	if(!s)
	{	QUIT(xstrdup);
	}
	void* p = _strdup(s);
	if(!p)
	{	QUIT(xstrdup);
	}
	return (char*) p;
}

inline
void *memdup(const void *mem, size_t bytes)
{	void* p = malloc(bytes);
	if(!p)
	{	return 0;
	}
	memcpy(p,mem,bytes);
	return p;
}

inline
void *xmemdup(const void *mem, size_t bytes)
{	void* p = malloc(bytes);
	if(!p)
	{	QUIT(xmemdup);
	}
	memcpy(p,mem,bytes);
	return p;
}

inline
void* xcalloc (size_t nelem, size_t elsize)
{	void* p = calloc(nelem,elsize);
	if(!p)
	{	QUIT(xrealloc);
	}
	return p;
}

#define xsnprintf _snprintf

#undef QUIT

#endif