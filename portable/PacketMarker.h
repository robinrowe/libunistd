// portable/PacketMarker.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef PacketMarker_h
#define PacketMarker_h

#include "Packet.h"

namespace portable 
{

class PacketMarker
{	friend class Packet;
	char *buffer;
	unsigned offset;
public:
	PacketMarker(char* buffer,unsigned offset)
	:	buffer(buffer)
	,	offset(offset)
	{}
	void SetDelta(unsigned offset)
	{	SetValue(offset - this->offset);
	}
	void SetValue(unsigned value)
	{	memcpy(buffer,&value,sizeof(unsigned));
	}
	template <typename T>
	void SetValue(T value)
	{	memcpy(buffer,&value,sizeof(T));
	}
};



}

#endif
