// FirstError.h
// Created by Robin Rowe on 1/2/2016
// Copyright (c) 2016 Robin.Rowe@CinePaint.org

#ifndef FirstError_h
#define FirstError_h

#include <cerrno>

class FirstError
{	static const int bufsize=80;
	char buffer[bufsize];
	void SetBufferErrno();
	void SetBufferSocketError();
public:
	FirstError()
	{	Reset();
	}
	void Reset()
	{	buffer[0]=0;
	}
	const char* GetSystemError() 
	{	SetBufferErrno();
		return buffer;
	}
	const char* GetSocketError()
	{	SetBufferSocketError();
		return buffer;
	}
	int GetErrno() 
	{	return errno;
	}
	int GetErrnoWSA();
};

extern FirstError firstError;

#endif
