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

#pragma warning(disable:4996)

Syslog_data syslog_data;

void openlog(const char *programname, int option, int facility)
{	if(isatty(fileno(stdout)))
	{	syslog_data.isTTY = true;
	}
	const char* err = "Error: can't openlog()";
	if(!programname || syslog_data.fp != 0)
	{	puts(err);
		return;
	}
	(void) option;
	const char* filename = "/var/log/messages";
	strcpy(syslog_data.programname,filename);
	if(facility >= LOG_LOCAL0 && facility <= LOG_LOCAL7)
	{	strncpy(syslog_data.programname+9,programname,SYSLOG_PROG_SIZE-10);
		syslog_data.programname[SYSLOG_PROG_SIZE-1] = 0;
	}
	syslog_data.fp = fopen(filename,"w+");
    if(!syslog_data.fp)
	{	puts(err);
		return;
	}
	// syslog_option = option;
	// syslog_facility = facility;
}

void syslog(int priority, const char *format, ...)
{	if(!(syslog_data.mask & priority))
	{	return;
	}
	va_list argp;
	va_start(argp,format);
	if(syslog_data.fp)
	{	fprintf(syslog_data.fp,"%s: ",syslog_data.programname+9);
		vfprintf(syslog_data.fp,format,argp);
		fputs("",syslog_data.fp);
	}
#ifdef _DEBUG
	char msg[80];
	vsprintf(msg,format,argp);
	OutputDebugStringA(msg);
#endif
	va_end(argp);
}

void closelog() 
{	if(!syslog_data.fp)
	{	return;
	}
	fclose(syslog_data.fp);
	syslog_data.fp = 0;
	syslog_data.programname[0] = 0;
}

int setlogmask(int mask)
{	const int prior = syslog_data.mask;
	syslog_data.mask = mask;
	return prior;
}

#pragma warning(default:4996)
