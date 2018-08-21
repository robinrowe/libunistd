// portable/PacketStats.h
// License MIT
// Copyright 2016/4/20 Robin.Rowe@Cinepaint.org 

#ifndef PacketStats_h
#define PacketStats_h

#include <chrono>

namespace portable
{

class PacketStats
{	bool isVerbose; 
	typedef unsigned T;
	T first;
	T last;
	T dropped;
	T polled;
	T transmitted;
	T skipped;
	float fps;
	std::chrono::system_clock::time_point startTime;
	unsigned GetRecentDrops(unsigned packetId)
	{	if(!last || last == packetId)
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
	PacketStats()
	{	Reset();
		isVerbose = true;
		startTime = std::chrono::system_clock::now();
	}
	void Reset()
	{	first = 0;
		last = 0;
		dropped = 0;
		transmitted = 0;
		polled = 0;
		pipelined = 0;
		fragments = 0;
		errors = 0;
		skipped = 0;
		fps = 0.;
	}
	void Transmit(unsigned packetId)
	{	const unsigned recentDrops = GetRecentDrops(packetId);
		dropped += recentDrops;
		if(isVerbose && 0!=recentDrops)
		{	printf("Lost packets = %u\n",recentDrops);
		}
		last = packetId;
		if(!first)
		{	first = packetId;
		}
		transmitted++;
	}
	void Print()
	{	if(polled == transmitted)
		{	printf(".");
			return;
		}
		const unsigned recent  = transmitted - polled;
		polled = transmitted;
		std::chrono::system_clock::time_point nowTime = std::chrono::system_clock::now();
	    std::chrono::duration<double> elapsed = nowTime - startTime;
		unsigned s = (unsigned) elapsed.count();
		const unsigned h = s/3600;
		s %= 3600;
		const unsigned m = s/60;
		s %= 60;
		printf("Packet #%u: fps=%.2f:%u (%u+%u) total=%u lost=%u [%02u:%02u:%02u]\n", last,fps,recent+skipped, recent,skipped, transmitted,dropped,h,m,s);
		skipped = 0;
	}
	void Print(unsigned id,int bytes,int packetSize, int capacity)
	{	printf("id: %u packets: %i bytes: %i packetSize: %i capacity: %u fragments: %u errors: %u\n",
				id, transmitted,bytes, packetSize, capacity, fragments,errors);
	}
	void SetFps(float fps)
	{	this->fps = fps;
	}
	void AddSkipped(unsigned skipped)
	{	this->skipped += skipped;
#if 0
		if(this->skipped > 1000)
		{	puts("ERROR: skipped out");
			printf("skipped = %u\n",this->skipped);
		}
#endif
	}
	T GetLast() const
	{	return last;
	}
};

}
#endif