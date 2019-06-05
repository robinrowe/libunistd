// mman_server.cpp
// 2018/10/28 Robin.Rowe@CinePaint.org
// Inspired by http://users.cs.cf.ac.uk/Dave.Marshall/C/node27.html

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "../../unistd/more/shm_more.h"

int main(int argc, char **argv) 
{	int oflags=O_RDWR;
	oflags = O_RDWR | O_CREAT;
	off_t length = 2 * 1024;
	const char *name = "/mmanjunk";
	int fd = shm_open(name, oflags, 0644 );
	shm_ftruncate(fd, length);
	fprintf(stderr,"Shared Mem Descriptor: fd=%d\n", fd);
	assert (fd>0);
	u_char *ptr = (u_char *) mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	fprintf(stderr, "Shared Memory Address: %p [0..%lu]\n", ptr, length-1);
	fprintf(stderr, "Shared Memory Path: /dev/shm/%s\n", name );
	assert (ptr);
	const char *msg = "hello world!!\n";
#pragma warning(disable : 4996)
	strcpy((char*)ptr,msg);
#pragma warning(default : 4996)
	shm_close(fd);
	return 0;
}

/* 
Shared Mem Descriptor: fd=1
Shared Memory Address: 000002DD34910000 [0..2047]
Shared Memory Path: /dev/shm//mmanjunk

*/