// Cfile.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef Cfile_h
#define Cfile_h

#include <stdio.h>

#pragma warning(disable:4996)

class Cfile
{	FILE* fp;
public:
	~Cfile()
	{	Close();
	}
	Cfile()
	:	fp(0)
	{}
	bool operator!() const
	{	return !fp;
	}
	Cfile(const char* filename,const char* mode)
	{	Open(filename,mode);
	}
	bool Open(const char* filename,const char* mode)
	{	fp=fopen(filename,mode);
		return 0!=fp;
	}
	void Close()
	{	if(fp)
		{	fclose(fp);
			fp=0;
	}	}
	operator FILE*()
	{	return fp;
	}
};

#pragma warning(default:4996)

#endif
