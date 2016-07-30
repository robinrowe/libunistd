// portable/PacketWriter.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef PacketWriter_h
#define PacketWriter_h

#include "Packet.h"

namespace portable 
{

class PacketWriter
:	public Packet
{	
public:
	PacketWriter(char* buffer,unsigned bufsize)
	:	Packet(buffer,bufsize)
	{	Reset();
	}
	PacketWriter(const PacketSizer& sizer)
	:	Packet(sizer)
	{	Reset();
	}
	PacketWriter(std::vector<char>& v)
	:	Packet(&v[0],unsigned(v.size()))
	{	Reset();
	}
	void Reset()
	{	packetId = 0;
		*packetSize=sizeof(T);
	}
	bool Skip(unsigned length) override
	{	if (*packetSize + length > bufsize)
		{	return false;
		}
		*packetSize+=length;
		return true;
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
PacketWriter& operator<<(PacketWriter& packet,T data)
{	const char* p = (const char*) &data;
	const bool ok = packet.Write(p,sizeof(data));
	return packet;
}

inline
PacketWriter& operator<<(PacketWriter& packet,const std::string& data)
{	const bool ok = packet.Write(data);
//	std::cout<<"read string" << std::endl;
	return packet;
}

inline
PacketWriter& operator<<(PacketWriter& packet,const char* data)
{	const bool ok = packet.Write(data);
//	std::cout<<"read string" << std::endl;
	return packet;
}

}
#endif
