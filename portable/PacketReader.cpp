// portable/PacketReader.cpp
// Libunistd Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#include "PacketReader.h"

namespace portable
{

template<>
PacketReader& operator>>(PacketReader& packet,std::string& data)
{	const bool ok = packet.Read(data);
//	std::cout<<"read string" << std::endl;
	return packet;
}

bool PacketReader::Read(std::string& s)
{	if(IsInvalid() || IsEmpty())
	{	return false;
	}
	const char* readPtr = GetReadPtr();
	const char* p = readPtr;
	const unsigned end = unsigned(GetEndPtr()-p);
	for(unsigned i = 0; i < end; i++)
	{	if(!p[i])
		{	const unsigned size = unsigned(p-readPtr);
			s=std::move(std::string(readPtr,size));
			readOffset+=size+1;
			return true;
	}	}
	printf("Packet read string overrun %u\n",end);
	Invalidate();
	return false;
}

bool PacketReader::Read(const char*& s,unsigned& size)
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

void PacketReader::Dump() const
{	Packet::Dump();
	const char* p=packet+readOffset;
	printf(", readOffset = %d, reading: %d, %d, %d, %d\n",readOffset,unsigned(p[0]),unsigned(p[1]),unsigned(p[2]),unsigned(p[3]));
	if(dumpFilename)
	{	StdFile file;
		if(!file.Open(dumpFilename,"wb"))
		{	printf("Can't open dump file %s\n",dumpFilename);
			return;
		}
		file.Write(packet,GetPacketSize()+sizeof(*packetSize));
	}
}

}
