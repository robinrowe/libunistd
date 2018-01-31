// StdDevice.h
// Created by Robin Rowe on 2016/7/5
// Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef StdDevice_h
#define StdDevice_h

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

namespace portable
{

class StdDevice
{protected:
	int fd;
	size_t bytes;
#ifndef WIN32
	static const int O_BINARY = 0;
#endif
public:
	StdDevice()
	:	fd(-1)
	,	bytes(0)
	{}
	StdDevice(int fd)
	:	fd(fd)
	,	bytes(0)
	{}
	~StdDevice()
	{//	Close(); //Do not close devices implicitly, may live beyond temporary StdDevice.
	}
	operator int() const
	{	return fd;
	}
	bool IsGood() const
	{	return 0 <= fd;
	}
	bool operator!() const
	{	return !IsGood();
	}
// O_RDWR | O_NOCTTY | O_NDELAY...
#pragma warning (disable : 4996)
	void Close()
	{	if(IsGood())
		{	close(fd);
			fd = -1;
	}	}
	bool Open(const char* filename,int flags)
	{	fd = open(filename,flags);
		return IsGood();
	}
	bool Open(const char* filename,int flags, mode_t mode)
	{	fd = open(filename, flags, mode);
		return IsGood();
	}
	bool OpenReadOnly(const char* filename)
	{	return Open(filename, O_BINARY | O_RDONLY);
	}
	bool OpenWriteOnly(const char* filename)
	{	return Open(filename,O_BINARY | O_WRONLY);
	}
	bool OpenAppend(const char* filename)
	{	return Open(filename,O_WRONLY | O_APPEND);
	}
	int Read(char* data,size_t length)
	{	if(!IsGood())
		{	return 0;
		}
		bytes=read(fd,data,length);
		return bytes;
	}
	int Read(char& c)
	{	if(!IsGood())
		{	return 0;
		}
		bytes=read(fd,&c,1);
		return bytes;
	}
	int Write(const char* data,size_t length)
	{	if(!data || !IsGood())
		{	return false;
		}
		bytes=write(fd,data,length);
		return bytes;
	}
	bool Create(const char *pathname, mode_t mode)
	{	fd = creat(pathname,mode);
		return IsGood();
	}
	off_t Seek(off_t offset, int origin)
	{	if(!IsGood())
		{	return 0;
		}
		return lseek(fd,offset,origin);
	}
#pragma warning(default:4996)
	int Write(char c)
	{	return Write(&c,1);
	}
	int Write(const char* string)
	{	if(!string)
		{	return false;
		}
		const size_t length=strlen(string);	
		return Write(string,length);
	}
	int WriteNull()
	{	return Write("",1);
	}
};


} // portable

#endif