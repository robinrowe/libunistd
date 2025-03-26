// getline.c
// Created by Robin Rowe 2024-10-05
// Open Source MIT

#include <string.h>
#include <memory.h>
#include "unistd.h"

#define BUFSIZE 256

ssize_t getline(char** buf, size_t* bufsiz, FILE* fp)
{	return getdelim(buf, bufsiz, '\n', fp);
}

ssize_t getdelim(char** buf, size_t* bufsiz, int delimiter, FILE* fp)
{	if (!*buf || !*bufsiz ) 
	{	*bufsiz = BUFSIZE;
		*buf = (char*) malloc(*bufsiz);
		if(!buf)
		{	return -1;
	}	}
	char* ptr = *buf;
	char* eptr = *buf + *bufsiz;
	for(;;) 
	{	const int c = fgetc(fp);
		if (c == -1) 
		{	if (feof(fp))
			{	return ptr == *buf ? -1 : ptr - *buf;
			}
			return -1;
		}
		*ptr++ = c;
		if (c == delimiter) 
		{	*ptr = '\0';
			return ptr - *buf;
		}
		if (ptr + 2 >= eptr) 
		{	char* nbuf;
			size_t nbufsiz = *bufsiz * 2;
			ssize_t d = ptr - *buf;
			nbuf = (char*) realloc(*buf, nbufsiz);
			if (!nbuf)
			{	return -1;
			}
			*buf = nbuf;
			*bufsiz = nbufsiz;
			eptr = nbuf + nbufsiz;
			ptr = nbuf + d;
		}
	}
	return -1;
}

