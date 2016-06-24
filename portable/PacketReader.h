// portable/PacketReader.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef PacketReader_h
#define PacketReader_h

#include "Packet.h"

namespace portable 
{

class PacketReader
:	public Packet
{	unsigned readOffset;
	bool IsOverflow(unsigned length) const
	{	const unsigned size = GetPacketSize();
		if(readOffset + length > size)
		{	printf("ERROR: overflow at offset %d, length %d, size %d\n",readOffset,length,size);
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
	bool IsGood() const
	{	return !IsInvalid();
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
	{	TRACE("Packet Invalidated");
		readOffset=0;
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
	{	Packet::Dump();
		const char* p=packet+readOffset;
		printf(", readOffset = %d, reading: %d, %d, %d, %d\n",readOffset,unsigned(p[0]),unsigned(p[1]),unsigned(p[2]),unsigned(p[3]));
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

}

#endif
