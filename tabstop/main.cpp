// tabstop/main.cpp
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#include <portable/StdFile.h>
#include <stdio.h>
#include <vector>

void Usage()
{	puts("Usage:");
	puts("\ttabstop filename [tabsize]");
	puts("Outputs contents of filename converting tabs to tabstops of spaces. Default tabstop size is 4 spaces.");
}

char* TabStop(unsigned offset,unsigned tabsize,char* outbuf)
{	unsigned spaces = offset % tabsize;
	if(!spaces)
	{	spaces = tabsize;
	}
	for(unsigned i = 0;i<spaces;i++)
	{	*outbuf = ' ';
		outbuf++;
	}
	return outbuf;
}

int main(int argc, char* argv[])
{	if(argc<2)
	{	Usage();
		return 1;
	}
	const size_t bufsize = 2048;
	std::vector<char> in(bufsize);
	std::vector<char> out(bufsize);
	StdFile infile;
	if(!infile.Open(argv[1]))
	{	printf("Can't open %s\n");
		return 2;
	}
	int tabsize = 4;
	if(argc> 2)
	{	tabsize = atoi(argv[2]);
	}
	if(tabsize < 1)
	{	Usage();
		return 3;
	}
	while(infile.IsGood())
	{	const char* inbuf = &in[0];
		char* outbuf = &out[0];
		infile.GetLine(inbuf);
		for(unsigned i = 0;i<in.size();i++)
		{	const char c = in[i];
			if('\t'==c)
			{	if(outbuf - &out[0) + tabsize > bufsize)
				{	puts("Buffer overflow > 2k line");
					return 4;
				}
				outbuf = TabStop(i,outbuf);
				continue;
			}
			*outbuf = in[i];
			outbuf++;
	}	}
	return 0;
}