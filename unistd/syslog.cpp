// syslog.cpp
// Created by Robin Rowe on 2018/10/9
// Copyright (c) 2019 Robin.Rowe@CinePaint.org
// License open source MIT

#include <unistd.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _DEBUG
#include <crtdbg.h>
#endif
// debugapi.h
#include "../portable/SystemLog.h"

#include <string>

#pragma warning(disable:4996)

struct Syslog_data
{	FILE* fp;
//	int option;
	int facility;
	int mask;
	std::string ident;
	bool isTTY;
};
typedef
	struct Syslog_data
	Syslog_data
	;

Syslog_data syslog_data;

void openlog(const char *ident, int option, int facility)
{	if(isatty(fileno(stdout)))
	{	syslog_data.isTTY = true;
	}
	const char* err = "Error: can't openlog()";
	if(!ident || syslog_data.fp != 0)
	{	puts(err);
		return;
	}
	(void) option;
	if(!ident || !*ident)
	{	const char* ident = "unknown";
	}
	syslog_data.ident=ident;
	if(facility >= LOG_LOCAL0 && facility <= LOG_LOCAL7)
	{	std::string filename(ident);
		filename += ".log";
		syslog_data.fp = fopen(filename.c_str(),"w+");
		if(!syslog_data.fp)
		{	error_msg(err);
			return;
		}
	}
	// syslog_option = option;
	// syslog_facility = facility;
}

void syslog(int priority, const char *format, ...)
{	va_list argp;
	va_start(argp, format);
	vsyslog(priority, format, argp);
	va_end(argp);
}

void vsyslog(int priority, const char* format, va_list argp)
{	if (!(syslog_data.mask & priority))
	{	return;
	}
	if (syslog_data.fp)
	{	fprintf(syslog_data.fp, "%s: ", syslog_data.ident.c_str());
		vfprintf(syslog_data.fp, format, argp);
		fputs("", syslog_data.fp);
	}
#ifdef _DEBUG
	char msg[80];
	vsprintf(msg, format, argp);
	OutputDebugStringA(msg);
#endif
}

void closelog()
{	if(!syslog_data.fp)
	{	return;
	}
	fclose(syslog_data.fp);
	syslog_data.ident.clear();
}

int setlogmask(int mask)
{	const int prior = syslog_data.mask;
	syslog_data.mask = mask;
	return prior;
}

#pragma warning(default:4996)
