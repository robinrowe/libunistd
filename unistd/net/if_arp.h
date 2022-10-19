// net/if_arp.h
// Provided by brimdata on 2022/10/19
// Copyright (c) 2019 Robin.Rowe@CinePaint.org
// License open source MIT

#pragma once

#include <stdint.h>

#define ARPHRD_ETHER 1

// Modification: Memory packing is needed for cross-platform buffer evaluation.
#pragma pack(push, 1)
struct arphdr {
	uint16_t ar_hrd;
	uint16_t ar_pro;
	uint8_t  ar_hln;
	uint8_t  ar_pln;
	uint16_t ar_op;
};
#pragma pack(pop)