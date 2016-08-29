// tabstop/TabStop.cpp
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include <ctype.h>
#include "TabStop.h"

bool TabStop::Open()
{	const char* filename = cmdLine.GetLast();
	if(!infile.Open(filename,"r"))
	{	printf("Can't open %s\n",filename);
		return false;
	}
	const char* tabs="--tabs";
	if(cmdLine.IsKey(tabs))
	{	if(!cmdLine.Get(tabs,tabSize) || tabSize < 1)
		{	return false;
	}	}
	return true;
}

bool TabStop::Run()
{	if(cmdLine.IsKey("--first-only") || cmdLine.IsKey("--unexpand"))
	{	TabsToSpaces(false);
		return true;
	}
	if(cmdLine.IsKey("--initial") || cmdLine.IsKey("--expand"))
	{	TabsToSpaces(true);
		return true;
	}
	puts("Not sure what you want to do...");
	return false;
}

void TabStop::TabsToSpaces(bool isTabs)
{	tabSize = 0;
	while(infile.GetLine(in))
	{	TabLine(isTabs);
		PrintLine();
	}
}

void TabStop::TabLine(bool isTabs)
{	unsigned spaces = 0;
	unsigned tabs = 0;
	for(unsigned i = 0;i<in.size();i++)
	{	if(isspace(in[i]))
		{	spaces++;
			continue;
		}
		return WriteOut(i,spaces,tabs,isTabs);
	}
	out[0] = 0;// only had spaces
}

void TabStop::WriteOut(unsigned offset,unsigned spaces,unsigned tabs,bool isToTabs)
{	char* outbuf = &out[0];
	if(!isToTabs)
	{	const unsigned length = tabs*tabSize;
		memset(outbuf,' ',length);
		AppendOut(tabs,&in[offset]);
		return;
	}
	if(!tabSize)
	{	tabSize = spaces;
	}	
	if(!tabSize)
	{	tabSize = 4;
	}
	unsigned length = spaces/tabSize;
	memset(outbuf,'\t',length);
	const unsigned extraLength = spaces % tabSize;
	if(extraLength)
	{	memset(outbuf+length,' ',extraLength);
		length += extraLength;
	}
	AppendOut(length,&in[offset]);
}
	
