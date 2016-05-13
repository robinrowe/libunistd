// ip.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef ip_h
#define ip_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

struct ip
{	uint8_t ip_hdr_len;
	uint8_t ip_version;
	uint8_t ip_tos;
	uint16_t ip_len;
	uint16_t ip_id;
	uint16_t ip_off;
	uint8_t ip_ttl;
	uint8_t ip_proto;
	uint16_t ip_chk;
	uint32_t ip_src;
	uint32_t ip_dst;
};

#ifdef __cplusplus
}
#endif

#endif
