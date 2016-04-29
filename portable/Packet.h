// Packet.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Packet_h
#define Packet_h

#include <memory.h>
#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;

#if 1
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
	unsigned GetCapacity() const//capacity()
	{	return bufsize;
	}
	void SetEndl()
	{	packet[GetPacketSize()-1]='\n';
	}
	void Init()
	{	packet=buffer;
		packetSize=(unsigned*) buffer;
	}	
public:			
	Packet(const PacketSizer& sizer)
	:	buffer(sizer.buffer)
	,	bufsize(sizer.bufsize)
	{	Init();
	}
	Packet(char* buffer,unsigned bufsize)
	:	buffer(buffer)
	,	bufsize(bufsize)
	{}
	const char* GetPayload() const //data()
	{	return packet+sizeof(*packetSize);
	}
	unsigned GetPacketSize() const//length()
	{	return *packetSize;
	}
	const char* GetPacket() const//get()
	{	return packet;
	}
	char* GetEndPtr() const
	{	return packet+*packetSize;//buffer+length()
	}
};

class PacketReader
:	public Packet
{	unsigned readOffset;
	bool IsOverflow(unsigned length) const
	{	const unsigned size = GetPacketSize();
		if(readOffset + length > size)
		{	cout<<"ERROR: overflow "<<readOffset<<" "<<length<<" "<<size<<endl;
			return true;
		}
		return false;
	}
	bool IsInvalid() const
	{//	std::cout << "readOffset "<<readOffset << std::endl;
		return !readOffset;
	}
	char* GetReadPtr()
	{	return packet+readOffset;
	}
	void InitReader()
	{	readOffset=sizeof(T);
	}
public:
	PacketReader(const PacketSizer& sizer)
	:	Packet(sizer)
	{	InitReader();
	}
	PacketReader(char* buffer,unsigned bufsize)
	:	Packet(buffer,bufsize)
	{	InitReader();
	}
	void Init()
	{	Packet::Init();
		InitReader();
	}	
	void NextInPipeline()
	{	packet+=*packetSize;
		packetSize=(unsigned*)packet;
		InitReader();
	}
#if 0
		const unsigned signature=1234567789;
		if(*packetSize!=signature)
		{	std::cout<<"Bad packet"<< std::endl;
		}
		packetSize++;
		readOffset+=sizeof(signature);
#endif
	unsigned GetReadOffset() const
	{	return readOffset;
	}
	void Invalidate()
	{	readOffset=0;
		TRACE("Packet Invalidated");
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
			{	const unsigned size = unsigned(p-readPtr);
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
			{	size=unsigned(p-readPtr);
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
	void Dump() const
	{	const char* p=packet+readOffset;
		cout <<"Packet dump: "<<packetSize[0]<<" "<<packetSize[1]<<" "<<packetSize[2]
		<< " readOffset = " << readOffset
		<< ", "<<packetSize[readOffset] << " - "
		<< unsigned(p[0])<<" "<<unsigned(p[1])<<" "<<unsigned(p[2])<<" "<<unsigned(p[3])<<endl;
	}
};

class PacketWriter
:	public Packet
{
public:
	PacketWriter(const PacketSizer& sizer)
	:	Packet(sizer)
	{	*packetSize=sizeof(T);
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
	void Duplicate()
	{	char* end=GetEndPtr();
		const unsigned size=GetPacketSize();
		memcpy(end,packet,size);
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

inline
PacketWriter& operator<<(PacketWriter& packet,std::string& data)
{	const bool ok = packet.Write(data);
//	std::cout<<"read string" << std::endl;
	return packet;
}

}

#endif
