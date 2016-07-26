// CommandLine.cpp
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include "CommandLine.h

void CommandLine::Read(int argc,const char** argv)
{	for(int i=0;i<argc;i++)
	{	const char* cmd = argv[i];
		const char* eq = strchr(cmd,'=');
		if(eq)
		{	std::string key(cmd,eq-cmd);
			data[std::move(key)]=eq+1;
		}
		else
		{	data[cmd] = "true";
	}	}
}