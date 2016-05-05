// resource.h
// Copyright 2016 Robin.Rowe@cinepaint.org
// License MIT open source

#ifndef resource_h
#define resource_h

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

int getrlimit(int resource, struct rlimit *rlim);
int setrlimit(int resource, const struct rlimit *rlim);

#endif
