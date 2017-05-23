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
{	const char* readPtr;
	const char* endPtr;
	const char* dumpFilename;
	bool IsInvalid() const
	{//	std::cout << "readOffset "<<readOffset << std::endl;
		return !readPtr;
	}
public:
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
	void Reset() override
	{	readPtr = 0;
		endPtr = 0;
		Packet::Reset();
	}
	bool ReadPacketHeader(int bytes)
	{	if(bytes < (int) header.GetSize())
		{	return false;
		}
		header.Read(packet);
		if(!header.packetSize)
		{	return false;
		}
		endPtr = packet + header.packetSize;
		if(packet+bytes<endPtr)
		{	return false;
		}
		readPtr = packet + header.GetSize();
		return true;
	}
	void SetDumpFilename(const char* dumpFilename)
	{	this->dumpFilename = dumpFilename;
	}
	bool IsGood() const override
	{	return !IsInvalid();
	}
	void NextInPipeline()
	{	readPtr = endPtr;
		Reset();
	}
	void SeekEnd()
	{	readPtr = (char*) endPtr;
	}
	void Invalidate()
	{	TRACE("Packet Invalidated");
		readPtr=0;
	}
	bool Read(char* data,unsigned length)
	{	if(IsInvalid())
		{	return false;
		}
		memcpy(data,readPtr,length);
		readPtr+=length;
		return true;
	}
	bool Read(std::string& s);
	bool Read(const char*& s,unsigned& size);
	void Dump() const;
	bool Skip(unsigned length) override
	{	readPtr+=length;
		return true;
	}
	bool IsGoodHash() const
	{	const XXH64_hash_t readHash = GetHash();
		const XXH64_hash_t calcHash = CalcHash();
		if(readHash != calcHash)
		{	printf("Error:   read hash %llx packetId #%u\n"
			       "mismatch calc hash %llx size=%u\n",readHash,header.packetId,calcHash,header.packetSize);
			PrintOffsets();
			return false;
		}
		return true;
	}
	void PrintOffsets() const
	{	const unsigned remaining = unsigned(endPtr-readPtr);
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
