// chrono.h: Windows emulation of common time functions
// Libunistd Copyright Nov 10, 2002, Robin.Rowe@CinePaint.org
// License MIT (http://opensource.org/licenses/mit-license.php)

#ifndef chrono_h
#define chrono_h

#include <chrono>
#include <thread>
#include <string>

#pragma warning(disable:4996)

typedef long long useconds_t;
#define sleep(x) Sleep(x)

inline
int usleep(useconds_t delay)
{	std::this_thread::sleep_for(std::chrono::nanoseconds(delay));
	return 0;
}

#if 0
inline
int usleep(useconds_t delay)
STUB0(usleep)

inline
std::string Now()
{	time_t now;
	now = time(NULL);
	return ctime(&now);
}
#endif

inline
std::string Now()
{	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft,&st);
// 2014-11-22 12:45:34.001
	const unsigned BUFSIZE=30;
	char buffer[BUFSIZE];
	sprintf_s(buffer,BUFSIZE,"%04d-%02d-%02d %02d:%02d:%02d.%03d",
		st.wYear,
		st.wMonth,
//		st.wDayOfWeek,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		st.wMilliseconds);
	return buffer;
}

typedef int clockid_t;

enum
{	CLOCK_REALTIME,
	CLOCK_MONOTONIC,
	CLOCK_PROCESS_CPUTIME_ID,
	CLOCK_THREAD_CPUTIME_ID
};

#if _MSC_VER <= 1800
struct timespec {
        time_t   tv_sec;        /* seconds */
        long     tv_nsec;       /* nanoseconds */
};

// std::chrono::high_resolution_clock
#endif

inline
int clock_getres(clockid_t clk_id, struct timespec *res)
STUB0(clock_getres)

// seconds and nanosecond count of the time since the Epoch (00:00 1 January, 1970 UTC).

inline
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

inline
int clock_settime(clockid_t clk_id, const struct timespec *tp)
STUB0(clock_settime)

inline
int nanosleep(const struct timespec *req, struct timespec *rem)
{	long long delay = req->tv_sec;
	const long long billion = 1000000000LL;
	delay*=billion;
    delay+=req->tv_nsec;
	return usleep(delay);
}

#pragma warning(default:4996)

#endif