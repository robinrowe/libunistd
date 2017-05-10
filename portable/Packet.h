// portable/Packet.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Packet_h
#define Packet_h

#include <memory.h>
#include <string>
#include <stdio.h>
#include "PacketSizer.h"
#include "SoftLock.h"
#include "StdFile.h"
#include "../xxHash/xxhash.h"

#if 1
#define TRACE(msg) puts("ERROR: " msg); Dump()
#else
#define TRACE(msg)
#endif

namespace portable 
{

struct PacketHeader
{	XXH64_hash_t hash;
	unsigned packetSize;
	unsigned packetId;
	PacketHeader()
	{	Reset();
	}
	void Reset()
	{	hash = 0;
		packetSize = 0;
		packetId = 0;
	}
	void ResetWrite()
	{	packetSize = GetSize();
		packetId = 0;
	}
	unsigned GetSize() const
	{	return sizeof(XXH64_hash_t) + 2 * sizeof(unsigned);
	}
	void Read(const char* packet)
	{	Reset();
		memcpy(&hash, packet, sizeof(XXH64_hash_t));
		memcpy(&packetSize,packet+sizeof(XXH64_hash_t),sizeof(packetSize));
		memcpy(&packetId,  packet+sizeof(XXH64_hash_t)+sizeof(packetSize),sizeof(packetId));
	}
	void Write(char* packet, XXH64_hash_t packetHash)
	{	memcpy(packet,(const char*) &hash,sizeof(hash));
		memcpy(packet+sizeof(XXH64_hash_t),(const char*) &packetSize, sizeof(packetSize));
		memcpy(packet+sizeof(XXH64_hash_t)+sizeof(packetSize),(const char*) &packetId, sizeof(packetId));
	}

};

class Packet
{	char* const buffer;
protected:
	typedef unsigned T;
	char* packet;
	PacketHeader header;
	const unsigned bufsize;
	bool IsEmpty() const
	{	const bool isEmpty = packet >= buffer + bufsize;
//		std::cout << "isEmpty = "<<isEmpty << std::endl;
		return isEmpty;
	}
	void Reset()
	{	packet=buffer;
		header.Reset();
	}	
public:			
	AtomicMutex ownership;
	Packet(const PacketSizer& sizer)
	:	buffer(sizer.buffer)
	,	bufsize(sizer.bufsize)
	{	Reset();
	}
	Packet(char* buffer,unsigned bufsize)
	:	buffer(buffer)
	,	bufsize(bufsize)
	{	Reset();
	}
	unsigned GetPacketId() const
	{	return header.packetId;
	}
#pragma warning(disable: 4458)
	void SetPacketId(unsigned packetId)
	{	header.packetId = packetId;
	}
	const char* GetPayload() const 
	{	return packet+ header.GetSize();
	}
	T GetCapacity() const
	{	return bufsize;
	}
	T GetPacketSize() const
	{	if(bufsize<header.packetSize)
		{	printf("ERROR: packet size overflow %u<%u\n",bufsize, header.packetSize);
			return 0;
		}	
		return header.packetSize;
	}
	T GetPacketSize(unsigned bytes) const
	{	const unsigned fullSize = GetPacketSize();
		if(!fullSize)
		{	return 0;
		}
		if(fullSize>bufsize)
		{	printf("ERROR overflow: packetSize %u > bufSize %u\n",fullSize,bufsize);
			return false;
		}
		if(fullSize > bytes)
		{//	stats.fragments++;
			return 0;
		}
		return fullSize;
	}
	T GetPayloadSize() const
	{	return GetPacketSize() - header.GetSize();
	}
	bool IsGood() const
	{	if(GetPacketSize()>GetCapacity())
		{	return false;
		}
		if(!GetPacketSize())
		{	return false;
		}
		return true;
	}
	const char* GetPacket() const
	{	return packet;
	}
	char* GetEndPtr() const
	{	return packet+header.packetSize;
	}
	void Dump() const
	{	printf("Dump Packet: size = %d, bufsize = %d",header.packetSize,bufsize);
	}
#if 0
	XXH64_hash_t GetHash(size_t offset,unsigned long long seed = 0)
	{	const size_t length = GetPacketSize() - offset;
		return XXH64(GetPayload()+offset,length,seed);
	}
#endif
	virtual bool Skip(unsigned length)
	{	(void) length;
		return true;
	}
	void Duplicate()
	{	char* end=GetEndPtr();
		const unsigned size=GetPacketSize();
		memcpy(end,packet,size);
	}
	XXH64_hash_t CalcHash(size_t length,unsigned long long seed = 0) const
	{	const XXH64_hash_t hash = XXH64(GetPayload(),GetPayloadSize(),seed);
		return hash;
	}
	XXH64_hash_t ReadHash() const
	{	if(header.hash == 0xcdcdcdcdcdcdcdcdull)
		{	puts("Invalid memory");
		}
		if(!header.hash)
		{	puts("zero hash");
		}
		return header.hash;
	}
};

}

#endif
