// sys/random.cpp
// Copyright (c) 2022/10/19 Tomer Lev <tomerlev@microsoft.com>
// License open source MIT

#define _CRT_RAND_S
#include <unistd.h>
#include <stdlib.h>

#include "random.h"

int getrandom(void *buf, size_t buflen, unsigned int flags)
{	size_t count = buflen / sizeof(unsigned int);
	unsigned int* result = reinterpret_cast<unsigned int*>(buf);
	for (size_t i = 0; i < count; ++i)
	{	if (0 != rand_s(&result[i]))
		{	return static_cast<int>(i * sizeof(unsigned int));
		}
	}
	size_t remainder = buflen % sizeof(unsigned int);
	if (remainder > 0)
	{	unsigned int val = 0;
		unsigned char* remainderBuf = reinterpret_cast<unsigned char*>(buf) + buflen - remainder;
		if (0 != rand_s(&val))
		{	return static_cast<int>(count * sizeof(unsigned int));
		}
		memcpy_s(remainderBuf, remainder, &val, remainder);
	}
	return static_cast<int>(buflen);
}
