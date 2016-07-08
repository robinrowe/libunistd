// if.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef if_h
#define if_h

#define IFNAMSIZ 32

enum ifr_flags
{	//IFF_UP,				// Interface is running.
	//IFF_BROADCAST,     // Valid broadcast address set.
	IFF_DEBUG,         // Internal debugging flag.
	//IFF_LOOPBACK,      // Interface is a loopback interface.
	IFF_POINTOPOINT,   // Interface is a point-to-point link.
	IFF_RUNNING,       // Resources allocated.
	IFF_NOARP,         // No arp protocol, L2 destination address not set.
	IFF_PROMISC,       // Interface is in promiscuous mode.
	IFF_NOTRAILERS,    // Avoid use of trailers.
	IFF_ALLMULTI,      // Receive all multicast packets.
	IFF_MASTER,        // Master of a load balancing bundle.
	IFF_SLAVE,         // Slave of a load balancing bundle.
	//IFF_MULTICAST,     // Supports multicast
	IFF_PORTSEL,       // Is able to select media type via ifmap.
	IFF_AUTOMEDIA,     // Auto media selection active.
	IFF_DYNAMIC,       // The addresses are lost when the interface goes down.
	IFF_LOWER_UP,      // Driver signals L1 up (since Linux 2.6.17)
	IFF_DORMANT,       // Driver signals dormant (since Linux 2.6.17)
	IFF_ECHO           // Echo sent packets (since Linux 2.6.25)
};

enum
{	SIOCGIFFLAGS,
	SIOCGIFHWADDR,
	SIOCSIFADDR,
	SIOCSIFNETMASK,
	RTF_UP,
	RTF_GATEWAY,
	SIOCDELRT
};

struct ifmap {
    unsigned long   mem_start;
    unsigned long   mem_end;
    unsigned short  base_addr;
    unsigned char   irq;
    unsigned char   dma;
    unsigned char   port;
};

struct ifreq {
    char ifr_name[IFNAMSIZ]; /* Interface name */
    union {
        struct sockaddr ifr_addr;
        struct sockaddr ifr_dstaddr;
        struct sockaddr ifr_broadaddr;
        struct sockaddr ifr_netmask;
        struct sockaddr ifr_hwaddr;
        short           ifr_flags;
        int             ifr_ifindex;
        int             ifr_metric;
        int             ifr_mtu;
        struct ifmap    ifr_map;
        char            ifr_slave[IFNAMSIZ];
        char            ifr_newname[IFNAMSIZ];
        char           *ifr_data;
    };
};

struct rtentry 
{
    unsigned long   rt_hash;        /* hash key for lookups         */
    struct sockaddr rt_dst;         /* target address               */
    struct sockaddr rt_gateway;     /* gateway addr (RTF_GATEWAY)   */
    struct sockaddr rt_genmask;     /* target network mask (IP)     */
    short           rt_flags;
    short           rt_refcnt;
    unsigned long   rt_use;
    struct ifnet    *rt_ifp;
    short           rt_metric;      /* +1 for binary compatibility! */
    char            *rt_dev;        /* forcing the device at add    */
    unsigned long   rt_mss;         /* per route MTU/Window         */
    unsigned long   rt_window;      /* Window clamping              */
    unsigned short  rt_irtt;        /* Initial RTT                  */
};

#endif
