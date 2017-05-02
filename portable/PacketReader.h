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
{	unsigned readOffset;
	const char* dumpFilename;
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
		dumpFilename=0;
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
	void SeekEnd()
	{	readOffset = GetPacketSize();
	}
	void SetDumpFilename(const char* dumpFilename)
	{	this->dumpFilename = dumpFilename;
	}
	bool IsGood() const
	{	return !IsInvalid();
	}
	void Init()
	{	//Packet::Init();
		InitReader();
	}	
	void NextInPipeline()
	{	packet+=*packetSize;
		packetSize=(T*)packet;
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
	bool Read(std::string& s);
	bool Read(const char*& s,unsigned& size);
	void Dump() const;
	bool Skip(unsigned length) override
	{	readOffset+=length;
		return true;
	}
	void SkipHash()
	{	Skip(sizeof(XXH64_hash_t));
	}
	bool IsGoodHash() const
	{	const XXH64_hash_t readHash = ReadHash();
		const XXH64_hash_t calcHash = CalcHash(GetPacketSize()-sizeof(XXH64_hash_t));
		if(readHash != calcHash)
		{	printf("Error: read hash %llx mismatch packetId #%u\n"
			       "       calc hash %llx at %u\n",readHash,packetId,calcHash,unsigned(GetPacketSize()-sizeof(XXH64_hash_t)));
			return false;
		}
		return true;
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
