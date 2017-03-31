// portable/PacketBuffer.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef PacketBuffer_h
#define PacketBuffer_h

#include "Packet.h"

namespace portable 
{

template <unsigned bufsize>
class PacketBuffer
{	char buffer[bufsize];
public:
	operator PacketSizer()
	{	return PacketSizer(buffer,bufsize);
	}
};

}

#endif
