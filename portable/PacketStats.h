// portable/PacketStats.h
// License MIT
// Copyright 2016/4/20 Robin.Rowe@Cinepaint.org 

#ifndef PacketStats_h
#define PacketStats_h

class PacketStats
{	bool isVerbose; 
	unsigned first;
	unsigned last;
	unsigned dropped;
	unsigned received;
public:
	PacketStats()
	{	Reset();
		isVerbose = false;
	}
	void Reset(unsigned packets = 0)
	{	first = 0;
		last = 0;
		dropped = 0;
		received = packets;
	}
	void Update(unsigned packetId,bool isGood)
	{	if(!isGood && isVerbose)
		{	printf("Read failed packetId = %u\n", packetId);
		}
		unsigned lost = 0;
		if(packetId<last)
		{	const unsigned maxId = -1;
			lost = maxId - last;
			lost += packetId - 1;
		}
		else
		{	lost = packetId - last - 1;
		}
		if(lost)
		{	dropped += lost;
			if(isVerbose)
			{	printf("Lost packets = %u\n",lost);
		}	}
		last = packetId;
		if(isGood)
		{	received++;	
			if(!first)
			{	first = packetId;
	}	}	}
	void Print()
	{	printf("dropped: %u,received: %u,first: %u,last: %u\n",dropped,received,first,last);
	}
};

#endif