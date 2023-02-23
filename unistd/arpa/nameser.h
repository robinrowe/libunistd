// arpa/nameser.h
// Copyright (c) 2022/10/19 Noah Treuhaft <noah@brimdata.io>
// License open source MIT

#ifndef arpa_nameser_h
#define arpa_nameser_h

#include <stdlib.h>

#define HFIXEDSZ 12

#define NS_IN6ADDRSZ 16
#define NS_INADDRSZ  4
#define NS_INT16SZ   2
#define NS_MAXDNAME  1025

#define T_A    1
#define T_PTR  12
#define T_TXT  16
#define T_AAAA 28

#define ns_o_query 0

#define ns_r_noerror  0
#define ns_r_formerr  1
#define ns_r_servfail 2
#define ns_r_nxdomain 3
#define ns_r_notimpl  4
#define ns_r_refused  5

typedef void ns_rr;

#define ns_rr_type(rr)	0
#define ns_rr_ttl(rr)	0
#define ns_rr_rdlen(rr)	0
#define ns_rr_rdata(rr)	0

typedef void ns_msg;

#define ns_msg_id(handle) 0
#define ns_msg_count(handle, section) 0
#define ns_msg_getflag(handle, flag) 0

typedef struct
{	int id;
} HEADER;

#define ns_initparse(a, b, c) (-1)
#define ns_parserr(a, b, c, d) (-1)

#endif
