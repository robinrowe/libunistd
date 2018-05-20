// unistd/utime.h
// Copyright 2018/5/19 Robin.Rowe@CinePaint.org
// License MIT open source

#ifndef utime_h
#define utime_h

#include <sys/types.h>

struct utimbuf
{	time_t actime;	//    Access time. 
	time_t modtime;	//   Modification time. 
};

inline
int utime(const char *, const struct utimbuf *)
{	return 0;
}

#endif