// resource.h
// Copyright 2016 Robin.Rowe@cinepaint.org
// License MIT open source

#ifndef resource_h
#define resource_h

#include "../../portable/stub.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

enum
{	SCHED_OTHER,
	SCHED_BATCH,
	SCHED_IDLE,
	SCHED_FIFO,
	SCHED_RR,
	RLIMIT_RTPRIO
};

typedef int rlim_t;

struct rlimit 
{	rlim_t rlim_cur;  
	rlim_t rlim_max;  
};

inline
int getrlimit(int resource, struct rlimit *rlim)
{	memset(rlim,0,sizeof(*rlim));
	STUB_MSG(getrlimit);	
	return 0;
}

inline
int setrlimit(int resource, const struct rlimit *rlim)
STUB0(setrlimit)

#ifdef __cplusplus
}
#endif

#endif
