// mman_create.cpp
// Note: Link with -lrt

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <string.h>
#include <assert.h>

#ifndef _WIN32
#define shm_close close
#define shm_ftruncate ftruncate
#endif

int main(int argc, char **argv) 
{	int oflags=O_RDWR;
	oflags = O_RDWR | O_CREAT;
	off_t   length = 2 * 1024;
	char *name   = "/junk.01";
	int fd = shm_open(name, oflags, 0644 );
	shm_ftruncate(fd, length);
	fprintf(stderr,"Shared Mem Descriptor: fd=%d\n", fd);
	assert (fd>0);
	u_char *ptr = (u_char *) mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	fprintf(stderr, "Shared Memory Address: %p [0..%lu]\n", ptr, length-1);
	fprintf(stderr, "Shared Memory Path: /dev/shm/%s\n", name );
	assert (ptr);
	char *msg = "hello world!!\n";
	strcpy((char*)ptr,msg);
	shm_close(fd);
	return 0;
}

