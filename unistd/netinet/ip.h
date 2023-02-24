// ip.h
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef ip_h
#define ip_h

#ifdef _WIN32
#include "../unistd.h"
#else
#include <unistd.h>
#endif
#include <inaddr.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#else
#define inline __inline
#endif

// Modification: Memory packing is needed for cross-platform buffer evaluation.
#pragma pack(push, 1)
struct ip
{	// Use uint8_t instead of unsigned for ip_hl and ip_v so they
	// occupy one byte instead of four on Windows.  (MSVC allocates
	// space sufficient for the bit field's type rather than its
	// width.  This corresponds to GCC's -mms-bitfields option,
	// which is enabled by default on Windows.  On other platforms,
	// GCC allocates space sufficient for the bit field's width
	// regardless of its type.)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	uint8_t ip_hl : 4;
	uint8_t ip_v : 4;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	uint8_t ip_v : 4;  /* header length */
	uint8_t ip_hl : 4; /* version */
#else
#error Unknown byte order.
#endif
	uint8_t ip_tos;				   /* type of service */
	uint16_t ip_len;			   /* total length */
	uint16_t ip_id;				   /* identification */
	uint16_t ip_off;			   /* fragment offset field */
#define IP_RF 0x8000				   /* reserved fragment flag */
#define IP_DF 0x4000				   /* dont fragment flag */
#define IP_MF 0x2000				   /* more fragments flag */
#define IP_OFFMASK 0x1fff			   /* mask for fragmenting bits */
	uint8_t ip_ttl;				   /* time to live */
	uint8_t ip_p;				   /* protocol */
	uint16_t ip_sum;			   /* checksum */
	struct in_addr ip_src, ip_dst; /* source and dest address */
};
#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
