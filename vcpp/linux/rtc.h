// rtc.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef rtc_h
#define rtc_h

#include "sys/ioctl.h"
#include "../portable/stub.h"

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

// The following ioctl(2) requests are defined on file descriptors connected to RTC devices: RTC_RD_TIME
// Returns this RTC's time in the following structure:

struct rtc_time 
{	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;     /* unused */
	int tm_yday;     /* unused */
	int tm_isdst;    /* unused */
};

enum
{	RTC_SET_TIME,
	RTC_ALM_READ, 
	RTC_ALM_SET,
	RTC_IRQP_READ, 
	RTC_IRQP_SET,
	RTC_AIE_ON, 
	RTC_AIE_OFF,
	RTC_UIE_ON, 
	RTC_UIE_OFF,
	RTC_PIE_ON, 
	RTC_PIE_OFF,
	RTC_EPOCH_READ, 
	RTC_EPOCH_SET,
	RTC_WKALM_RD, 
	RTC_WKALM_SET
};

struct rtc_wkalrm 
{	unsigned char enabled;
	unsigned char pending;
	struct rtc_time time;
};

#ifdef __cplusplus
}
#endif

#endif
