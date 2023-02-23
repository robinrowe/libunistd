// net/if_arp.h
// Copyright (c) 2022/10/19 Noah Treuhaft <noah@brimdata.io>
// License open source MIT

#ifndef net_if_arp_h
#define net_if_arp_h

#include <stdint.h>

#define ARPHRD_ETHER 1

// Modification: Memory packing is needed for cross-platform buffer evaluation.
#pragma pack(push, 1)
struct arphdr
{	uint16_t ar_hrd;
	uint16_t ar_pro;
	uint8_t  ar_hln;
	uint8_t  ar_pln;
	uint16_t ar_op;
};
#pragma pack(pop)

#endif
