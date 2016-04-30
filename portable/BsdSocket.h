// BsdSocket.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocket_h
#define BsdSocket_h

#ifdef UNREAL_ENGINE
#include <AllowWindowsPlatformTypes.h>
#endif

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#pragma warning(push)
#pragma warning(disable:4265)
#include <thread>
#pragma warning(pop)
#include <memory.h> 
#include <string>
#include <memory>
#include <vector>
#include "MsgBuffer.h"
#include "Packet.h"

#pragma warning(disable:4265)

namespace portable 
{

class SocketStartup
{
public:
	SocketStartup();
	~SocketStartup();
};

class BsdSocket
{protected:
	SOCKET socketfd;
	bool isGo;
	sockaddr_in server_sockaddr;
	std::thread worker;
	int OpenSocket(bool isTcp=true)
	{	if(isTcp)
		{	return (int) socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		}
		return (int) socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}
protected:
	virtual void Run()
	{}
	virtual unsigned OnPacket(unsigned bytes,portable::PacketReader& packet)
	{	return 0;
	}
	virtual void OnStop()
	{}
public:
	MsgBuffer<120> errorMsg;
	virtual ~BsdSocket()
	{}
	BsdSocket()
	:	socketfd(0)
	,	isGo(false)
	{}
	BsdSocket(SOCKET socketfd)
	:	socketfd(socketfd)
	,	isGo(false)
	{}
	BsdSocket(const BsdSocket&) = default;
	bool IsOpen() const
	{	return isGo;
	}
	bool SendTo(const char* msg,unsigned len)
	{	if(socketfd<=0)
		{	return false;
		}
		int slen = sizeof(sockaddr_in);
		if(sendto(socketfd,msg,len,0,(struct sockaddr *)&server_sockaddr,slen)==-1)
		{	puts(errorMsg.GetLastError());
			return false;
		}
		return true;
	}
	bool SendTo(Packet& packet)
	{	return SendTo(packet.GetPacket(),packet.GetPacketSize());
	}
	void Close()
	{	isGo=false;
		if(socketfd)
		{	closesocket(socketfd);
			socketfd=0;
	}	}
	int RecvFrom(char* buffer,unsigned bufsize,unsigned offset=0)
	{	int slen = sizeof(sockaddr_in);
		if(socketfd<=0)
		{	errorMsg.Set("Socket not open");
			return -1;
		}	
		return recvfrom(socketfd,buffer+offset,bufsize-offset,0,(struct sockaddr *)&server_sockaddr,&slen);
	}
	void SocketReset(const char* msg)
	{	socketfd=0;
		puts(msg);
	}
	virtual void Stop()
	{	if(isGo)
		{	isGo=false;
			SendTo("",0);
	}	}
	virtual void Start()
	{}
	void GetPeerName(std::string& s) const;
};

class BsdSocketClient
:	public BsdSocket
{	std::thread worker;
	const unsigned bufsize;
	static void Main(BsdSocketClient* self)
    {   self->Run();
    }
protected:
	void Run() override;
public:
	BsdSocketClient(unsigned bufsize)
	:	bufsize(bufsize)
	{}
	void Close()
	{	Stop();
		if(socketfd)
		{	closesocket(socketfd);
			socketfd=0;
	}	}
	bool Open(const char* serverName,int serverPort);
	void Start() override
	{	worker=std::thread(Main,this);
		worker.detach();
	}
};

class BsdSocketPool
{	
public:
	std::vector<SOCKET> socketfd;
	void Reset(unsigned size)
	{	socketfd.resize(size);
		socketfd.assign(size,0);
	}
};

class BsdSocketServer
:	public BsdSocket
{	SocketStartup socketStartup;
protected:
	BsdSocketPool pool;
	const unsigned bufsize;
	SOCKET ListenAccept();
	static void Main(BsdSocketServer* self)
    {   self->Run();
    }
	void Start() override;
public:
	virtual ~BsdSocketServer()
	{}
	BsdSocketServer(unsigned bufsize)
	:	bufsize(bufsize)
	{}
#if 0
	BsdSocket operator[](unsigned i)
	{	return pool.Get(i);
	}
#endif
	bool Open(int serverPort,int maxStreams);
	void Close()
	{	Stop();
		if(socketfd)
		{	closesocket(socketfd);
			socketfd=0;
	}	}
	virtual bool Login(SOCKET fd)
	{	return false;
	}
	void Run() override;
};

}

#ifdef UNREAL_ENGINE
#include <HideWindowsPlatformTypes.h>
#endif

#endif
