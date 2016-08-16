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
{	const size_t bufsize = 2048;
	int tabsize;
	std::vector<char> in;
	std::vector<char> out;
	portable::StdFile infile;
	portable::CommandLine cmdLine;
	bool TabsToSpaces();
	bool SpacesToTabs();
	unsigned Tab(unsigned pin,unsigned pout);
	unsigned Untab(unsigned pin,unsigned pout);
	bool Append(unsigned pin,unsigned pout);
public:
	TabStop(int argc,const char* argv[])
	:	cmdLine(argc,argv)
	,	tabsize(4)
	{	in.resize(bufsize);
		out.resize(bufsize);
	}
	bool Open();
	bool Run();
};

#endif