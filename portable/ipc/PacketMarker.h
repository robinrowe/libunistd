// portable/PacketMarker.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef PacketMarker_h
#define PacketMarker_h

#include "PacketWriter.h"
#include "PacketReader.h"

namespace portable 
{

template <typename T>
class PacketMarker
{	char *data;
public:
	PacketMarker(PacketWriter& packet)
	:	data(packet.GetEndPtr())
	{	packet.Skip(sizeof(T));
	}
	void SetValue(const T& value)
	{	memcpy(data,&value,sizeof(T));
	}
	unsigned GetSize() const
	{	return sizeof(T);
	}
};

class PacketDelta
{	PacketWriter& packet;
	PacketMarker<unsigned> marker;
	unsigned offset;
public:
	PacketDelta(PacketWriter& packet)
	:	packet(packet)
	,	marker(packet)
	{	offset = packet.GetPacketSize();
	}
	void SetValue()
	{	const unsigned delta = packet.GetPacketSize() - offset;
		marker.SetValue(delta);
	}
};

}

#endif
