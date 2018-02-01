// StdCopy.h
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef StdCopy_h
#define StdCopy_h

#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <bsd/string.h>

#ifdef _WIN32
#define ATTRIBUTE(a,b) 
#else
#define ATTRIBUTE(a,b) __attribute__((format(printf, a, b)))
#endif

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

template <typename T>
size_t strlcpy(T& dst, const char *src)
{	return strlcpy(&dst[0],src,dst.size());
}

template <typename T>
size_t strlcat(T& dst, const char *src)
{	return strlcat(&dst[0],src,dst.size());
}

inline
int slprintf(char* const buffer, size_t size,const char* const format,...) //ATTRIBUTE(3,4)
{	if(!buffer || !format || !size)
	{	return 0;
	}
	va_list argList;
	va_start(argList,format);
#ifdef _WIN32
#pragma warning(suppress:4996)
	const int count = vsnprintf(buffer,size-1,format,argList);
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=format"
	const int count = vsnprintf(buffer,size-1,format,argList);
#pragma GCC diagnostic push
#endif
	va_end(argList);
	buffer[size-1]=0;
	return count;
}

template<typename T>
int slprintf(T& buffer, const char* const format,...) //ATTRIBUTE(2,3)
{	const size_t size = buffer.size();
	if(!format || !size)
	{	return 0;
	}
	va_list argList;
	va_start(argList,format);
	char* p = &buffer[0];
#ifdef _WIN32
#pragma warning(suppress:4996)
	const int count = vsnprintf(p,size-1,format,argList);
#else
//__pragma("GCC diagnostic ignored \"-Wsuggest-attribute=format\"")
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-attribute=format"
	const int count = vsnprintf(p,size-1,format,argList);
#pragma GCC diagnostic push
#endif
	va_end(argList);
	buffer[size-1]=0;
	return count;
}

template <typename T>
size_t strlen(const T& buffer)
{	const char* p = &buffer[0];
	return strlen(p);
}
#if 0
inline
void* memcpy(char* dest, const char* src, size_t size)
{	return memcpy((void*)dest,(const void*) src,size);
}

template <typename T>
char* memcpy(T& dest,const void* src,size_t size)
{	memcpy(&dest[0], src, size);
	return &dest[0];
}

template <typename T>
char* memcpy(void* dest,T& src,size_t size)
{	memcpy(dest, &src[0], size);
	return dest;
}

template <typename T>
char* memcpy(T& dest,T& src,size_t size)
{	memcpy(&dest[0], &src[0], size);
	return &dest;
}
#endif
#pragma warning(default:4996)

#endif
