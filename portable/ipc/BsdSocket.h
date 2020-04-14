// portable/BsdSocket.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocket_h
#define BsdSocket_h
#include <thread>
#include <memory.h> 
#include <string>
#include <memory>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "BsdSocketStartup.h"
#include "../bsd_string.h"
#include "../MsgBuffer.h"
#include "../AtomicCounter.h"
#include "../VerboseCounter.h"
#include "../SystemLog.h"
#include "../time/Timestamp.h"

#pragma warning(disable:4265)
#ifndef _WIN32
	typedef int SOCKET;
	#define closesocket close
#endif

namespace portable 
{

class BsdSocket
{protected:
	SOCKET socketfd;
	int bytesRead;
	bool isVerbose;
	bool isTrace;
	bool isAutoClose;
	sockaddr_in server_sockaddr;
	virtual int OpenSocket()
	{	TRACE("OpenSocket not implemented");
		return 0;
	}
	virtual void OnSocketError(const char* filename,int lineno,const char* msg)
	{	if(!isVerbose)
		{	return;
		}
		SystemLog(filename,lineno,msg);
	}
	void Trace(const char* msg)
	{	if(!isTrace)
		{	return;
		}
		portable::Timestamp now;
		const char* t = now.toString();
		printf("TRACE SOCKET(%s): %s\n",t,msg);
	}
	void Trace(const char* function,const char* msg,int length)
	{	if(!isTrace)
		{	return;
		}
		portable::Timestamp now;
		const char* t = now.toString();
		printf("TRACE SOCKET %s(%s): %.*s[%i]\n", function,t,length,msg,length);
	}
	void TraceOpen(const char* serverName,int port)
	{	if(!isTrace)
		{	return;
		}
		portable::Timestamp now;
		const char* t = now.toString();
		printf("TRACE SOCKET OPEN %s:%i @ %s\n",serverName,port,t);
	}
public:
    BsdSocket(BsdSocket const&) = delete;
    BsdSocket& operator=(BsdSocket const&) = delete;
	virtual ~BsdSocket()
	{	if(isAutoClose) // ok, not a temp copy
		{	Close(); 
	}	}
	BsdSocket()
	:	socketfd(0)
	,	bytesRead(0)
	,	isTrace(false)
	,	isVerbose(false)
	,	isAutoClose(true)
	{	memset(&server_sockaddr,0,sizeof(server_sockaddr));
	}
	BsdSocket(SOCKET socketfd)
	:	socketfd(socketfd)
	,	bytesRead(0)
	,	isTrace(false)
	,	isVerbose(false)
	,	isAutoClose(false)
	{	memset(&server_sockaddr,0,sizeof(server_sockaddr));
	}
	void SetVerbose(bool isVerbose = true)
	{	this->isVerbose = isVerbose;
	}
	bool IsVerbose() const
	{	return isVerbose;
	}
	void SetTrace(bool isTrace = true)
	{	this->isTrace = isTrace;
	}
	bool IsTrace() const
	{	return isTrace;
	}
	bool IsOpen() const
	{	return socketfd > 0;
	}
	bool Send(const char* s)
	{	if(!s)
		{	return false;
		}
		const unsigned len = (unsigned) strlen(s);
		return SendTo(s,len);
	}
	void SocketReset(const char* msg = nullptr)
	{	socketfd=0;
		if(isVerbose)
		{	puts("Socket Reset");
		}
		if(msg)
		{	puts(msg);
	}	}
	void GetPeerName(std::string& s) const
	{	return GetPeerName(socketfd,s);
	}
	static bool SetReuse(SOCKET socketfd,int isReuse=1)
	{	const char* p = (const char*) &isReuse;
		const int size = sizeof(int);
		const int status = setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,p,size);
		return status > 0;
	}
	bool SendEmptyPacket()
	{	if(isTrace)
		{	Trace("Send Empty Packet");
		}
		return SendTo("", 0);
	}
	int BytesRead() const
	{	return bytesRead;
	}
	static bool GetIp(const char* hostname,std::string& ip);
	static void GetPeerName(SOCKET sock, std::string& s);
	bool SetAsyncMode(bool isAsync = true);
	bool Open(const char* serverName, int serverPort);
	bool Connect();
	bool Bind();
//	bool SetTimeout(unsigned secondsTimeout);// Call before Bind()
	virtual void Close();
	bool SendTo(const char* msg,unsigned len);
	int RecvFrom(char* buffer,unsigned bufsize,unsigned offset=0);
};

}

#endif
