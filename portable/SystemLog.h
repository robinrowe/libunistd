// portable/SystemLog.h
// Copyright 2018/4/3 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef SystemLog_h
#define SystemLog_h

namespace portable
{

#ifdef _WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#else
#include <syslog.h>
#endif
#ifdef QT
#include <QStringView>
#endif
#include <stdio.h>
#ifdef SYSTEM_LOGFILE
extern FILE* systemLogfile;
#endif

inline
void SystemLog(const char* filename,int lineNo,const char* msg)
{/*OutputDebugStringA("My output string.");
	_RPTF2(_CRT_WARN, "In NameOfThisFunc( )," " someVar= %d, otherVar= %d\n", someVar, otherVar );
	_CRT_WARN, _CRT_ERROR, and _CRT_ASSERT.*/
	if(!filename)
	{	filename = "unknown";
	}
#ifdef _WIN32
	enum {len = 60};
	char buffer[len];
	const DWORD num = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		WSAGetLastError(),
		0,
		buffer,
		len-1,
		NULL);
	if(!msg && *buffer)
	{	msg = buffer;
		buffer[len-1] = 0;
	}
#endif
#pragma warning(disable:4996)
	if(!msg || !*msg)
	{	msg = strerror(errno);
	}
	printf("%s@%i: %s\n",filename,lineNo,msg);
#ifdef _DEBUG
#ifdef QT
	qDebug() << filename << '@' << lineNo << ': " << msg;
#else
	_CrtDbgReport( _CRT_WARN,filename,lineNo,msg,NULL);
#endif
	DebugBreak();
#endif
#ifdef SYSTEM_LOGFILE
	if(systemLogfile)
	{	fprintf(systemLogfile,"%s@%s: %s\n",filename,lineNo,msg);
	}
#endif
}

inline
void StatusMsg(const char* msg)
{
#ifdef _DEBUG
#ifdef QT
	qDebug() << msg;
#else
	OutputDebugStringA(msg);
#endif
#endif
	printf("Status: %s\n",msg);
}

}

#ifdef _DEBUG
#define TRACE(msg) portable::SystemLog(__FILE__,__LINE__,msg)
#else
#define TRACE(msg) 
#endif
#pragma warning(default:4996)

#endif
