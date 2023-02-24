// netinet/tcp.h
// Copyright (c) 2022/10/19 Noah Treuhaft <noah@brimdata.io>
// License open source MIT

#ifndef netinet_tcp_h
#define netinet_tcp_h

#include <stdint.h>

#define TCPOPT_EOL            0
#define TCPOPT_NOP            1
#define TCPOPT_MAXSEG         2
#define TCPOPT_WINDOW         3
#define TCPOPT_SACK_PERMITTED 4
#define TCPOPT_TIMESTAMP      8

#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20

// Modification: Memory packing is needed for cross-platform buffer evaluation.
#pragma pack(push, 1)
struct tcphdr
{	uint16_t th_sport;
	uint16_t th_dport;
	uint32_t th_seq;
	uint32_t th_ack;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	uint8_t  th_x2:4;
	uint8_t  th_off:4;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	uint8_t  th_off:4;
	uint8_t  th_x2:4;
#else
#error Unknown byte order.
#endif
	uint8_t  th_flags;
	uint16_t th_win;
	uint16_t th_sum;
	uint16_t th_urp;
};
#pragma pack(pop)

#endif
