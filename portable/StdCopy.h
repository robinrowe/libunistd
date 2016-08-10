// StdCopy.h
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef StdCopy_h
#define StdCopy_h

#include <stdarg.h>
#include <assert.h>

namespace portable
{

template <typename T>
T* copy(T* first, T* last, T* d_first)
{	const unsigned n = last-first-1;
	if(d_first > last || d_first+n-1 < first)
	{	memcpy(d_first,first,n*sizeof(T));
	}
	else
	{	memmove(d_first,first,n*sizeof(T));
	}
	return d_first;
}

#if 0
template <typename T,typename A>
T* copy2(T* first, T* last,const A& array,unsigned offset)
{	const unsigned n = last-first-1;
	assert(n<=array.size()-offset);
	if(n<=array.size()-offset)
	{	return copy(first,first+array.size()-offset,&array[offset]);
	}
	return copy(first,last,&array[offset]);
}
#endif

}

#pragma warning(disable:4996)
inline
size_t strlcpy(char *dst, const char *src, size_t size)
{	if(!dst || !src)
	{	return 0;
	}
	const char* end = strncpy(dst,src,size);
	if(end == dst + size - 1)
	{	dst[size-1] = 0;
		return size -1;
	}
	return end - dst;
}
#pragma warning(default:4996)

inline
size_t strlcat(char *dst, const char *src, size_t size)
{	if(!dst || !src)
	{	return 0;
	}
	const size_t length = strlen(dst);	
	char* p = dst + length;
	return length + strlcpy(p,src,size - length);
}

#endif
