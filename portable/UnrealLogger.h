// UnrealLogger.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// Open source license MIT

#ifndef UnrealLogger_h
#define UnrealLogger_h

#include <Runtime/CoreUObject/Public/UObject/ObjectBase.h>
#include <portable/Counter.h>

class UnrealLogger
{	FString s;
	FName categoryName;
	Counter c;
	void Puts(const char* filename,int lineNo,const char* msg,ELogVerbosity::Type t)
	{	s=c(msg);
		FMsg::Logf_Internal(filename, lineNo,  categoryName, t, *s); 
		s.Reset();
	}
public:
	UnrealLogger(const char* loggerName)
	{	categoryName = loggerName;
	}
	void Log(const char* filename,int lineNo,const char* msg)
	{	Puts(filename,lineNo,msg,ELogVerbosity::Display);
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

#endif
