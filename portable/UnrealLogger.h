// UnrealLogger.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// Open source license MIT

/* Add this to your UE project header:

DECLARE_LOG_CATEGORY_EXTERN(homStream, Log, All);

Add to project source file:

#include <portable/Logger.h>
DEFINE_LOG_CATEGORY(your_plugin_name);
UnrealLogger logger("your_plugin_name");

*/

#ifndef UnrealLogger_h
#define UnrealLogger_h

#ifdef UNREAL_ENGINE
#include <AllowWindowsPlatformTypes.h>
#endif

//#include <Runtime/CoreUObject/Public/UObject/ObjectBase.h>
#include <portable/Counter.h>
#include <crtdbg.h>

class UnrealLogger
{	FString s;
	FName categoryName;
	Counter c;
	void Puts(const char* filename,int lineNo,const char* msg,ELogVerbosity::Type t)
	{	s=c(msg);
		puts(msg);
		FMsg::Logf_Internal(filename, lineNo,  categoryName, t, *s); 
		s.Reset();
	}
	void OutputWindow(const char* filename,int lineNo,const char* msg)
	{//OutputDebugStringA("My output string.");
		//_RPTF2(_CRT_WARN, "In NameOfThisFunc( )," " someVar= %d, otherVar= %d\n", someVar, otherVar );
		//_CRT_WARN, _CRT_ERROR, and _CRT_ASSERT.
#ifdef _DEBUG
		_CrtDbgReport( _CRT_WARN,filename,lineNo,msg,NULL);
#endif
	}
public:
	UnrealLogger(const char* loggerName)
	{	categoryName = loggerName;
		OutputWindow(loggerName,0,"_CrtDbgReport enabled");
	}
	void Log(const char* filename,int lineNo,const char* msg)
	{	OutputWindow(filename,lineNo,msg);
		Puts(filename,lineNo,msg,ELogVerbosity::Display);
	}
	void Error(const char* filename,int lineNo,const char* msg)
	{	s="LogError: ";
		s+=msg;
		Puts(filename,lineNo,TCHAR_TO_ANSI(*s),ELogVerbosity::Warning); 
		s.Reset();
	}
};

extern UnrealLogger logger;

#define LogMsg(msg) logger.Log(__FILE__, __LINE__,msg)
#define LogError(msg) logger.Error(__FILE__, __LINE__,msg)


#ifdef UNREAL_ENGINE
#include <HideWindowsPlatformTypes.h>
#endif

#endif

