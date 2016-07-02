// portable/PacketQueue.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef PacketQueue_h
#define PacketQueue_h

#include "PacketWriter.h"

namespace portable 
{

template <size_t bufsize>
class PacketQueue
{	static const size_t depth = 3;
	char buffer[bufsize][depth];
	PacketWriter packetWriter0;
	PacketWriter packetWriter1;
	PacketWriter packetWriter2;
	PacketWriter* baked;
	PacketWriter* dirty;
	PacketWriter* fresh;
public:
	PacketQueue(unsigned bufSize)
	:	packetWriter0(buffer[0],bufsize)
	,	packetWriter1(buffer[1],bufsize)
	,	packetWriter2(buffer[2],bufsize)
	{	baked = &packetWriter0;
		dirty = &packetWriter1;
		fresh = &packetWriter2;
		memset(buffer,0,sizeof(buffer));
	}
	void Advance()
	{	PacketWriter* oldBaked = baked;
		baked = dirty;//must be first
		dirty = fresh;
		fresh = oldBaked;//must be last
		dirty->Reset();
	}
	PacketWriter& GetDirty()
	{	return *dirty;
	}
	PacketWriter& GetBaked()
	{	return *baked;
	}
};

}
#endif