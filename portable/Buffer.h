// Buffer.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Buffer_h
#define Buffer_h

#include <memory.h>

namespace portable 
{

class Marker
{	char *buffer;
public:
	void Set(unsigned size)
	{	memcpy(buffer,&size,sizeof(unsigned));
	}
};

template <unsigned bufsize>
class Buffer
{	char buffer[bufsize];
	typedef unsigned T;
	T* size;
public:
	Buffer()	
	{	size=(unsigned*) buffer;
		*size=sizeof(T);
		buffer[size]=0;
	}
	char* get() 
	{	return buffer;
	}
	const char* get() const
	{	return buffer;
	}
	const char* data() const
	{	return buffer+sizeof(*size);
	}
	unsigned capacity() const
	{	return bufsize;
	}
	unsigned length() const
	{	return *size;
	}
	bool Append(const char* data,unsigned length)
	{	if (*size + length > bufsize)
		{	return false;
		}
		memcpy(buffer+*size,data,length);
		*size+=length;
		return true;
	}
	bool Append(unsigned data)
	{	return buffer.Append((const char*) &data,sizeof(data));
	}
	Marker GetMarker()
	{	char* p=buffer+*size;
		*size+=sizeof(T);
		return Marker(p);
	}
#if 0
	bool ends()
	{	if(*size<bufsize)
		{	buffer[*size]=0;
			return true;
		}
		return false;
	}
#endif
};

}

#endif
