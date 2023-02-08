// netinet/udp.h
// Provided by brimdata on 2022/10/19
// Copyright (c) 2019 Robin.Rowe@CinePaint.org
// License open source MIT

#pragma once

#include <stdint.h>

// Modification: Memory packing is needed for cross-platform buffer evaluation.
#pragma pack(push, 1)
struct udphdr {
	uint16_t uh_sport;
	uint16_t uh_dport;
	uint16_t uh_ulen;
	uint16_t uh_sum;
};
#pragma pack(pop)