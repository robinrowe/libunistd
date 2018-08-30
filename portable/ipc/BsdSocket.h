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
#include "../MsgBuffer.h"
#include "../AtomicCounter.h"
#include "../VerboseCounter.h"

#pragma warning(disable:4265)

namespace portable 
{

class BsdSocket
{protected:
	SOCKET socketfd;
	bool isVerbose;
	sockaddr_in server_sockaddr;
	virtual int OpenSocket()
	{	puts("OpenSocket");
		return 0;
	}
public:
	MsgBuffer<120> errorMsg;
	virtual ~BsdSocket()
	{	// Close(); Don't do this, might be a temp copy
	}
	BsdSocket()
	:	socketfd(0)
	{}
	BsdSocket(SOCKET socketfd)
	:	socketfd(socketfd)
	{}
	BsdSocket(const BsdSocket&) = default;
	void SetVerbose(bool isVerbose = true)
	{	this->isVerbose = isVerbose;
	}
	bool IsVerbose() const
	{	return isVerbose;
	}
	bool IsOpen() const
	{	return socketfd > 0;
	}
	bool SendTo(const char* msg,unsigned len)
	{	if(!IsOpen())
		{	return false;
		}
		int slen = sizeof(sockaddr_in);
		if(isVerbose)
		{	printf("SendTo: %.*s", len, msg);
			//fwrite(msg,len,1,stdout);
		}
		if(sendto(socketfd,msg,len,0,(struct sockaddr *)&server_sockaddr,slen)==-1)
		{	OnSocketError(msg,len);
			return false;
		}
		return true;
	}
	bool Send(const char* s)
	{	if(!s)
		{	return false;
		}
		const unsigned len = (unsigned) strlen(s);
		return SendTo(s,len);
	}
	int RecvFrom(char* buffer,unsigned bufsize,unsigned offset=0)
	{	int slen = sizeof(sockaddr_in);
		if(socketfd<=0)
		{	errorMsg.Set("Socket not open");
			return -1;
		}	
		const int bytes = recvfrom(socketfd,buffer+offset,bufsize-offset,0,(struct sockaddr *)&server_sockaddr,&slen);
		if(isVerbose && bytes >= 0)
		{	printf("RecvFrom: %.*s", bytes, buffer+offset);
			//fwrite(msg,len,1,stdout);
		}
		return bytes;
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
	virtual void OnSocketError(const char* msg,unsigned len)
	{	(void)msg;
		(void)len;
		puts(errorMsg.GetLastError());
	}
	bool SendEmptyPacket()
	{	if(isVerbose)
		{	puts("Send Empty Packet");
		}
		return SendTo("", 0);
	}
	static bool GetIp(const char* hostname,std::string& ip);
	static void GetPeerName(SOCKET sock, std::string& s);
	bool SetAsyncMode(bool isAsync = true);
	bool Open(const char* serverName, int serverPort);
	bool Connect();
	bool Bind();
	bool SetTimeout(unsigned secondsTimeout);// Call before Bind()
	virtual void Close();
};

}

#endif
