// Network.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef Network_h
#define Network_h

#include <memory>
#include <vector>

namespace portable
{

struct IfInterface
{	unsigned long long packets;
	unsigned long long bytes;
	unsigned long long errors;
	unsigned long long drops;
	unsigned long long overruns;
	unsigned long long multicast;
	union
	{	unsigned long long in_frame;
		unsigned long long out_colls;
	};	
	union
	{	unsigned long long in_compress;
		unsigned long long out_carrier;
	};	
	IfInterface()
	{	Reset();
	}
	void Reset()
	{	bytes=0;
		errors=0;
		drops=0;
		overruns=0;
		multicast=0;
		in_frame=0;
		in_compress=0;
	}
};

struct IfStat
{	char ifname[10];
	IfInterface in;
	IfInterface out;
	IfStat()
	{	Reset();
	}
	void Reset()
	{	ifname[0]=0;
		in.Reset();
		out.Reset();
	}
};

class Network
{
public:
	std::vector<IfStat> ifStats;
	Network(unsigned interfaceCount)
	{	ifStats.resize(interfaceCount);
	}
	bool UpdateIfStats();
	IfStat* GetIfStat(const char* ifname);
};

}

#endif
