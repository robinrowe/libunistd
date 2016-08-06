// tabstop/main.cpp
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include <stdio.h>
#include <vector>
#include <ctype.h>
#include <portable/StdFile.h>
#include <portable/CommandLine.h>
#include "TabStop.h"

/*
	[filter "tabspace"]
		smudge = unexpand --tabs=4 --first-only
		clean = expand --tabs=4 --initial
*/

void Usage()
{	puts("Usage:");
	puts("tab \ttabstop --initial|--expand [--tabs=N] filename");
	puts("untab \ttabstop --first-only|--unexpand [--tabs=N] filename");
	puts("Outputs contents of filename converting tabs to tabstops of spaces. Default tabstop size is 4 spaces.");
}

int main(int argc,const char* argv[])
{	if(argc<2)
	{	Usage();
		return 1;
	}
	TabStop tabStop(argc,argv);
	if(!tabStop.Open())
	{	Usage();
		return 2;
	}
	if(!tabStop.Run())
	{	return 3;
	}
	return 0;
}