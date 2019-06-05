// Timestamp.cp
// Created by Robin Rowe on 12/17/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <time.h>
#else
#include <sys/time.h>
#endif
#include <time.h>
#include <stdio.h>
#include <string>
#include "Timestamp.h"

namespace portable {

#ifdef _WIN32
// accurate to 0.1 microseconds
static long long WindowsTimeMicroseconds(FILETIME& ft) 
{	unsigned long long tt = ft.dwHighDateTime;
	tt <<=32;
	tt |= ft.dwLowDateTime;
	tt /=10;
	tt -= 11644473600000000ULL;
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
	const int toMicroseconds = 1000000; 
	return tv.tv_sec * toMicroseconds + tv.tv_usec;
}
#endif 

time64_t Timestamp::currentTimeMicros()
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

const char* Timestamp::toTimeString(const char* timeFormat,bool isMicros)
{	
#ifdef WIN32
	tm t;
	tm* tp=&t;
	const time64_t t64 = microseconds/1000000; 
    const errno_t err = _localtime64_s(tp,&t64); 
	const bool isOk=!err;
	if(!isOk)
    {	_strerror_s(buffer,bufsize,"");
		return buffer;
    }
#else
	time_t time;
	tm result;
	tm* tp=localtime_r(&time,&result);
	if(!tp)
    {	memcpy(buffer,"Error",6);
		return buffer;
    }
#endif
	strftime(buffer,bufsize-1,timeFormat,tp);
	if(isMicros)
	{	char* p=buffer+strlen(buffer);
		const int micros = int(microseconds%1000000);
#pragma warning( disable : 4996 )
		sprintf(p,".%6d",micros);
#pragma warning( default : 4996 )
	}
	buffer[bufsize-1]=0;
	return buffer;
}

void Timestamp::StringToEpoch(const char* cstring)
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
	const int toMicroseconds = 1000000; 
	microseconds = mktime(&t)*toMicroseconds;//not timegm
}
#endif
}

#if 0

inline
std::string Now()
{	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft,&st);
// 2014-11-22 12:45:34.001
	const unsigned BUFSIZE=30;
	std::string s(BUFSIZE,0);
	char* buffer = (char*) s.c_str();
	sprintf_s(buffer,BUFSIZE,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
		st.wYear,
		st.wMonth,
//		st.wDayOfWeek,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);
	return s;
}

#endif