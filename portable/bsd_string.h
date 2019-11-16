/* bsd/string.h
   Copyright 2018/02/01 Robin.Rowe@CinePaint.org
   License MIT (http://opensource.org/licenses/mit-license.php)
*/

#ifndef bsd_string_h
#define bsd_string_h

#include <string.h>

#pragma warning(disable:4996)
inline
size_t strlcpy(char *dst, const char *src, size_t size)
{	if(!dst || !src)
	{	return 0;
	}
	const char* end = strncpy(dst,src,size);
	const size_t length = end - dst;
	if(length>=size)
	{	dst[size-1] = 0;
		return size-1;
	}
	return length;
}

inline
size_t strlcat(char *dst, const char *src, size_t size)
{	if(!dst || !src)
	{	return 0;
	}
	const size_t length = strlen(dst);	
	char* p = dst + length;
	return length + strlcpy(p,src,size - length);
}
#pragma warning(default:4996)

#endif