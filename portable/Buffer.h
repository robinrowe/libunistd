// Buffer.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Buffer_h
#define Buffer_h

#include <memory.h>

namespace portable 
{

template <typename T,unsigned bufsize>
class Buffer
{	T buffer[bufsize];
	unsigned size;
public:
	Buffer()	
	{	size=0;
	}
	T* get() 
	{	return buffer;
	}
	const T* get() const
	{	return buffer;
	}
	unsigned capacity() const
	{	return bufsize;
	}
	unsigned length() const
	{	return size;
	}
	bool Append(T* data,unsigned length)
	{	if (size + length > bufsize)
		{	return false;
		}
		memcpy(buffer+size,data,length);
		size+=length;
		return true;
	}
	bool Append(T data)
	{	if (size + sizeof(T) > bufsize)
		{	return false;
		}
		buffer[size]=data;
		size++;
		return true;
	}
};

}

#endif
