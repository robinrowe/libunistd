// Network.cpp
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include "Network.h"
#include "StdFile.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace portable
{

/* (size = 450 +4 = 77 + 123 + 2*125 + 4 LF)
root@vm-ubuntu:/opt/toolchains# cat /proc/net/dev
Inter-|   Receive                                                |  Transmit
 face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed
enp0s3: 616352817  446732    0    0    0     0          0         0 13516902  165770    0    0    0     0       0          0
    lo:  485454    4814    0    0    0     0          0         0   485454    4814    0    0    0     0       0          0
*/ 

bool Network::UpdateIfStats()
{	StdFile proc_net_dev;
#ifdef WIN32
	const char* filename = "C:/Users/rrowe/proc_net_dev.txt";
#else
	const char* filename = "/proc/net/dev";
#endif
	if(!proc_net_dev.Open(filename, "r"))
	{	return false;
	}
	proc_net_dev.SkipLine();
	proc_net_dev.SkipLine();
	FILE* fd = proc_net_dev.GetFp();
	for(unsigned i=0;i<ifStats.size();i++)
	{	IfStat* ifstat=&ifStats[i];
		if(proc_net_dev.Feof())
		{	ifstat->Reset();
			continue;
		}
#pragma warning(disable:4996)
		const int items = fscanf(fd,
			" %20[^:]:%llu %llu %llu %llu %llu %llu %llu %llu "
			"%llu %llu %llu %llu %llu %llu %llu %llu",
			&ifstat->ifname,
			&ifstat->in.bytes,    
			&ifstat->in.packets,
			&ifstat->in.errors,   
			&ifstat->in.drops,
			&ifstat->in.overruns,     
			&ifstat->in.in_frame,
			&ifstat->in.in_compress, 
			&ifstat->in.multicast,
			&ifstat->out.bytes,   
			&ifstat->out.packets,
			&ifstat->out.errors,  
			&ifstat->out.drops,
			&ifstat->out.overruns,    
			&ifstat->out.out_colls,
			&ifstat->out.out_carrier, 
			&ifstat->out.out_carrier
		);
#pragma warning(default:4996)
		if (items != 17) 
		{	puts("Invalid data read");
		}
	}
	return true;
}

IfStat* Network::GetIfStat(const char* ifname)
{	for(unsigned i=0;i<ifStats.size();i++)
	{	if(!strcmp(ifStats[i].ifname,ifname))
		{	return &ifStats[i];
	}	}
	return nullptr;
}


}