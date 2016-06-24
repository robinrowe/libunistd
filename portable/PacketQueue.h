// portable/PacketQueue.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef PacketQueue_h
#define PacketQueue_h

#include "PacketWriter.h"

namespace portable 
{

class PacketQueue
{	std::vector<char> buffer1;
	std::vector<char> buffer2;
	PacketWriter packetWriter1;
	PacketWriter packetWriter2;
public:
	volatile bool isOne;
	PacketQueue(unsigned bufSize)
	:	isOne(true)
	,	buffer1(bufSize)
	,	buffer2(bufSize)
	,	packetWriter1(buffer1)
	,	packetWriter2(buffer2)
	{}
	void Toggle()
	{	isOne=!isOne;
		PacketWriter& p=GetBack();
		p.Reset();
	}
	PacketWriter& GetFront()
	{	return (isOne ? packetWriter1:packetWriter2);
	}
	PacketWriter& GetBack()
	{	return (!isOne ? packetWriter1:packetWriter2);
	}
};

}
#endif