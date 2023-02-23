// netinet/udp.h
// Copyright (c) 2022/10/19 Noah Treuhaft <noah@brimdata.io>
// License open source MIT

#ifndef netinet_udp_h
#define netinet_udp_h

#include <stdint.h>

// Modification: Memory packing is needed for cross-platform buffer evaluation.
#pragma pack(push, 1)
struct udphdr
{	uint16_t uh_sport;
	uint16_t uh_dport;
	uint16_t uh_ulen;
	uint16_t uh_sum;
};
#pragma pack(pop)

#endif
