// tabstop/TabStop.cpp
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include <ctype.h>
#include "TabStop.h"

unsigned TabStop::Untab(unsigned pin,unsigned pout)
{	unsigned spaces = pout % tabsize;
	if(!spaces)
	{	spaces = tabsize;
	}
	char* outbuf = &out[pout];
	const unsigned end = pout+spaces;
	if(end>=bufsize)
	{	return 0;
	}
	for(unsigned i = pout;i<end;i++)
	{	*outbuf = ' ';
		outbuf++;
	}
	return end;
}

unsigned TabStop::Tab(unsigned spaces,unsigned pout)
{	unsigned tabs = spaces/tabsize;
	spaces %= tabsize;
	char* outbuf = &out[pout];
	const unsigned end = pout+spaces;
	if(end>=bufsize)
	{	return 0;
	}
	for(unsigned i = pout;i<end;i++)
	{	*outbuf = ' ';
		outbuf++;
	}
	return end;
}

bool TabStop::Append(unsigned pin,unsigned pout)
{	for(; pin<bufsize; pin++,pout++)
	{	if(pout>=bufsize)
		{	return false;
		}
		out[pout] = in[pin];
		if(!in[pin])
		{	return true;
	}	}
	return true;
}

bool TabStop::Open()
{	const char* filename = cmdLine.GetLast();
	if(!infile.Open(filename,"r"))
	{	printf("Can't open %s\n",filename);
		return false;
	}
	const char* tabs="--tabs";
	if(cmdLine.IsKey(tabs))
	{	if(!cmdLine.Get(tabs,tabsize) || tabsize < 1)
		{	return false;
	}	}
	return true;
}

bool TabStop::Run()
{	if(cmdLine.IsKey("--first-only") || cmdLine.IsKey("--unexpand"))
	{	return SpacesToTabs();
	}
	if(cmdLine.IsKey("--initial") || cmdLine.IsKey("--expand"))
	{	return TabsToSpaces();
	}
	puts("Not sure what you want to do...");
	return false;
}

bool TabStop::TabsToSpaces()
{	while(infile.IsGood())
	{	unsigned offset = 0;
		infile.GetLine(in);
		for(unsigned i = 0;i<in.size();i++)
		{	const char c = in[i];
			if('\t'==c)
			{	offset = Tab(i,offset);
				if(!offset)
				{	puts("Buffer overflow > 2k line");
					return false;
				}
				continue;
			}
			if(!isspace(c))
			{	Append(i,offset);
				break;
		}	}
		puts(&out[0]);
	}
	return true;
}

bool TabStop::SpacesToTabs()
{	while(infile.IsGood())
	{	const char* inbuf = &in[0];
		char* outbuf = &out[0];
		infile.GetLine(in);
		bool isMargin = true;
		for(unsigned i = 0;i<in.size();i++)
		{	const char c = in[i];
			if('\t'==c)
			{	if(outbuf - &out[0] + tabsize > (int) bufsize)
				{	puts("Buffer overflow > 2k line");
					return false;
				}
				outbuf = Tab(i,outbuf);
				continue;
			}
			*outbuf = in[i];
			outbuf++;
		}
		*outbuf = 0;
		puts(outbuf);
	}
	return true;
}
