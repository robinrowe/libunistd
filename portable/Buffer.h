// Buffer.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Buffer_h
#define Buffer_h

#include <memory.h>

namespace portable 
{

template <unsigned bufsize>
class Buffer
{	char buffer[bufsize];
	unsigned size;
public:
	Buffer()	
	{	buffer[0]=0;
		size=0;
	}
	char* get() 
	{	return buffer;
	}
	const char* get() const
	{	return buffer;
	}
	unsigned capacity() const
	{	return bufsize;
	}
	unsigned length() const
	{	return size;
	}
	bool Append(const char* data,unsigned length)
	{	if (size + length > bufsize)
		{	return false;
		}
		memcpy(buffer+size,data,length);
		size+=length;
		return true;
	}
	bool Append(unsigned data)
	{	return buffer.Append((const char*) &data,sizeof(data));
	}
	bool ends()
	{	if(size<bufsize)
		{	buffer[size]=0;
			return true;
		}
		return false;
	}
};

}

#endif
