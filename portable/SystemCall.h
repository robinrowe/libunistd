// SystemCall_h
// Libunist Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef SystemCall_h
#define SystemCall_h

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#if 0
#ifdef __clang__
#include <stdnoreturn.h>
#define no_return noreturn
#elif __GNUC__
#define no_return  __attribute__((__noreturn__))
#elif _MSC_VER
#define no_return
#endif
#endif

namespace portable
{
	
class PrintTask
{
public:
	PrintTask(const char* functionName,const char* description = "")
	{
#ifdef TASK_TRACE
		static unsigned i;
		i++;
		printf("Thread(%u): %s() %s\n",i,functionName,description);
#endif
		(void) functionName;
		(void) description;
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
{	
#ifdef _DEBUG
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

enum OS {unknown_os,android_os,linux_os,ios_os,mac_os,windows_os};

inline 
OS OperatingSystem()
{
#if defined(__ANDROID__)
	return android_os;
#elif defined(__gnu_linux__) || defined(__linux__)
	return linux_os;
#elif defined(__APPLE__) && defined(__MACH__)
#if defined(TARGET_OS_IPHONE)
	return ios_os;
#else
	return mac_os;
#endif
#elif defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || defined(WIN32)
	return windows_os;
#else
	return unknown_os;
#endif
}

} //portable

#define SystemExit(errorlevel) portable::StubExit(errorlevel,"Exit ",__FILE__, __FUNCTION__, __LINE__)

#define SystemAssert(expression)  (void)( (!!(expression)) || portable::StubAssert(-1,#expression,__FILE__, __FUNCTION__, __LINE__) )

#endif

