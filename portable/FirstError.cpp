// FirstError.cpp
// Created by Robin Rowe on 1/2/2016
// Copyright (c) 2016 Robin.Rowe@CinePaint.org

#include "FirstError.h"
#include <Winsock2.h>
#include <string.h>
#pragma comment(lib, "Ws2_32.lib")

FirstError firstError;

int FirstError::GetErrnoWSA()
{
#ifdef _WIN32	
	return WSAGetLastError();
#else
	return 0;
#endif
}

void FirstError::SetBufferErrno()
{
#pragma warning (disable : 4996)
	char* p=strerror(errno);
	strncpy(buffer,p,bufsize);
#pragma warning (default : 4996)
}

void FirstError::SetBufferSocketError()
{
#ifdef _WIN32
	DWORD num = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		WSAGetLastError(),
		0,
		buffer,
		bufsize,
		NULL);
#else
	SetBufferErrno();
#endif
}