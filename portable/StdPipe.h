// StdPipe.h
// Created by Robin Rowe on 12/5/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org

#ifndef StdPipe_h
#define StdPipe_h

#include "StdFile.h"
#include <stdio.h>

namespace portable
{

class StdPipe
:	public StdFile
{
public:
	~StdPipe()
	{	Close();
	}
	bool Open(const char* filename)
	{	fp = popen(filename,"w");
		return 0!=fp;
	}
	int Close()
	{	if(!fp)
		{	return 0;
		}
		const int retval=pclose(fp);
		fp=0;
		return retval;
	}
};

}

#endif
