// Timestamp.h
// Created by Robin Rowe on 12/17/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org

#ifndef Timestamp_h
#define Timestamp_h

#include <memory.h>

namespace portable {

// At 11:01:20 UTC on 13 July 2012, the Unix time number reached 0x50000000 (1,342,177,280 seconds).

#ifdef _WIN32
typedef __time64_t time64_t;
#undef min
#else
typedef long long time64_t;
#endif

typedef time64_t Timestamp_t;
typedef time64_t Micros_t;
typedef time64_t Microseconds_t;

static const bool isNow=true;

struct Epoch
{	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
	int ms;
	int us;
	Epoch()
	:	year(0)
	,	month(0)
	,	day(0)
	,	hour(0)
	,	min(0)
	,	sec(0)
	,	ms(0)
	,	us(0)
	{}
};

class Timestamp
{	time64_t microseconds;
	const static int bufsize=28;
	char buffer[bufsize];
	static time64_t currentTimeMicros();
	const char* toTimeString(const char* timeFormat,bool isMicros);
public:
	Timestamp(Micros_t epoch)
	:	microseconds(epoch)
	{	buffer[0]=0;
	}
	Timestamp(bool isSetNow=true)
	:	microseconds(0)
	{	buffer[0]=0;
		if(isSetNow)
		{	SetNow();
	}	}
	Timestamp& operator=(const char* cstring)
	{	StringToEpoch(cstring);
		return *this;
	}
	Timestamp& operator+=(Micros_t offset)
	{	microseconds+=offset;
		return *this;
	}
	bool operator!() const
	{	return microseconds<=0;
	}
	bool operator>(const Timestamp& r) const
	{	return microseconds>r.microseconds;
	}
	bool operator>=(const Timestamp& r) const
	{	return microseconds>=r.microseconds;
	}
	bool operator<(const Timestamp& r) const
	{	return microseconds<r.microseconds;
	}
	bool operator<=(const Timestamp& r) const
	{	return microseconds<=r.microseconds;
	}
	bool operator==(const Timestamp& r)
	{	return microseconds==r.microseconds;
	}
	const char* toSqlString() 
	{	// SQL FORMAT: 2003-01-02 10:30:00.000123 
		return toTimeString("%Y-%m-%d %H:%M:%S",true);
	}
	const char* toCtimeString() 
	{	// Tue Aug 05 12:57:21 2014
		return toTimeString("%a %b %d %H:%M:%S %Y",false);
	}
	const char* toEmailString() 
	{	// Tue Aug 05 12:57:21 2014
		return toTimeString("%a, %d %b %Y %T",false);
	}
	const char* toString() 
	{	// Tue Aug 05 12:57:21 2014
		return toTimeString("%H:%M:%S",true);
	}
	Micros_t toMicros() const
	{	return microseconds;
	}
	Micros_t SetNow()
	{	microseconds=currentTimeMicros();
		return microseconds;
	}
	void clear()
	{	microseconds=0;
	}
	static Micros_t GetNow() 
	{	return currentTimeMicros();
	}
	Micros_t GetTimeLeftMicros(const Timestamp& sooner) const
	{	if(microseconds<=0 || sooner.microseconds<=0)
		{	return 0;
		}
		if(sooner.microseconds>microseconds)
		{	return 0;
		}
		const long long whatsLeft=microseconds-sooner.microseconds;
		return whatsLeft;
	}
	Micros_t GetTimeLeftMicros() const
	{	Timestamp now(isNow);
		return GetTimeLeftMicros(now);
	}
	Micros_t operator-(const Timestamp& sooner) const
	{	return GetTimeLeftMicros(sooner);
	}
	bool IsPast() const
	{	if(!microseconds)
		{	return false;// 'never' will never be in the past
		}
		Timestamp now(isNow);
		return now.microseconds>=microseconds;
	}
	bool IsFuture() const
	{	return !IsPast();
	}
	bool IsExpired() const
	{	return IsPast();// 'never' will never expire
	}
	void StringToEpoch(const char* cstring);
};

}

#endif
