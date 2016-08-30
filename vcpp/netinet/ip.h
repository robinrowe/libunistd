// ip.h
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef ip_h
#define ip_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

#if 0
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
#endif

struct in_addr 
{	uint32_t       s_addr;     /* address in network byte order */
};

struct ip
{	unsigned int ip_hl:4;		/* header length */
	unsigned int ip_v:4;		/* version */
	u_int8_t ip_tos;			/* type of service */
	u_short ip_len;			/* total length */
	u_short ip_id;			/* identification */
	u_short ip_off;			/* fragment offset field */
	#define	IP_RF 0x8000			/* reserved fragment flag */
	#define	IP_DF 0x4000			/* dont fragment flag */
	#define	IP_MF 0x2000			/* more fragments flag */
	#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	u_int8_t ip_ttl;			/* time to live */
	u_int8_t ip_p;			/* protocol */
	u_short ip_sum;			/* checksum */
	struct in_addr ip_src, ip_dst;	/* source and dest address */
};

#ifdef __cplusplus
}
#endif

#endif
