// Packet.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Packet_h
#define Packet_h

#include <memory.h>
#include "BsdSocket.h"

namespace portable 
{

class PacketMarker
{	char *buffer;
	unsigned offset;
public:
	PacketMarker(char* buffer,unsigned offset)
	:	buffer(buffer)
	,	offset(offset)
	{}
	void Set(unsigned size)
	{	offset = size - offset;
		memcpy(buffer,&size,sizeof(unsigned));
	}
};

template <unsigned bufsize>
class Packet
{	char buffer[bufsize];
	typedef unsigned T;
	T* size;
public:
	Packet()
	{	Reset();
	}
	void Reset()
	{	size=(unsigned*) buffer;
		*size=sizeof(T);
		buffer[*size]=0;
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
	{	return Append((const char*) &data,sizeof(data));
	}
	bool Append(long long data)
	{	return Append((const char*) &data,sizeof(data));
	}
	PacketMarker GetMarker()
	{	char* p=buffer+*size;
		*size+=sizeof(T);
		return PacketMarker(p,*size);
	}
	bool SendTo(BsdSocket& socket)
	{	return socket.SendTo(get(),length());
	}
	bool Append(char c)
	{	if(*size<bufsize)
		{	buffer[*size]=c;
			*size++;
			return true;
		}
		return false;
	}
};

}

#endif
