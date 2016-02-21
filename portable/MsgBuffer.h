// MsgBuffer.h
// Created by Robin Rowe on 1/2/2016
// Copyright (c) 2016 Robin.Rowe@CinePaint.org

#ifndef MsgBuffer_h
#define MsgBuffer_h

#include <string.h>
#include <cerrno>
#ifdef _WIN32
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#endif 

namespace portable
{

template<unsigned bufsize>
class MsgBuffer
{	char buffer[bufsize];
	void MsgBuffer::SetPerErrno()
	{	char* p=strerror(errno);
		Append(p);
	}
	void MsgBuffer::SetPerLastError()
	{	const unsigned len=length();
#ifdef _WIN32
		const DWORD num = FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			WSAGetLastError(),
			0,
			buffer+len,
			bufsize-len-1,
			NULL);
#else
		SetBufferErrno();
#endif
	}
public:
	MsgBuffer()
	{	Reset();
	}
	void Reset()
	{	buffer[0]=0;
	}
	const char* GetErrnoError() 
	{	SetPerErrno();
		return buffer;
	}
	const char* GetLastError()
	{	SetPerLastError();
		return buffer;
	}
	int GetErrno() 
	{	return errno;
	}
	int GetSocketErrorCode()
	{
#ifdef _WIN32	
		return WSAGetLastError();
#else
		return GetErrno();
#endif
	}
	unsigned length() const
	{	return (unsigned) strlen(buffer);
	}
	unsigned capacity() const
	{	return bufsize-1;
	}
	void Append(const char* text)
	{	const unsigned len=length();
#pragma warning (disable : 4996)
		strncpy(buffer+len,p,bufsize-len-1);
#pragma warning (default : 4996)
		buffer[bufsize-1]=0;
	}
};

}
#endif
