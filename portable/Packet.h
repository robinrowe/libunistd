// Packet.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Packet_h
#define Packet_h

#include <memory.h>
#include <string>

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
	T* packetSize;
	unsigned bufsize;
	unsigned readOffset;
	unsigned GetDirtySize() const
	{	return sizeof(*packetSize)+*packetSize;
	}
public:
	Packet(const PacketSizer& sizer,bool isReset=true)
	{	buffer=sizer.buffer;
		bufsize=sizer.bufsize;
		packetSize=(unsigned*) buffer;
		Rewind();
		if(isReset)
		{	Reset();
	}	}
	void Rewind()
	{	readOffset = sizeof(T);
	}
	void Reset()
	{	*packetSize=sizeof(T);
		buffer[*packetSize]=0;
	}
	bool IsInvalid(unsigned length) const
	{	return readOffset > GetDirtySize();
	}
	bool IsInvalid() const
	{	return !readOffset;
	}
	void Invalidate()
	{	readOffset = 0;
	}
	const char* get() const
	{	return buffer;
	}
	const char* data() const
	{	return buffer+sizeof(*packetSize);
	}
	unsigned length() const
	{	return *packetSize;
	}
	unsigned capacity() const
	{	return bufsize;
	}
	bool Append(const char* data,unsigned length)
	{	if (*packetSize + length > bufsize)
		{	return false;
		}
		memcpy(buffer+*packetSize,data,length);
		*packetSize+=length;
		return true;
	}
	PacketMarker GetMarker()
	{	char* p=buffer+*packetSize;
		*packetSize+=sizeof(T);
		return PacketMarker(p,*packetSize);
	}
	bool Append(const std::string& s)
	{	const unsigned length = (unsigned) s.length()+1;
		if(!Append(s.c_str(),length))
		{	return false;
		}
		buffer[*packetSize-1]='\n';
		return true;
	}
	bool Append(const char* s)
	{	if(!s)
		{	return false;
		}
		const unsigned length = (unsigned) strlen(s)+1;
		if(!Append(s,length))
		{	return false;
		}
		buffer[*packetSize-1]='\n';
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
	bool IsEmpty() const
	{	return readOffset ? readOffset>=GetDirtySize():true;
	}
	bool Read(char* data,unsigned length)
	{	if(!readOffset)
		{	return false;
		}
		if(IsInvalid(length))
		{	Invalidate();
			return false;
		}
		memcpy(data,buffer+readOffset,length);
		readOffset+=length;
		return true;
	}
	bool Read(unsigned& data)
	{	return Read((char*) &data,sizeof(data));
	}
	bool Read(int& data)
	{	return Read((char*) &data,sizeof(data));
	}
	bool Read(std::string& s)
	{	if(IsInvalid() || IsEmpty())
		{	return false;
		}
		const char* readPtr = buffer+readOffset;
		const char* p = readPtr;
		const char* endPtr = buffer+GetDirtySize();
		while(p<endPtr)
		{	if('\n' == *p)
			{	const unsigned length = p-readPtr;
				s=std::string(readPtr,length);
				readOffset+=length+1;
				return true;
			}
			p++;
		}
		Invalidate();
		return false;
	}
	bool Read(const char*& s,unsigned& length)
	{	if(IsInvalid() || IsEmpty())
		{	return false;
		}
		const char* readPtr = buffer+readOffset;
		const char* p = readPtr;
		const char* endPtr = buffer+GetDirtySize();
		while(p<endPtr)
		{	if('\n' == *p)
			{	length=p-readPtr;
				s = readPtr;
				readOffset+=length+1;
				return true;
			}
			p++;
		}
		Invalidate();
		return false;
	}
};

}

#endif
