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
	s = readPtr;
	stringLength = 0;
	while(readPtr<endPtr)
	{	if(0 == *readPtr)
		{	stringLength = unsigned(readPtr-s);
			readPtr++;
			return true;
		}
		readPtr++;
	}
	stringLength = unsigned(readPtr-s);
	return false;
}

void PacketReader::Dump() const
{	Packet::Dump();
#if 0
const char* p=packet+readOffset;
	printf(", readOffset = %d, reading: %d, %d, %d, %d\n",readOffset,unsigned(p[0]),unsigned(p[1]),unsigned(p[2]),unsigned(p[3]));
#endif
	if(dumpFilename)
	{	StdFile file;
		if(!file.Open(dumpFilename,"wb"))
		{	printf("Can't open dump file %s\n",dumpFilename);
			return;
		}
		file.Write(packet,GetPacketSize());
	}
}

}
