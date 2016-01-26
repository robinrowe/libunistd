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

struct PacketSizer
{	char* buffer;
	unsigned bufsize;
	PacketSizer(char* buffer,unsigned bufsize)
	:	buffer(buffer)
	,	bufsize(bufsize)
	{}
};

template <unsigned bufsize>
class PacketBuffer
{	char buffer[bufsize];
public:
	operator PacketSizer()
	{	return PacketSizer(buffer,bufsize);
	}
};

class Packet
{	char* buffer;
	typedef unsigned T;
	T* size;
	unsigned bufsize;
public:
	Packet(const PacketSizer& sizer)
	{	buffer=sizer.buffer;
		bufsize=sizer.bufsize;
		Reset();
	}
	void Reset()
	{	size=(unsigned*) buffer;
		*size=sizeof(T);
		buffer[*size]=0;
	}
	const char* get() const
	{	return buffer;
	}
	const char* data() const
	{	return buffer+sizeof(*size);
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
	PacketMarker GetMarker()
	{	char* p=buffer+*size;
		*size+=sizeof(T);
		return PacketMarker(p,*size);
	}
	bool SendTo(BsdSocket& socket)
	{	return socket.SendTo(get(),length());
	}
	bool Append(const std::string& s)
	{	const size_t length = s.size()+1;
		if(!Append(s.c_str(),length))
		{	return false;
		}
		buffer[*size-1]='\n';
		return true;
	}
	bool Append(const char* s)
	{	if(!s)
		{	return false;
		}
		const size_t length = strlen(s)+1;
		if(!Append(s,length))
		{	return false;
		}
		buffer[*size-1]='\n';
		return true;
	}
	bool Append(unsigned data)
	{	const char* p = (const char*) &data;
		return Append(p,sizeof(data));
	}
	bool Append(long long data)
	{	const char* p = (const char*) &data;
		return Append(p,sizeof(data));
	}
#if 0
	bool Append(char c)
	{	return Append(*this,data);
	}
#endif
};

}

#endif
