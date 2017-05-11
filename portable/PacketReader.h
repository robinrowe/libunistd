// portable/PacketReader.h
// Libunistd Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef PacketReader_h
#define PacketReader_h

#include "Packet.h"

namespace portable 
{

class PacketReader
:	public Packet
{	const char* dumpFilename;
	const char* endPtr;
	bool IsInvalid() const
	{//	std::cout << "readOffset "<<readOffset << std::endl;
		return !packet;
	}
#if 0
	bool IsOverflow(unsigned length) const
	{	const unsigned size = GetPacketSize();
		if(readOffset + length > size)
		{	printf("ERROR: overflow at offset %d, length %d, size %d\n",readOffset,length,size);
			return true;
		}
		return false;
	}
	char* GetReadPtr()
	{	return packet+readOffset;
	}
#endif
public:
	bool ReadPacketHeader(int bytes)
	{	if(bytes < (int) header.GetSize())
		{	return false;
		}
		header.Read(packet);
		endPtr = packet + header.packetSize;
		if(packet+bytes<endPtr)
		{	return false;
		}
		packet += header.GetSize();
		return true;
	}
	PacketReader(const PacketSizer& sizer)
	:	Packet(sizer)
	,	dumpFilename(nullptr)
	{	Reset();
	}
	PacketReader(char* buffer,unsigned bufsize)
	:	Packet(buffer,bufsize)
	,	dumpFilename(nullptr)
	{	Reset();
	}
	void SetDumpFilename(const char* dumpFilename)
	{	this->dumpFilename = dumpFilename;
	}
	bool IsGood() const
	{	return !IsInvalid();
	}
	void NextInPipeline()
	{	packet += header.packetSize;
		Reset();
	}
	void SeekEnd()
	{	packet = (char*) endPtr;
	}
#if 0
		const unsigned signature=1234567789;
		if(*packetSize!=signature)
		{	std::cout<<"Bad packet"<< std::endl;
		}
		packetSize++;
		readOffset+=sizeof(signature);
	unsigned GetReadOffset() const
	{	return readOffset;
	}
#endif
	void Invalidate()
	{	TRACE("Packet Invalidated");
		packet=0;
	}
	bool Read(char* data,unsigned length)
	{	if(IsInvalid())
		{	return false;
		}
#if 0
		if(IsOverflow(length))
		{	Invalidate();
			return false;
		}
#endif
		memcpy(data,packet,length);
		packet+=length;
		return true;
	}
	bool Read(std::string& s);
	bool Read(const char*& s,unsigned& size);
	void Dump() const;
	bool Skip(unsigned length) override
	{	packet+=length;
		return true;
	}
	bool IsGoodHash() const
	{	const XXH64_hash_t readHash = GetHash();
		const XXH64_hash_t calcHash = CalcHash();
		if(readHash != calcHash)
		{	printf("Error:   read hash %llx packetId #%u\n"
			       "mismatch calc hash %llx size=%u\n",readHash,header.packetId,calcHash,header.packetSize);
			return false;
		}
		return true;
	}
	void PrintOffsets() const
	{	const unsigned remaining = unsigned(endPtr-packet);
		printf("PacketReader size=%u offset=%u remaining=%u\n",header.packetSize,header.packetSize-remaining,remaining);
	}
};

template <typename T>
PacketReader& operator>>(PacketReader& packet,T& data)
{	const bool ok = packet.Read((char*) &data,sizeof(data));
//	std::cout<<"read T" << std::endl;
	return packet;
}

template<>
PacketReader& operator>>(PacketReader& packet,std::string& data);

}

#endif
