// CommandLine.cpp
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include "CommandLine.h"

namespace portable
{

CommandLine::CommandLine(int argc,const char** argv)
:	argc(argc)
,	argv(argv)
{	// skip arg[0] = prog_name
    for(int i=1;i<argc;i++)
	{	Append(argv[i]);
    }
}

void CommandLine::Append(const char* keyval)
{   const char* eq = strchr(keyval,'=');
	if(eq)
	{	std::string key(keyval,eq-keyval);
		data[std::move(key)]=eq+1;
        return;
	}
    data[keyval] = "true";
}

}
