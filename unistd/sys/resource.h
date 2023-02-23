// resource.h
// Copyright 2016 Robin.Rowe@cinepaint.org
// License MIT open source

#ifndef resource_h
#define resource_h

#include "../../portable/stub.h"
#include <unistd.h>
#include <psapi.h>

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

static void
usage_to_timeval(FILETIME* ft, struct timeval* tv)
{	ULARGE_INTEGER time;
	time.LowPart = ft->dwLowDateTime;
	time.HighPart = ft->dwHighDateTime;
	tv->tv_sec = time.QuadPart / 10000000;
	tv->tv_usec = (time.QuadPart % 10000000) / 10;
}

#define RLIMIT_NOFILE 0

#define RLIM_INFINITY (~0ULL)

typedef unsigned long long rlim_t;

struct rlimit
{	rlim_t rlim_cur;
	rlim_t rlim_max;
};

int getrlimit(int resource, struct rlimit* rlp);
int setrlimit(int resource, const struct rlimit* rlp);

#define RUSAGE_SELF	 0
#define RUSAGE_CHILDREN (-1)
#define RUSAGE_THREAD   1

struct rusage
{	struct timeval ru_utime;
	struct timeval ru_stime;
	long ru_maxrss;
	long ru_ixrss;
	long ru_idrss;
	long ru_isrss;
	long ru_minflt;
	long ru_majflt;
	long ru_nswap;
	long ru_inblock;
	long ru_oublock;
	long ru_msgsnd;
	long ru_msgrcv;
	long ru_nsignals;
	long ru_nvcsw;
	long ru_nivcsw;
};

inline
int getrusage(int who, struct rusage* usage)
{	FILETIME creation_time, exit_time, kernel_time, user_time;
	PROCESS_MEMORY_COUNTERS_EX pmc;
	memset(usage, 0, sizeof(struct rusage));
	if (who == RUSAGE_SELF)
	{	if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,
							 &kernel_time, &user_time))
		{	return -1;
		}
		if (!GetProcessMemoryInfo(GetCurrentProcess(),
								  reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc),
								  sizeof(PROCESS_MEMORY_COUNTERS)))
		{	return -1;
		}
		usage_to_timeval(&kernel_time, &usage->ru_stime);
		usage_to_timeval(&user_time, &usage->ru_utime);
		usage->ru_majflt = pmc.PageFaultCount;
		usage->ru_maxrss = pmc.WorkingSetSize / 1024;
		usage->ru_ixrss = pmc.PrivateUsage / 1024;
		return 0;
	}
	else if (who == RUSAGE_THREAD)
	{	if (!GetThreadTimes(GetCurrentThread(), &creation_time, &exit_time,
							&kernel_time, &user_time))
		{	return -1;
		}
		usage_to_timeval(&kernel_time, &usage->ru_stime);
		usage_to_timeval(&user_time, &usage->ru_utime);
		return 0;
	}
	else
	{	return -1;
	}
}

inline
int getrlimit(int resource, struct rlimit* rlim)
{	memset(rlim, 0, sizeof(*rlim));
	rlim->rlim_max = RLIM_INFINITY;
	STUB_MSG(getrlimit);
	return 0;
}

inline
int setrlimit(int resource, const struct rlimit* rlim)
{	STUB_NEG(setrlimit);
}

#ifdef __cplusplus
}
#endif

#endif
