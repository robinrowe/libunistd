// clock_gettime.cpp: Windows emulation of common time functions
// Libunistd Copyright Nov 10, 2002, Robin.Rowe@CinePaint.org
// License MIT (http://opensource.org/licenses/mit-license.php)

#undef _CRT_NO_TIME_T
#include <time.h>
#include <chrono>
#include <thread>
#include <string>
#include "unistd.h"
#include "../portable/stub.h"
#include "clock_gettime.h"

#pragma warning(disable:4996)

#if _MSC_VER <= 1800
struct timespec {
        time_t   tv_sec;        /* seconds */
        long     tv_nsec;       /* nanoseconds */
};

// std::chrono::high_resolution_clock
#endif

int sleep(useconds_t delay)
{	std::this_thread::sleep_for(std::chrono::seconds(delay));
	return 0;
}

int usleep(useconds_t delay)
{	std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
	return 0;
}

int nanosleep(const struct timespec *req, struct timespec *rem)
{	(void)rem;
	long long delay = req->tv_sec;
	const long long billion = 1000000000LL;
	delay*=billion;
    delay+=req->tv_nsec;
	return usleep(delay);
}

int clock_getres(clockid_t clk_id, struct timespec *res)
{	(void)clk_id;
	(void)res;
	STUB_NEG(clock_getres);
}

// seconds and nanosecond count of the time since the Epoch (00:00 1 January, 1970 UTC).

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
#if 0
    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER end;
    double elapsedSeconds;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    
	QueryPerformanceCounter(&end);
    elapsedSeconds = (end.QuadPart - start.QuadPart) / (double)frequency.QuadPart;
#endif
	if(CLOCK_REALTIME != clk_id && CLOCK_MONOTONIC != clk_id)
	{	return -1;
	}
	SYSTEMTIME systemTime;
	GetSystemTime(&systemTime);
	FILETIME fileTime;
	SystemTimeToFileTime(&systemTime,&fileTime);
	ULARGE_INTEGER now = *(ULARGE_INTEGER*) &fileTime;
	memset(&systemTime,0,sizeof(systemTime));
	systemTime.wYear = 1970;
	systemTime.wMonth = 1;
	systemTime.wDay = 1;
	SystemTimeToFileTime(&systemTime,&fileTime);
	ULARGE_INTEGER in1970 = *(ULARGE_INTEGER*) &fileTime;
	long long diff = now.QuadPart - in1970.QuadPart; //100-nanosecond intervals since January 1, 1970
	diff *= 100;//nanoseconds
	const long long nanosPerSecond = 1000*1000*1000;
	tp->tv_sec = diff/nanosPerSecond; // 31557600 seconds/year, * 35 years = 1,420,092,000, says 1,464,129,754
	tp->tv_nsec = diff%nanosPerSecond; // says 126,000,000, / 1,000,000,000 = .126
	return 0;
}

int clock_settime(clockid_t clk_id, const struct timespec *tp)
{	(void)clk_id;
	(void)tp;
	STUB_NEG(clock_settime);
}


