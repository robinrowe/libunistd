// Network.h
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
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
	{	packets=0;
		bytes=0;
		errors=0;
		drops=0;
		overruns=0;
		multicast=0;
		in_frame=0;
		out_colls=0;
	}
	void Print(bool isIn)
	{	if(isIn)
		{	printf("packets=%llu bytes=%llu errors=%llu drops=%llu\n overruns==%llu multicast==%llu frame==%llu compress==%llu\n"
				,packets,bytes,errors,drops,overruns,multicast,in_frame,in_compress);
		}
		else
		{	printf("packets=%llu bytes=%llu errors=%llu drops=%llu\n overruns==%llu multicast==%llu colls==%llu carrier==%llu\n"
				,packets,bytes,errors,drops,overruns,multicast,out_colls,out_carrier);
	}	}
};

struct IfStat
{	char ifname[10];
	uint32_t address;
	uint32_t netmask;
	uint32_t broadcast;
	int mtu;
	uint64_t hw_address;
	uint32_t gateway;
	IfInterface in;
	IfInterface out;
	IfStat()
	{	Reset();
	}
	void Reset()
	{	ifname[0]=0;
		address=0;
		netmask=0;
		broadcast=0;
		mtu=0;
		hw_address=0;
		gateway=0;
		in.Reset();
		out.Reset();
	}
};

class Network
{	bool UpdateIoctls(IfStat* ifstat);
	void UpdateRoute();
public:
	bool isChanged;
	std::vector<IfStat> ifStats;
	Network(unsigned interfaceCount)
	:	isChanged(false)
	{	ifStats.resize(interfaceCount);
	}
	bool UpdateIfStats();
	IfStat* GetIfStat(const char* ifname);
	void PrintIfStat(IfStat* ifstat);
	void PrintStats();
};

}

#endif
