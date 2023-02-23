#ifndef mem_h
#define mem_h

#include "cfunc.h"

CFUNC void* memmem(const void* haystack, size_t haystack_len, const void* needle, size_t needle_len)
{	if (haystack == 0)
	{	return 0;
	}
	if (haystack_len == 0)
	{	return 0;
	}
	if (needle == 0)
	{	return 0;
	}
	if (needle_len == 0)
	{	return 0;
	}
	for (const char* h = haystack;
			haystack_len >= needle_len;
			++h, --haystack_len)
	{	if (!memcmp(h, needle, needle_len))
		{	return (void*)h;
		}
	}
	return 0;
}

#endif
