// Network.cpp
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include "Network.h"
#include "StdFile.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define CHECK(var) if(var!=ifstat->var) isChanged = true

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
	unsigned lineCount=3;
	for(unsigned i=0;i<ifStats.size();i++)
	{	if(proc_net_dev.Feof())
		{	break;
		}
		IfStat* ifstat=&ifStats[i];
		ifstat->Reset();
#pragma warning(disable:4996)
		const int items = fscanf(fd,
//			" %20[^:]:%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
			"%s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
			ifstat->ifname,
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
		if(-1==items)
		{	break;
		}
		if(items != 17) 
		{	printf("Invalid data read Network::UpdateIfStats() %u:%i\n",lineCount,items);
			break;
		}
		UpdateIoctls(ifstat);
		lineCount++;
	}
	UpdateRoute();
	return true;
}

struct ProcNetRoute
{	char ifname[10];
	unsigned destination;
	unsigned gateway;
	unsigned flags;
	unsigned refcnt;
	unsigned use;
	unsigned metric;
	unsigned mask;
	unsigned mtu;
	unsigned window;
	unsigned irtt;
	void Reset()
	{	ifname[0]=0;
		destination=0;
		gateway=0;
		flags=0;
		refcnt=0;
		use=0;
		metric=0;
		mask=0;
		mtu=0;
		window=0;
		irtt=0;
	}
};

/*
$ cat /proc/net/route
Iface	Destination	Gateway 	Flags	RefCnt	Use	Metric	Mask		MTU	Window	IRTT                                                       
eth0	00000000	0202000A	0003	0	0	100	00000000	0	0	0                                                                           
eth1	0002000A	00000000	0001	0	0	100	00FFFFFF	0	0	0                    
*/

void Network::UpdateRoute()
{	StdFile proc_net_route;
#ifdef WIN32
	const char* filename = "C:/Users/rrowe/proc_net_route.txt";
#else
	const char* filename = "/proc/net/route";
#endif
	if(!proc_net_route.Open(filename, "r"))
	{	return;
	}
	proc_net_route.SkipLine();
	FILE* fd = proc_net_route.GetFp();
	ProcNetRoute pnr;
	unsigned lineCount = 2;
	while(!proc_net_route.Feof())
	{	pnr.Reset();
#pragma warning(disable:4996)
		const int items = fscanf(fd,
			"%s %x %x %x %x %x %x %x %x %x %x",
			pnr.ifname,
			&pnr.destination,
			&pnr.gateway,
			&pnr.flags,
			&pnr.refcnt,
			&pnr.use,
			&pnr.metric,
			&pnr.mask,
			&pnr.mtu,
			&pnr.window,
			&pnr.irtt
		);
#pragma warning(default:4996)
		if(pnr.gateway)
		{	IfStat* ifstat=GetIfStat(pnr.ifname);
			if(ifstat)
			{	const unsigned gateway = pnr.gateway;
				CHECK(gateway);
		}	}
		if(-1==items)
		{	break;
		}
		if(items != 11) 
		{	printf("Invalid data read Network::UpdateRoute() %u:%i\n",lineCount,items);
			break;
		}
		lineCount++;
}	}

IfStat* Network::GetIfStat(const char* ifname)
{	for(unsigned i=0;i<ifStats.size();i++)
	{	if(!strcmp(ifStats[i].ifname,ifname))
		{	return &ifStats[i];
	}	}
	return nullptr;
}

inline
uint32_t GetQuad(sockaddr *addr,bool isGood=true)
{	if(!isGood || AF_INET !=addr->sa_family) 
	{	return 0;
	}
	return ((struct sockaddr_in*)addr)->sin_addr.s_addr;
}

inline
bool IsGood(int retval)
{	return -1!=retval || EADDRNOTAVAIL==errno;
}

bool Network::UpdateIoctls(IfStat* ifstat)
{	ifreq req;
	const int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(-1 == sock) 
	{	return false;
	}	
#pragma warning(disable:4996)
	strncpy(req.ifr_name, ifstat->ifname, IFNAMSIZ);
#pragma warning(default:4996)
	req.ifr_name[IFNAMSIZ - 1] = 0;
	int retval = ioctl(sock, SIOCGIFADDR, &req);
	const uint32_t address=GetQuad(&req.ifr_addr,IsGood(retval));
	CHECK(address);
	retval = ioctl(sock, SIOCGIFNETMASK, &req);
	const uint32_t netmask=GetQuad(&req.ifr_addr,IsGood(retval));;
	CHECK(netmask);
	retval = ioctl(sock, SIOCGIFBRDADDR, &req);
	const uint32_t broadcast=GetQuad(&req.ifr_addr,IsGood(retval));
	CHECK(broadcast);
	retval = ioctl(sock, SIOCGIFMTU, &req);
	const int mtu = -1!=retval ? req.ifr_mtu:0;
	CHECK(mtu);
	retval = ioctl(sock, SIOCGIFHWADDR, &req);
	uint64_t hw_address = 0;
	if(-1!=retval)
	{	unsigned char* hwaddr = (unsigned char *) &req.ifr_hwaddr.sa_data;
		memcpy(&ifstat->hw_address,hwaddr,6);
	}
	CHECK(hw_address);
#ifdef WIN32
	closesocket(sock);
#else
	close(sock);
#endif
	return true;
}
#if 0
uint32_t GetInet4(SOCKET sock)
{	struct sockaddr_storage addr;
	char ipstr[INET_ADDRSTRLEN];
	socklen_t len = sizeof addr;
	getpeername(sock, (struct sockaddr*)&addr, &len);
	if (addr.ss_family != AF_INET) 
	{	return 0;
	}
	struct sockaddr_in *s = (struct sockaddr_in *)&addr;
	inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	unsigned ip[4];
#pragma warning(disable:4996)
	scanf("%x.%x.%x.%x",ip,ip+1,ip+2,ip+3);
#pragma warning(default:4996)
	uint32_t r;
	unsigned char* p = reinterpret_cast<unsigned char*>(&r);
	p[0] = (unsigned char) ip[3];
	p[1] = (unsigned char) ip[2];
	p[2] = (unsigned char) ip[1];
	p[3] = (unsigned char) ip[0];
	return r;
}


f()
{
   int s;
   struct sockaddr_in sa;
   int sa_len;
   sa_len = sizeof(sa);
   if (getsockname(s, &sa, &sa_len) == -1) {
      perror("getsockname() failed");
      return -1;
   }
   printf("Local IP address is: %s\n", inet_ntoa(sa.sin_add r));
   printf("Local port is: %d\n", (int) ntohs(sa.sin_port));
}
#endif

}