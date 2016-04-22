// Packet.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Packet_h
#define Packet_h

#include <memory.h>
#include <string>
#include <stdio.h>
#include <vector>

#ifdef _DEBUG
#define TRACE(msg) puts("ERROR: " msg)
#else
#define TRACE(msg)
#endif

namespace portable 
{

class PacketMarker
{	friend class Packet;
	char *buffer;
	unsigned offset;
public:
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
#if 0
template <unsigned bufsize>
class PacketBufferStack
{	char buffer[bufsize];
public:
	operator PacketSizer()
	{	return PacketSizer(buffer,bufsize);
	}
};

template <unsigned bufsize>
class PacketBufferHeap
{	std::unique_ptr<char[]> buffer;
public:
	PacketBufferHeap()
	{	buffer=std::make_unique<char[]>(bufsize);
	}
	operator PacketSizer()
	{	return PacketSizer(buffer.get(),bufsize);
	}
};
#endif

struct Vec3d
{	double x;
	double y;
	double z;
	int size() const
	{	return sizeof(*this) == 3*sizeof(double) ? sizeof(*this) : 0;
	}
	void Copy(const Vec3d& v)
	{	x=v.x;
		y=v.y;
		z=v.z;
	}
	void Copy(const double* v)
	{	x=v[0];
		y=v[1];
		z=v[2];
	}
	Vec3d()
	:	x(0.)
	,	y(0.)
	,	z(0.)
	{}
	Vec3d(const Vec3d& v)
	{	Copy(v);
	}
	Vec3d& operator=(const Vec3d& v)
	{	Copy(v);
	}
	bool operator!=(const Vec3d& v) const
	{	if(x!=v.x || y!=v.y || z!=v.z)
		{	return false;
		}
		return true;
	}
};

class Packet
{	char* const buffer;
protected:
	char* packet;
	typedef unsigned T;
	T* packetSize;
	const unsigned bufsize;
	unsigned readOffset;
	void Reset()
	{	*packetSize=sizeof(T);
		packet=buffer;
	}
	void Rewind(unsigned size)
	{	readOffset-=size;
	}
	bool IsEmpty() const
	{	const bool isEmpty = packet >= buffer + bufsize;
//		std::cout << "isEmpty = "<<isEmpty << std::endl;
		return isEmpty;
	}
#if 0
	bool IsEmpty() const
	{	if(IsInvalid())
		{	return true;
		}
		const unsigned size = length();
		if(readOffset>=size)
		{	return true;
		}
		return false;
	}
#endif
	bool IsOverflow(unsigned size) const
	{	return readOffset + size > GetPacketSize();
	}
	bool IsInvalid() const
	{//	std::cout << "readOffset "<<readOffset << std::endl;
		return !readOffset;
	}
	unsigned GetCapacity() const//capacity()
	{	return bufsize;
	}
	char* GetReadPtr()
	{	return buffer+readOffset;
	}
	void SetEndl()
	{	packet[GetPacketSize()-1]='\n';
	}
public:			
	Packet(char* buffer,unsigned bufsize)
	:	buffer(buffer)
	,	bufsize(bufsize)
	{	packet=buffer;
		Rewind();
	}
	const char* GetPayload() const //data()
	{	return packet+sizeof(*packetSize);
	}
	unsigned GetPacketSize() const//length()
	{	return *packetSize;
	}
	unsigned GetReadOffset() const
	{	return readOffset;
	}
	void Rewind()
	{	readOffset=sizeof(T);
		packetSize=(unsigned*) packet;
	}
	bool NextInPipeline()
	{	packet+=readOffset;
		Rewind();
		return !IsEmpty();
	}
	const char* GetPacket() const//get()
	{	return packet;
	}
	void Invalidate()
	{	readOffset=0;
		TRACE("Packet Invalidated");
	}
	char* GetEndPtr() const
	{	return packet+*packetSize;//buffer+length()
	}
};

class PacketReader
:	public Packet
{
public:
	PacketReader(std::vector<char>& buffer)
	:	Packet(&buffer[0],buffer.size())
	{	Reset();
	}
	PacketReader(char* buffer,unsigned size)
	:	Packet(buffer,size)
	{	Reset();
	}
	bool Read(char* data,unsigned length)
	{	if(IsInvalid())
		{	return false;
		}
#if 1
		if(IsOverflow(length))
		{	Invalidate();
			return false;
		}
#endif
		memcpy(data,GetReadPtr(),length);
		readOffset+=length;
		return true;
	}
	bool Read(std::string& s)
	{	if(IsInvalid() || IsEmpty())
		{	return false;
		}
		const char* readPtr = GetReadPtr();
		const char* p = readPtr;
		const char* endPtr = GetEndPtr();
		while(p<endPtr)
		{	if('\n' == *p)
			{	const unsigned size = p-readPtr;
				s=std::move(std::string(readPtr,size));
				readOffset+=size+1;
				return true;
			}
			p++;
		}
		TRACE("Packet read string");
		Invalidate();
		return false;
	}
	bool Read(const char*& s,unsigned& size)
	{	if(IsInvalid() || IsEmpty())
		{	return false;
		}
		const char* readPtr = GetReadPtr();
		const char* p = readPtr;
		const char* endPtr = GetEndPtr();
		while(p<endPtr)
		{	if('\n' == *p)
			{	size=p-readPtr;
				s = readPtr;
				readOffset+=size+1;
				return true;
			}
			p++;
		}
		size=0;
		Invalidate();
		return false;
	}
};

class PacketWriter
:	public Packet
{
public:
#if 0
	PacketWriter(const PacketSizer& sizer)
	:	Packet(sizer)
	{	Reset();
	}
#endif
	PacketWriter(std::vector<char>& buffer)
	:	Packet(&buffer[0],buffer.size())
	{	Reset();
	}
	PacketWriter(char* buffer,unsigned size)
	:	Packet(buffer,size)
	{	Reset();
	}
	PacketMarker GetMarker()
	{	char* p=GetEndPtr();
		*packetSize+=sizeof(T);
		return PacketMarker(p,*packetSize);
	}
	void SetMarker(PacketMarker& packetMarker)
	{	packetMarker.Set(GetPacketSize());
	}
	bool Write(const char* data,unsigned length)
	{	if (*packetSize + length > bufsize)
		{	return false;
		}
		memcpy(GetEndPtr(),data,length);
		*packetSize+=length;
		return true;
	}
	bool Write(const std::string& s)
	{	const unsigned length = (unsigned) s.length()+1;
		if(!Write(s.c_str(),length))
		{	return false;
		}
		SetEndl();
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
		SetEndl();
		return true;
	}
};

template <typename T>
PacketReader& operator>>(PacketReader& packet,T& data)
{	const bool ok = packet.Read((char*) &data,sizeof(data));
//	std::cout<<"read T" << std::endl;
	return packet;
}

inline
PacketReader& operator>>(PacketReader& packet,std::string& data)
{	const bool ok = packet.Read(data);
//	std::cout<<"read string" << std::endl;
	return packet;
}

template <typename T>
PacketWriter& operator<<(PacketWriter& packet,T data)
{	const char* p = (const char*) &data;
	const bool ok = packet.Write(p,sizeof(data));
	return packet;
}

}

#endif
