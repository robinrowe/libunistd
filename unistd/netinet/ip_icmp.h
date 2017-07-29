// ip_icmp.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef ip_icmp_h
#define ip_icmp_h

#include <stdint.h>
#include "ip.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

struct icmp_ra_addr 
{	uint32_t ira_addr;
	uint32_t ira_preference;
};

#define	icmp_pptr	icmp_hun.ih_pptr
#define	icmp_gwaddr	icmp_hun.ih_gwaddr
#define	icmp_id		icmp_hun.ih_idseq.icd_id
#define	icmp_seq	icmp_hun.ih_idseq.icd_seq
#define	icmp_void	icmp_hun.ih_void
#define	icmp_pmvoid	icmp_hun.ih_pmtu.ipm_void
#define	icmp_nextmtu	icmp_hun.ih_pmtu.ipm_nextmtu
#define	icmp_num_addrs	icmp_hun.ih_rtradv.irt_num_addrs
#define	icmp_wpa	icmp_hun.ih_rtradv.irt_wpa
#define	icmp_lifetime	icmp_hun.ih_rtradv.irt_lifetime
#define	icmp_otime	icmp_dun.id_ts.its_otime
#define	icmp_rtime	icmp_dun.id_ts.its_rtime
#define	icmp_ttime	icmp_dun.id_ts.its_ttime
#define	icmp_ip		icmp_dun.id_ip.idi_ip
#define	icmp_radv	icmp_dun.id_radv
#define	icmp_mask	icmp_dun.id_mask
#define	icmp_data	icmp_dun.id_data

struct icmp 
{	u_char	icmp_type;
	u_char	icmp_code;		
	u_short	icmp_cksum;		
	union 
	{	u_char ih_pptr;
		struct in_addr ih_gwaddr;
		struct ih_idseq 
		{	short	icd_id;
			short	icd_seq;
		} ih_idseq;
	int ih_void;
	struct ih_pmtu 
	{	short ipm_void;
		short ipm_nextmtu;
	} ih_pmtu;
	struct ih_rtradv 
	{	u_char irt_num_addrs;
		u_char irt_wpa;
		uint16_t irt_lifetime;
		} ih_rtradv;
	} icmp_hun;
	union 
	{	struct id_ts 
		{	time_t its_otime;
			time_t its_rtime;
			time_t its_ttime;
		} id_ts;
		struct id_ip  
		{	struct ip idi_ip;
		} id_ip;
		struct icmp_ra_addr id_radv;
		uint32_t id_mask;
		char	id_data[1];
	} icmp_dun;
};

#define	ICMP_MINLEN	8				
#define	ICMP_TSLEN	(8 + 3 * sizeof (time_t))	
#define	ICMP_MASKLEN	12				
#define	ICMP_ADVLENMIN	(8 + sizeof (struct ip) + 8)	
#define	ICMP_ADVLEN(p)	(8 + ((p)->icmp_ip.ip_hl << 2) + 8)

enum
{	ICMP_UNREACH_NET,
	ICMP_UNREACH_HOST,	
	ICMP_UNREACH_PROTOCOL,	
	ICMP_UNREACH_PORT,
	ICMP_UNREACH_NEEDFRAG,	
	ICMP_UNREACH_SRCFAIL,
	ICMP_UNREACH_NET_UNKNOWN, 
	ICMP_UNREACH_HOST_UNKNOWN,
	ICMP_UNREACH_ISOLATED,
	ICMP_UNREACH_NET_PROHIB,	
	ICMP_UNREACH_HOST_PROHIB, 
	ICMP_UNREACH_TOSNET,
	ICMP_UNREACH_TOSHOST,
	ICMP_UNREACH_FILTER_PROHIB, 
	ICMP_UNREACH_HOST_PRECEDENCE, 
	ICMP_UNREACH_PRECEDENCE_CUTOFF 
};

enum
{	ICMP_REDIRECT_NET,
	ICMP_REDIRECT_HOST,
	ICMP_REDIRECT_TOSNET,
	ICMP_REDIRECT_TOSHOST
};

#define	ICMP_ECHOREPLY		0	
#define	ICMP_UNREACH		3	
#define	ICMP_SOURCEQUENCH	4	
#define	ICMP_REDIRECT		5	
#define	ICMP_ECHO		8	
#define	ICMP_ROUTERADVERT	9	
#define	ICMP_ROUTERSOLICIT	10		
#define	ICMP_TIMXCEED		11		
#define	ICMP_TIMXCEED_INTRANS	0	
#define	ICMP_TIMXCEED_REASS	1		
#define	ICMP_PARAMPROB		12	
#define	ICMP_PARAMPROB_ERRATPTR 0	
#define	ICMP_PARAMPROB_OPTABSENT 1	
#define	ICMP_PARAMPROB_LENGTH 2		
#define	ICMP_TSTAMP		13		
#define	ICMP_TSTAMPREPLY	14		
#define	ICMP_IREQ		15		
#define	ICMP_IREQREPLY		16		
#define	ICMP_MASKREQ		17		
#define	ICMP_MASKREPLY		18		
#define	ICMP_MAXTYPE		18

#define	ICMP_INFOTYPE(type) \
	((type) == ICMP_ECHOREPLY || (type) == ICMP_ECHO || \
	(type) == ICMP_ROUTERADVERT || (type) == ICMP_ROUTERSOLICIT || \
	(type) == ICMP_TSTAMP || (type) == ICMP_TSTAMPREPLY || \
	(type) == ICMP_IREQ || (type) == ICMP_IREQREPLY || \
	(type) == ICMP_MASKREQ || (type) == ICMP_MASKREPLY)


#ifdef __cplusplus
}
#endif

#endif
