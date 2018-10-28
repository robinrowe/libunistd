// mman_read.cpp

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
#endif

int main(int argc, char **argv) 
{	int oflags=O_RDWR;
	int i;
	char   *name   = "/mmanjunk";
	int     fd     = shm_open(name, oflags, 0644 );
	fprintf(stderr,"Shared Mem Descriptor: fd=%d\n", fd);
	assert (fd>0);
	struct stat sb;
	fstat(fd, &sb);
	off_t length = sb.st_size ;
	u_char *ptr = (u_char *) mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	fprintf(stderr, "Shared Mem Address: %p [0..%lu]\n", ptr, length-1);
	assert (ptr);
	// hexdump first 100 bytes
	fprintf(stdout,"First 100 bytes:\n");
	for(i=0; i<100; i++)
	{	fprintf(stdout, "%02X%s", ptr[i], (i%25==24)?("\n"):(" ") );
	}
	// change 1st byte
	ptr[ 0 ] = 'H' ;
	shm_close(fd);
	return 0;
}
 