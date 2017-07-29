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
extern "C" {
#else
#define inline __inline
#endif

struct ip
{	unsigned int ip_hl:4;		/* header length */
	unsigned int ip_v:4;		/* version */
	uint8_t ip_tos;			/* type of service */
	u_short ip_len;			/* total length */
	u_short ip_id;			/* identification */
	u_short ip_off;			/* fragment offset field */
	#define	IP_RF 0x8000			/* reserved fragment flag */
	#define	IP_DF 0x4000			/* dont fragment flag */
	#define	IP_MF 0x2000			/* more fragments flag */
	#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	uint8_t ip_ttl;			/* time to live */
	uint8_t ip_p;			/* protocol */
	u_short ip_sum;			/* checksum */
	struct in_addr ip_src, ip_dst;	/* source and dest address */
};

#ifdef __cplusplus
}
#endif

#endif
