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
{	friend class Packet;
	char *buffer;
	unsigned offset;
	PacketMarker(char* buffer,unsigned offset)
	:	buffer(buffer)
	,	offset(offset)
	{}
	void Set(unsigned size)
	{	offset = size - offset;
		memcpy(buffer,&offset,sizeof(unsigned));
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

struct Vec3d
{	double x;
	double y;
	double z;
	int size() const
	{	return sizeof(*this) == 3*sizeof(double) ? sizeof(*this) : 0;
	}
};



class Packet
{	char* const buffer;
	typedef unsigned T;
	T* packetSize;
	const unsigned bufsize;
	unsigned readOffset;
public:
	Packet(const PacketSizer& sizer,bool isReset=true)
	:	buffer(sizer.buffer)
	,	bufsize(sizer.bufsize)
	{	packetSize=(unsigned*) buffer;
		Rewind();
		if(isReset)
		{	Reset();
	}	}
	const char* GetEnd() const
	{	return buffer+*packetSize;
	}
	void Reset()
	{	*packetSize=sizeof(T);
	}
	void Rewind()
	{	readOffset = sizeof(T);
	}
	void Rewind(unsigned size)
	{	readOffset-=size;
	}
	bool IsInvalid(unsigned size) const
	{	return readOffset + size > length();
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
	unsigned ReadOffset() const
	{	return readOffset;
	}
	unsigned capacity() const
	{	return bufsize;
	}
	const char* GetPutPtr() const
	{	return buffer+*packetSize;
	}
	bool Write(const char* data,unsigned length)
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
	void SetMarker(PacketMarker& packetMarker)
	{	packetMarker.Set(length());
	}
	bool Write(const std::string& s)
	{	const unsigned length = (unsigned) s.length()+1;
		if(!Write(s.c_str(),length))
		{	return false;
		}
		buffer[*packetSize-1]='\n';
		return true;
	}
	bool Write(const char* s)
	{	if(!s)
		{	return false;
		}
		const unsigned length = (unsigned) strlen(s)+1;
		if(!Write(s,length))
		{	return false;
		}
		buffer[*packetSize-1]='\n';
		return true;
	}
	bool Write(unsigned data)
	{	const char* p = (const char*) &data;
		return Write(p,sizeof(data));
	}
	bool Write(int data)
	{	const char* p = (const char*) &data;
		return Write(p,sizeof(data));
	}
	bool Write(unsigned long long data)
	{	const char* p = (const char*) &data;
		return Write(p,sizeof(data));
	}
	bool Write(float data)
	{	const char* p = (const char*) &data;
		return Write(p,sizeof(data));
	}
	bool Write(double data)
	{	const char* p = (const char*) &data;
		return Write(p,sizeof(data));
	}
	bool Write(const Vec3d& data)
	{	const char* p = (const char*) &data;
		return Write(p,sizeof(data));
	}
	bool IsEmpty() const
	{	if(!readOffset)
		{	return true;
		}
		const unsigned size = length();
		if(readOffset>=size)
		{	return true;
		}
		return false;
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
		const char* endPtr = buffer+length();
		while(p<endPtr)
		{	if('\n' == *p)
			{	const unsigned size = p-readPtr;
				s=std::move(std::string(readPtr,size));
				readOffset+=size+1;
				return true;
			}
			p++;
		}
		Invalidate();
		return false;
	}
	bool Read(const char*& s,unsigned& size)
	{	if(IsInvalid() || IsEmpty())
		{	return false;
		}
		const char* readPtr = buffer+readOffset;
		const char* p = readPtr;
		const char* endPtr = buffer+length();
		while(p<endPtr)
		{	if('\n' == *p)
			{	size=p-readPtr;
				s = readPtr;
				readOffset+=size+1;
				return true;
			}
			p++;
		}
		Invalidate();
		return false;
	}
	bool Read(Vec3d& v)
	{	return Read((char*)&v,sizeof(v));
	}
};

}

#endif
