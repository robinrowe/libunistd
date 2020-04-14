// SystemCall.h
// Libunist Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef SystemCall_h
#define SystemCall_h

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

namespace portable
{

inline
void OpenConsole()
{
#ifdef _WIN32
	AllocConsole();
#pragma warning(disable: 4996) 	
	freopen("CONOUT$", "w", stdout);
#pragma warning(default: 4996)
#endif
}

class PrintTask
{
public:
	PrintTask(const char* functionName,const char* description = "")
	{	if(!functionName || !description)
		{	return;
		}
#ifdef TRACE_TASKS
		static unsigned i;
		i++;
		printf("Thread(%u): %s() %s\n",i,functionName,description);
#endif
	}
};

[[ noreturn ]]
inline
void StubExit(int errorlevel,const char* msg,const char* file,const char* function,int line)
{	printf("exit(%i): %s %s\n%s:%i",errorlevel,msg,function,file,line);
	exit(errorlevel);
}

[[ noreturn ]]
inline
void StubAssert(int errorlevel,const char* msg,const char* file,const char* function,int line)
{	printf("assert(%s): %s\n%s:%i",msg,function,file,line);
	exit(errorlevel);
}

inline
int SystemCall(const char* cmd)
{	(void)cmd;
#ifdef TRACE_SYSTEM_CALLS
	static int i;
	printf("[%i] system(%s)\n",++i,cmd);
#endif
#ifdef _WIN32
	return 1;
#else
	return system(cmd);
#endif
}

template <typename T>
bool memcopy(T& dest,const char* start,unsigned size)
{	const auto p=dest.data();
	if(dest.size()*sizeof(*p) < size*sizeof(char) )
	{	return false;
	}
	memcpy(dest.data(),start,size);
	return true;
}

template <typename T>
bool std_cpy(const char* start,const char* end,T& dest)
{	const unsigned size = (end-start)*sizeof(char);
	const auto p=dest.data();
	if(dest.size()*sizeof(*p) < size*sizeof(char) )
	{	return false;
	}
	memcpy(dest.data(),start,size);
	return true;
}

enum class OS {unknown_os,android_os,linux_os,ios_os,mac_os,windows_os};

inline 
OS OperatingSystem()
{
#if defined(__ANDROID__)
	return OS::android_os;
#elif defined(__gnu_linux__) || defined(__linux__)
	return OS::linux_os;
#elif defined(__APPLE__) && defined(__MACH__)
#if defined(TARGET_OS_IPHONE)
	return OS::ios_os;
#else
	return OS::mac_os;
#endif
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || defined(WIN32)
	return OS::windows_os;
#else
	return OS::unknown_os;
#endif
}

} //portable

#define SystemExit(errorlevel) portable::StubExit(errorlevel,"Exit ",__FILE__, __FUNCTION__, __LINE__)

#define SystemAssert(expression)  (void)( (!!(expression)) || portable::StubAssert(-1,#expression,__FILE__, __FUNCTION__, __LINE__) )

#ifdef _WIN32
#define ANSI_RED   
#define ANSI_GRN   
#define ANSI_YEL   
#define ANSI_BLU   
#define ANSI_MAG   
#define ANSI_CYN   
#define ANSI_WHT   
#define ANSI_CLR   
#else
#define ANSI_RED   "\x1B[31m"
#define ANSI_GRN   "\x1B[32m"
#define ANSI_YEL   "\x1B[33m"
#define ANSI_BLU   "\x1B[34m"
#define ANSI_MAG   "\x1B[35m"
#define ANSI_CYN   "\x1B[36m"
#define ANSI_WHT   "\x1B[37m"
#define ANSI_CLR   "\x1B[0m"
#endif

//  printf(ANSI_RED "red" ANSI_CLR);

#endif

