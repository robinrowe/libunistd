// unistd/more/shm_more.h
// 2018/10/28 Robin.Rowe@CinePaint.org

#ifndef shm_more_h
#define shm_more_h

#ifndef _WIN32

#include <sys/stat.h>
#include <fcntl.h>

#define shm_close close
#define shm_ftruncate ftruncate
#define shm_flush(fd) 

int shm_size(int fd)
{	struct stat sb;
	fstat(fd, &sb);
	off_t length = sb.st_size;
	return int(length);
}
#endif

#endif
