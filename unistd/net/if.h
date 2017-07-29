// if.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef if_h
#define if_h

#include <time.h>

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
{	SIOCGIFHWADDR,
	SIOCSIFADDR,
	SIOCSIFNETMASK,
	RTF_UP,
	RTF_GATEWAY,
	SIOCDELRT,
	SIOCGIFADDR,
	SIOCGIFNETMASK,
	SIOCGIFBRDADDR,
	SIOCGIFMEDIA
};

enum 
{	SIOCGIFCONF,
	OSIOCGIFCONF,	//	  Get interface	configuration.
	SIOCSIFNAME,	//	  Set the interface name. Caller must have appropriate privilege.  
	SIOCGIFCAP,
	SIOCGIFFIB,
	SIOCGIFFLAGS,
	SIOCGIFMETRIC,
	SIOCGIFMTU,
	SIOCGIFPHYS,	//  Get interface	capabilities, FIB, flags, metric, MTU, medium selection.  
	SIOCSIFCAP,		//  Enable or disable interface capabilities. Caller must have appropriate privilege.
	SIOCSIFFIB,		//  Sets interface FIB.  Caller must have appropriate privilege.  
	SIOCSIFFLAGS,	//  Change interface flags.  Caller must have appropriate privilege.  
	SIOCSIFMETRIC,
	SIOCSIFPHYS,	//  Change interface metric or medium. Caller must have appropriate	privilege.
	SIOCSIFMTU,		//  Change interface MTU. Caller must haveappropriate privilege.  
	SIOCADDMULTI,
	SIOCDELMULTI,	//  Add or delete permanent multicast group memberships on the interface.  Caller must have	appropriate privilege.  
	SIOCAIFADDR,
	SIOCDIFADDR,	//  The socket's protocol control routine is called to implement the requested action.
	OSIOCGIFADDR,
	OSIOCGIFDSTADDR,
	OSIOCGIFBRDADDR,
	OSIOCGIFNETMASK	//  The socket's protocol control routine is called to implement the requested action.
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


struct ifcapreq {
	char		ifcr_name[IFNAMSIZ];	/* if name, e.g. "en0" */
	uint64_t	ifcr_capabilities;	/* supported capabiliites */
	uint64_t	ifcr_capenable;		/* capabilities enabled */
};

struct ifaliasreq {
	char	ifra_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	struct	sockaddr ifra_addr;
	struct	sockaddr ifra_dstaddr;
#define	ifra_broadaddr	ifra_dstaddr
	struct	sockaddr ifra_mask;
};


/*
 * Structure defining statistics and other data kept regarding a network
 * interface.
 */
struct if_data {
	/* generic interface information */
	u_char	ifi_type;		/* ethernet, tokenring, etc. */
	u_char	ifi_addrlen;		/* media address length */
	u_char	ifi_hdrlen;		/* media header length */
	int	ifi_link_state;		/* current link state */
	uint64_t ifi_mtu;		/* maximum transmission unit */
	uint64_t ifi_metric;		/* routing metric (external only) */
	uint64_t ifi_baudrate;		/* linespeed */
	/* volatile statistics */
	uint64_t ifi_ipackets;		/* packets received on interface */
	uint64_t ifi_ierrors;		/* input errors on interface */
	uint64_t ifi_opackets;		/* packets sent on interface */
	uint64_t ifi_oerrors;		/* output errors on interface */
	uint64_t ifi_collisions;	/* collisions on csma interfaces */
	uint64_t ifi_ibytes;		/* total number of octets received */
	uint64_t ifi_obytes;		/* total number of octets sent */
	uint64_t ifi_imcasts;		/* packets received via multicast */
	uint64_t ifi_omcasts;		/* packets sent via multicast */
	uint64_t ifi_iqdrops;		/* dropped on input, this interface */
	uint64_t ifi_noproto;		/* destined for unsupported protocol */
	struct	timespec ifi_lastchange;/* last operational state change */
};

struct ifdatareq {
	char	ifdr_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	struct	if_data ifdr_data;
};

struct ifmediareq {
	char ifm_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	int	ifm_current;			/* current media options */
	int	ifm_mask;			/* don't care mask */
	int	ifm_status;			/* media status */
	int	ifm_active;			/* active options */
	int	ifm_count;			/* # entries in ifm_ulist
						   array */
	int	*ifm_ulist;			/* media words */
};


struct  ifdrv {
	char		ifd_name[IFNAMSIZ];	/* if name, e.g. "en0" */
	unsigned long	ifd_cmd;
	size_t		ifd_len;
	void		*ifd_data;
};

enum
{	IFM_ACTIVE,
	IFM_AVALID,
	IFM_STATUS_VALID,
	IFM_ETHER,
	IFM_FDDI,
	IFM_TOKEN,
	IFM_IEEE80211,
	IFM_CARP
};

#define IFFBITS "0"
#define IFCAPBITS "0"

 struct ifmedia_status_description {
int     ifms_type;
int     ifms_valid;
int     ifms_bit;
const char *ifms_string[2];
};

#define IFM_STATUS_DESC(ifms, bit)                                      \
	(ifms)->ifms_string[((ifms)->ifms_bit & (bit)) ? 1 : 0]

#define IFM_STATUS_DESCRIPTIONS {                                       \
	{ IFM_ETHER,            IFM_AVALID,     IFM_ACTIVE,             \
	{ "no carrier", "active" } },                                 \
	{ IFM_FDDI,             IFM_AVALID,     IFM_ACTIVE,             \
	{ "no ring", "inserted" } },                                  \
	{ IFM_TOKEN,            IFM_AVALID,     IFM_ACTIVE,             \
	{ "no ring", "inserted" } },                                  \
	{IFM_IEEE80211,        IFM_AVALID,     IFM_ACTIVE,             \
	{ "no network", "active" } },                                 \
	{ IFM_CARP,             IFM_AVALID,     IFM_ACTIVE,             \
	{ "backup", "master" } },                                   \
	{ 0,                    0,              0,                      \
	{ NULL, NULL } },                                             \
}

enum
{	IFM_NMASK,
	IFM_TMASK,
	IFM_ISHIFT,
	IFM_OMASK,
	IFM_GMASK
};

#define IFM_INST_MAX    IFM_INST(IFM_IMASK)

#define IFM_TYPE(x)     ((x) & IFM_NMASK)
#define IFM_SUBTYPE(x)  ((x) & IFM_TMASK)
#define IFM_INST(x)     (((x) & IFM_IMASK) >> IFM_ISHIFT)
#define IFM_OPTIONS(x)  ((x) & (IFM_OMASK | IFM_GMASK))
#define IFM_MODE(x)     ((x) & IFM_MMASK)

#define IFM_INST_MAX    IFM_INST(IFM_IMASK)
#define IFM_INST_ANY    ((u_int) -1)

enum
{	HN_AUTOSCALE, 
	HN_DECIMAL,
	HN_NOSPACE
};

inline
void humanize_number(char* buf, int len,int64_t bytes,const char*,int flags,int)
{   (void)buf;
    (void)len;
    (void)bytes;
    (void)flags;
    STUB(humanize_number);
}

enum
{	SIOCZIFDATA,
	SIOCGIFDATA
};

struct afswtch
{	int af_status;
	char af_name[30];
	int af_af;
};

#endif
