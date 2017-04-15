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
	char buffer[bufsize*depth];
	PacketWriter packetWriter0;
	PacketWriter packetWriter1;
	PacketWriter packetWriter2;
	PacketWriter* baked;
	PacketWriter* dirty;
	PacketWriter* fresh;
public:
	PacketQueue()
	:	packetWriter0(buffer,bufsize)
	,	packetWriter1(buffer+bufsize,bufsize)
	,	packetWriter2(buffer+2*bufsize,bufsize)
	{	const unsigned size = sizeof(buffer);
		memset(buffer,0,size);
		baked = &packetWriter0;
		dirty = &packetWriter1;
		fresh = &packetWriter2;
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