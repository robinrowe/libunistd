// portable/PacketStats.h
// License MIT
// Copyright 2016/4/20 Robin.Rowe@Cinepaint.org 

#ifndef PacketStats_h
#define PacketStats_h

namespace portable
{

class PacketStats
{	bool isVerbose; 
	typedef unsigned T;
	T first;
	T last;
	T dropped;
	T polled;
	T received;
	unsigned GetRecentDrops(unsigned packetId)
	{	if(!last)
		{	return 0;
		}
		unsigned recentDrops = 0;
		if(packetId>last)
		{	recentDrops = packetId - last - 1;
			return recentDrops;
		}
		const unsigned maxId = -1;
		recentDrops = maxId - last;
		recentDrops += packetId - 1;
		return recentDrops;
	}
public:
	T pipelined;
	T fragments;
	T errors;
	float fps;
	PacketStats()
	{	Reset();
		isVerbose = true;
	}
	void Reset()
	{	first = 0;
		last = 0;
		dropped = 0;
		received = 0;
		polled;
		pipelined = 0;
		fragments = 0;
		errors = 0;
		fps = 0.;
	}
	void Received(unsigned packetId)
	{	const unsigned recentDrops = GetRecentDrops(packetId);
		dropped += recentDrops;
		if(isVerbose && 0!=recentDrops)
		{	printf("Lost packets = %u\n",recentDrops);
		}
		last = packetId;
		if(!first)
		{	first = packetId;
		}
		received++;	
	}
	void Print()
	{	if(polled == received)
		{	printf(".");
			return;
		}
		const unsigned recent  = received - polled;
		polled = received;
		printf("%u: fps=%.2f received=%u recent=%u dropped=%u\n",last,fps,received,recent,dropped);
	}
	void Print(unsigned id,int bytes,int packetSize, int capacity)
	{	printf("id: %u packets: %i bytes: %i packetSize: %i capacity: %u fragments: %u errors: %u\n",
				id, received,bytes, packetSize, capacity, fragments,errors);	
	}
};

#if 0
struct PacketStatus
{	unsigned packetCount;
	unsigned printStatusMax;
	unsigned packetFragments;
	unsigned packetErrors;
	PacketStatus()
	:	packetCount(0)
	,	packetFragments(0)
	,	packetErrors(0)
	{	printStatusMax = 60*60;
		dumpFilename = "PacketDump.bin";
	}
	bool IsActive() const
	{	return 0 != printStatusMax;
	}
	const char* dumpFilename;
	void Print(unsigned id,int bytes,int packetSize, int capacity)
	{	//if(packetCount >= printStatusMax || packetErrors == 1)
		{	packetCount = 0;
			printf("id: %u packets: %i bytes: %i packetSize: %i capacity: %u fragments: %u errors: %u\n",
					id,    packetCount,bytes,   packetSize, capacity, packetFragments,packetErrors);	
	}	}
};
#endif
}
#endif