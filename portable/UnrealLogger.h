// UnrealLogger.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// Open source license MIT

#ifndef UnrealLogger_h
#define UnrealLogger_h

#include "UnrealLogger.h"

class UnrealLogger
{	FString s;
	FName categoryName;
	void Puts(const char* filename,int lineNo,const char* msg,ELogVerbosity::Type t)
	{	s=msg;
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

#endif
