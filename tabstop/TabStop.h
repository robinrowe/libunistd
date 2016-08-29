// tabstop/TabStop.h
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef TabStop_h
#define TabStop_h

#include <stdio.h>
#include <vector>
#include <ctype.h>
#include <portable/StdFile.h>
#include <portable/CommandLine.h>

class TabStop
{	const size_t bufSize = 2048;
	int tabSize;
	std::vector<char> in;
	std::vector<char> out;
	portable::StdFile infile;
	portable::CommandLine cmdLine;
	void PrintLine()
	{	puts(&out[0]);
	}
#pragma warning(disable:4996)
	void AppendOut(unsigned offset,const char* pin)
	{	strncpy(&out[offset],pin,bufSize-offset);
		out[bufSize-1] = 0;
	}
#pragma warning(default:4996)
	void TabsToSpaces(bool isTabs);
	void TabLine(bool isTabs);
	void WriteOut(unsigned offset,unsigned spaces,unsigned tabs,bool isTabs);
	unsigned SpaceOut(unsigned pin,unsigned pout);
public:
	TabStop(int argc,const char* argv[])
	:	tabSize(4)
	,	cmdLine(argc,argv)
	{	in.resize(bufSize);
		out.resize(bufSize);
		in[0] = 0;
		out[0] = 0;
	}
	bool Open();
	bool Run();
};

#endif