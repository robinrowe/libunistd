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
		{	puts("Packet write failed");
			return false;
		}
		memcpy(GetEndPtr(),data,length);
		*packetSize+=length;
		return true;
	}
	bool Write(const std::string& s)
	{	const unsigned zlength = (unsigned) s.length()+1;
		return Write(s.c_str(),zlength);
	}
	bool Write(const char* s)
	{	if(!s)
		{	return false;
		}
		const unsigned zlength = (unsigned) strlen(s)+1;
		return Write(s,zlength);
	}
	void Duplicate()
	{	char* end=GetEndPtr();
		const unsigned size=GetPacketSize();
		memcpy(end,packet,size);
	}
	bool WriteHash(XXH64_hash_t* hashReturn = nullptr)
	{	const XXH64_hash_t packetHash = CalcHash(GetPacketSize());
		if(hashReturn)
		{	*hashReturn = packetHash;
		}
		return Write((const char*) &packetHash,sizeof(packetHash));
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

}
#endif
