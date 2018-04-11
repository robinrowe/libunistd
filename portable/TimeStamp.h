// TimeStamp.h
// Created by Robin Rowe on 12/17/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org

#ifndef TimeStamp_h
#define TimeStamp_h

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <time.h>
#else
#include <sys/time.h>
#endif
#include <time.h>
#include <stdio.h>

namespace portable {

// At 11:01:20 UTC on 13 July 2012, the Unix time number reached 0x50000000 (1,342,177,280 seconds).

#ifdef WIN32
typedef __time64_t time64_t;
#undef min
#else
typedef long long time64_t;
#endif

typedef time64_t Timestamp;
typedef time64_t Micros;
typedef time64_t Microseconds;

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

class TimeStamp
{	time64_t microseconds;
	const static int bufsize=28;
	char buffer[bufsize];
#ifdef WIN32
// accurate to 0.1 microseconds
	static long long WindowsTimeMicroseconds(FILETIME& ft) 
	{	unsigned long long tt = ft.dwHighDateTime;
		tt <<=32;
		tt |= ft.dwLowDateTime;
		tt /=10;
		tt -= 11644473600000000ULL;
	//	tt /=1000; //micro to mille
		if(tt<0)
		{	tt=0;
		}
		return tt;
	}
#else
// accurate to better than 1 microsecond
	static long long LinuxTimeMicroseconds()
	{	struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec * factor + tv.tv_usec;
	}
#endif 
	static time64_t currentTimeMicros()
	{	
#ifdef WIN32
		FILETIME ft;//64-bit value of 100-nanosecond intervals since January 1, 1601 UTC
		GetSystemTimeAsFileTime(&ft);
		const time64_t t=WindowsTimeMicroseconds(ft);
#else
		const time64_t t=LinuxTimeMicroseconds();
#endif
		return t;
	}
	const char* toTimeString(const char* timeFormat,bool isMicros)
	{	tm t;
#ifdef WIN32
        const errno_t err = _localtime64_s(&t,&microseconds); 
		const bool isOk=!err;
		if(!isOk)
        {	_strerror_s(buffer,bufsize,"");
			return buffer;
        }
#else
		tm* ok=localtime_r(const time_t *timep, struct tm *result);
		const bool isOk=0!=ok;
		if(!isOk)
        {	memcpy(buffer,"Error",6);
			return buffer;
        }
#endif
		strftime(buffer,bufsize-1,timeFormat,&t);
		if(isMicros)
		{	char* p=buffer+strlen(buffer);
			const int micros = int(microseconds%1000000);
#pragma warning( disable : 4996 )
			sprintf(p,".%d",micros);
#pragma warning( default : 4996 )
		}
		buffer[bufsize-1]=0;
		return buffer;
	}
public:
	TimeStamp(Micros epoch)
	:	microseconds(epoch)
	{	buffer[0]=0;
	}
	TimeStamp(bool isSetNow=true)
	:	microseconds(0)
	{	buffer[0]=0;
		if(isSetNow)
		{	SetNow();
	}	}
	TimeStamp& operator=(const char* cstring)
	{	StringToEpoch(cstring);
		return *this;
	}
	TimeStamp& operator+=(Micros offset)
	{	microseconds+=offset;
		return *this;
	}
	bool operator!() const
	{	return microseconds<=0;
	}
	bool operator>(const TimeStamp& r) const
	{	return microseconds>r.microseconds;
	}
	bool operator>=(const TimeStamp& r) const
	{	return microseconds>=r.microseconds;
	}
	bool operator<(const TimeStamp& r) const
	{	return microseconds<r.microseconds;
	}
	bool operator<=(const TimeStamp& r) const
	{	return microseconds<=r.microseconds;
	}
	bool operator==(const TimeStamp& r)
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
		return toTimeString("%a, %d %b %Y %T ",false);
	}
	Micros toMicros() const
	{	return microseconds;
	}
	Micros SetNow()
	{	microseconds=currentTimeMicros();
		return microseconds;
	}
	void clear()
	{	microseconds=0;
	}
	static Micros GetNow() 
	{	return currentTimeMicros();
	}
	const char* toString() 
	{	return toSqlString();
	}
	Micros GetTimeLeftMicros(const TimeStamp& sooner) const
	{	if(microseconds<=0 || sooner.microseconds<=0)
		{	return 0;
		}
		if(sooner.microseconds>microseconds)
		{	return 0;
		}
		const long long whatsLeft=microseconds-sooner.microseconds;
		return whatsLeft;
	}
	Micros GetTimeLeftMicros() const
	{	TimeStamp now(isNow);
		return GetTimeLeftMicros(now);
	}
	Micros operator-(const TimeStamp& sooner) const
	{	return GetTimeLeftMicros(sooner);
	}
	bool IsPast() const
	{	if(!microseconds)
		{	return false;// 'never' will never be in the past
		}
		TimeStamp now(isNow);
		return now.microseconds>=microseconds;
	}
	bool IsFuture() const
	{	return !IsPast();
	}
	bool IsExpired() const
	{	return IsPast();// 'never' will never expire
	}
	void StringToEpoch(const char* cstring)
#ifdef WIN32
	{	if(!cstring)
		{	microseconds=0;
			return;
		}
		SYSTEMTIME localTime;
		memset(&localTime,0,sizeof(localTime));
	//"2012-08-12 12:00:00"
		Epoch e;
		sscanf_s(cstring,"%d%*c%d%*c%d %d:%d:%d",&(e.year),&(e.month),&(e.day),&(e.hour),&(e.min),&(e.sec));
		localTime.wYear=(WORD) e.year;
		localTime.wMonth=(WORD) e.month;
		//localTime.wDayOfWeek;
		localTime.wDay=(WORD) e.day;
		localTime.wHour=(WORD) e.hour;
		localTime.wMinute=(WORD) e.min;
		localTime.wSecond=(WORD) e.sec;
		localTime.wMilliseconds=0;
		SYSTEMTIME systemTime;
		if(!TzSpecificLocalTimeToSystemTime(0,&localTime,&systemTime))
	//	if(!SystemTimeToTzSpecificLocalTime(0,&systemTime,&localTime))
		{	microseconds=0;
			return;
		}
		FILETIME fileTime;
		if(!SystemTimeToFileTime(&systemTime,&fileTime))//UTC
		{	microseconds=0;
			return;
		}
		microseconds=WindowsTimeMicroseconds(fileTime);
	}
#else
	{//"2012-08-12 12:00:00"
		if(!cstring)
		{	microseconds=0;
			return;
		}
		struct tm t;
		memset(&t,0,sizeof(t));
		sscanf(cstring,"%d%*c%d%*c%d %d:%d:%d",&t.tm_year,&t.tm_mon,&t.tm_mday,&t.tm_hour,&t.tm_min,&t.tm_sec);
		t.tm_mon--; // Month, 0 - jan
		t.tm_isdst = -1; // Is DST on? 1 = yes, 0 = no, -1 = unknown
		t.tm_year-=1900;
		microseconds = mktime(&t)*factor;//not timegm
	}
#endif

};

}

#endif
