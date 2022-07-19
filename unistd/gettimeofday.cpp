// gettimeofday.cpp
// Copyright 2016/06/20 robin.rowe@cinepaint.org
// License open source MIT

#include <unistd.h>

#include <time.h>
#include <iomanip>
#include <sstream>

extern "C" char* strptime(const char* s,const char* f,struct tm* tm) 
{	// std::get_time is defined such that its format parameters are the exact same as strptime
	std::istringstream input(s);
	input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
	input >> std::get_time(tm, f);
	if (input.fail()) {
		return nullptr;
	}
	return (char*)(s + input.tellg());
}

extern "C" int gettimeofday(struct timeval* tv, struct timezone* tz)
{	(void)tz;
    FILETIME ft;
	ULARGE_INTEGER t;
	ULONGLONG x;
	ULONGLONG m=1000000;
	GetSystemTimeAsFileTime(&ft);
	t.LowPart=ft.dwLowDateTime;
	t.HighPart=ft.dwHighDateTime;
	x=t.QuadPart/m;
	tv->tv_sec=(long) x;
	x=t.QuadPart%m;
	tv->tv_usec=(long) x;
	return 0;
}

int settimeofday(const struct timeval *tv, const struct timezone *tz)
{	(void)tv;
	(void)tz;
	STUB_NEG(settimeofday);
}

#if 0
#include <chrono>

int gettimeofday(struct timeval* tp, struct timezone* tzp) 
{	namespace sc = std::chrono;
	sc::system_clock::duration d = sc::system_clock::now().time_since_epoch();
	sc::seconds s = sc::duration_cast<sc::seconds>(d);
	tp->tv_sec = s.count();
	tp->tv_usec = sc::duration_cast<sc::microseconds>(d - s).count();
	return 0;
}

#endif