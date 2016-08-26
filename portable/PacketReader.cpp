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
{	const char* cs ="";
	unsigned stringLength = 0;
	if(!Read(cs,stringLength))
	{	return false;
	}
	s = cs;
	return true;
}

bool PacketReader::Read(const char*& s,unsigned& stringLength)
{	stringLength = 0;
	if(IsInvalid())
	{	return false;
	}
	if(IsEmpty())
	{	return false;
	}
	const char* readPtr = GetReadPtr();
	const char* p = readPtr;
	const unsigned size = unsigned(GetEndPtr() - p);
	for(unsigned i=0;i<size;i++)
	{	if(0 == p[i])
		{	stringLength = i;
			s = readPtr;
			readOffset += i;
			readOffset++;
			return true;
		}
	}
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
