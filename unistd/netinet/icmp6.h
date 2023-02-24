// netinet/icmp6.h
// Copyright (c) 2022/10/19 Noah Treuhaft <noah@brimdata.io>
// License open source MIT

#ifndef netinet_icmp6_h
#define netinet_icmp6_h

#include <stdint.h>

#define ICMP6_DST_UNREACH          1
#define ICMP6_PACKET_TOO_BIG       2
#define ICMP6_TIME_EXCEEDED        3
#define ICMP6_PARAM_PROB           4

#define ICMP6_ECHO_REQUEST       128
#define ICMP6_ECHO_REPLY         129

#define MLD_LISTENER_QUERY       130
#define MLD_LISTENER_REPORT      131
#define MLD_LISTENER_REDUCTION   132

#define ND_ROUTER_SOLICIT        133
#define ND_ROUTER_ADVERT         134
#define ND_NEIGHBOR_SOLICIT      135
#define ND_NEIGHBOR_ADVERT       136
#define ND_REDIRECT              137

#define ICMP6_ROUTER_RENUMBERING 138

// Modification: Memory packing is needed for cross-platform buffer evaluation.
#pragma pack(push, 1)
struct icmp6_hdr
{	uint8_t  icmp6_type;
	uint8_t  icmp6_code;
	uint16_t icmp6_cksum;
	union
	{	uint32_t icmp6_un_data32[1];
		uint16_t icmp6_un_data16[2];
		uint8_t  icmp6_un_data8[4];
	} icmp6_dataun;
};
#pragma pack(pop)

#endif
