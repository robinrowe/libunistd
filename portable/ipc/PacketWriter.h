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
	bool IsGood() const override
	{	if(!header.isGood)
		{	return false;
		}
		return Packet::IsGood();
	}
	void Reset() override
	{	header.ResetWrite();
	}
	bool Skip(unsigned length) override
	{	if (header.packetSize + length > bufsize)
		{	return false;
		}
		header.packetSize += length;
		return true;
	}
	bool Write(const char* data,unsigned length)
	{	if (header.packetSize + length > bufsize)
		{	puts("Packet write failed");
			return false;
		}
		memcpy(GetEndPtr(),data,length);
		header.packetSize += length;
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
	char* GetEndPtr() const
	{	return packet+header.packetSize;
	}
	void Duplicate()
	{	char* end=GetEndPtr();
		const unsigned size=GetPacketSize();
		memcpy(end,packet,size);
	}
	bool WriteHash(XXH64_hash_t* hashReturn = nullptr)
	{	header.WriteSizeId(buffer);
		const XXH64_hash_t packetHash = CalcHash();
		if(hashReturn)
		{	*hashReturn = packetHash;
		}
		header.WriteHash(buffer,packetHash);
		return true;
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
