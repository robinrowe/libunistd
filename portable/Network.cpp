// Network.cpp
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "Network.h"
#include "StdFile.h"
#include "StdBlob.h"

#ifdef WIN32
#define SUPPRESS_SECURITY_WARNING __pragma(warning(suppress:4996))
#else
#define SUPPRESS_SECURITY_WARNING 
#endif

#define CHECK(var) if(var!=ifstat->var){ ifstat->var=var; isChanged = true;}

namespace portable
{

const char* output_proc_net_route =
"Iface	Destination	Gateway 	Flags	RefCnt	Use	Metric	Mask		MTU	Window	IRTT\n"
"eth0	00000000	0202000A	0003	0	0	100	00000000	0	0	1\n"
"eth1	0002000A	00000000	0001	0	0	100	00FFFFFF	0	0	2\n";

const char* output_proc_net_dev =
"Inter-|   Receive                                                |  Transmit\n"
" face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed\n"
"    lo:   13380      87    0    0    0     0          0         0    13380      87    0    0    0     0       0          0\n"
"  eth0: 279068630 1507895    0    0    0     0          0         0 567397982  852533    0    0    0     0       0          0\n"
"  eth1: 1090810015 4017054   42    0    0     0          0         0 13975469  134398    0    0    0     0       0          0\n";

Network::Network(unsigned interfaceCount)
:	isChanged(false)
,	interfaces(0)
{	ifStats.resize(interfaceCount);
	route_filename = "/proc/net/route";
	dev_filename = "/proc/net/dev";
}

/* (size = 450 +4 = 77 + 123 + 2*125 + 4 LF)
root@vm-ubuntu:/opt/toolchains# cat /proc/net/dev
Inter-|   Receive                                                |  Transmit
 face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed
enth0:  616352817  446732    0    0    0     0          0         0 13516902  165770    0    0    0     0       0          0
enth1:   485454    4814    0    0    0     0          0         0   485454    502     0    0    0     0       0          0
*/ 

bool Network::UpdateIfStats()
{	
#ifdef WIN32
	StdBlob proc_net_dev(output_proc_net_dev);
#else
	StdFile proc_net_dev;
	if(!proc_net_dev.Open(dev_filename, "r"))
	{	return false;
	}
#endif
	proc_net_dev.SkipLine();
	proc_net_dev.SkipLine();
	unsigned lineCount=3;
	interfaces = 0;
	while(!proc_net_dev.IsFeof())
	{	if(interfaces>=ifStats.size())
		{	proc_net_dev.SkipLine();
			interfaces++;
		}
		IfStat* ifstat=&ifStats[interfaces];
		ifstat->Reset();
SUPPRESS_SECURITY_WARNING
		const int items = proc_net_dev.fscanf(
			" %20[^:]:%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
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

		interfaces++;
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
{
#ifdef WIN32
	StdBlob proc_net_route(output_proc_net_route);
#else
	StdFile proc_net_route;
	if(!proc_net_route.Open(route_filename, "r"))
	{	return;
	}
#endif
	proc_net_route.SkipLine();
	ProcNetRoute pnr;
	unsigned lineCount = 2;
	while(!proc_net_route.IsFeof())
	{	pnr.Reset();
SUPPRESS_SECURITY_WARNING
		const int items = proc_net_route.fscanf(
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

//		printf("Network::ifname=%s gateway=%u\n",pnr.ifname,pnr.gateway);
		if(pnr.gateway)
		{	IfStat* ifstat=GetIfStat(pnr.ifname);
			if(ifstat)
			{	const unsigned gateway = pnr.gateway;
				CHECK(gateway)
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
{
#ifdef WIN32
	ifstat->address=	0x0100007f;
	ifstat->broadcast=	0x0000007f;
	ifstat->netmask=	0x00ffffff;
	ifstat->gateway=	0x04030201;
	ifstat->hw_address=0x0102030405060708ULL;
	ifstat->mtu=32;
	return true;
#else
	const int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(-1 == sock) 
	{	return false;
	}	
	ifreq req;
	strncpy(req.ifr_name, ifstat->ifname, IFNAMSIZ);
	req.ifr_name[IFNAMSIZ - 1] = 0;
	int retval = ioctl(sock, SIOCGIFADDR, &req);
	const uint32_t address=GetQuad(&req.ifr_addr,IsGood(retval));
	CHECK(address)
	retval += ioctl(sock, SIOCGIFNETMASK, &req);
	const uint32_t netmask=GetQuad(&req.ifr_addr,IsGood(retval));;
	CHECK(netmask)
	retval += ioctl(sock, SIOCGIFBRDADDR, &req);
	const uint32_t broadcast=GetQuad(&req.ifr_addr,IsGood(retval));
	CHECK(broadcast)
	retval += ioctl(sock, SIOCGIFMTU, &req);
	const int mtu = -1!=retval ? req.ifr_mtu:0;
	CHECK(mtu)
	retval += ioctl(sock, SIOCGIFHWADDR, &req);
	uint64_t hw_address = 0;
	unsigned char* hwaddr = (unsigned char *) &req.ifr_hwaddr.sa_data;
	memcpy(&hw_address,hwaddr,6);
	CHECK(hw_address)
	close(sock);
	//printf("Network::ioctl=%i address=%u netmask=%u broadcast=%u mtu=%i hw=%llu\n",retval,address,netmask,broadcast,mtu,hw_address);
	return !retval;
#endif
}

void Network::PrintStats()
{	printf("Network::PrintStats() interfaces=%u isChanged=%i\n",interfaces,isChanged);
	for(unsigned i=0;i<ifStats.size();i++)
	{	PrintIfStat(&ifStats[i]);
}	}

void Network::PrintStats(const char* ifname)
{	printf("Network::PrintStats() interfaces=%u isChanged=%i\n",interfaces,isChanged);
	IfStat* ifstat = GetIfStat(ifname);
	if(ifstat)
	{	PrintIfStat(ifstat);
	}
	else
	{	printf("Interface %s not found\n",ifname);
}	}

class Ip4Address
{	char data[4*4];
public:
	Ip4Address(uint32_t quad,char sep)
	{	unsigned char* p = reinterpret_cast<unsigned char*>(&quad);
SUPPRESS_SECURITY_WARNING 
		sprintf(data,"%u%c%u%c%u%c%u",unsigned(p[0]),sep,unsigned(p[1]),sep,unsigned(p[2]),sep,unsigned(p[3]));			
	}
	const char* Get() const
	{	return data;
	}
};

class HwAddress
{	char data[4*4];
public:
	HwAddress(uint64_t quad2,char sep)
	{	unsigned char* p = reinterpret_cast<unsigned char*>(&quad2);
SUPPRESS_SECURITY_WARNING
		sprintf(data,"%u%c%u%c%u%c%u%c%u%c%u",// only has 48 bits, ignore p[0] and p[1]
			unsigned(p[2]),sep,unsigned(p[3]),sep,unsigned(p[4]),sep,unsigned(p[5]),sep,unsigned(p[6]),sep,unsigned(p[7]));			
	}
	const char* Get() const
	{	return data;
	}
};


void Network::PrintIfStat(IfStat* ifstat)
{	if(!ifstat || !ifstat->address)
	{	return;
	}
	Ip4Address address(ifstat->address,'.');
	Ip4Address netmask(ifstat->netmask,':');
	Ip4Address broadcast(ifstat->broadcast,'.');
	Ip4Address gateway(ifstat->gateway,'.');
	HwAddress hw_address(ifstat->hw_address,':');
	printf("Interface: %s ip=%s mask=%s gateway=%s\n  broadcast=%s hw=%s mtu=%i\n",ifstat->ifname,address.Get(),netmask.Get(),gateway.Get(),broadcast.Get(),hw_address.Get(),ifstat->mtu);
	ifstat->in.Print(true);
	ifstat->out.Print(false);
}

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
SUPPRESS_SECURITY_WARNING
	scanf("%x.%x.%x.%x",ip,ip+1,ip+2,ip+3);

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
