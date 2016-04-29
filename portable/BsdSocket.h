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
	bool isTcp;
	sockaddr_in server_sockaddr;
	int OpenSocket();
	std::thread worker;
protected:
	virtual void Run()
	{}
public:
	MsgBuffer<120> errorMsg;
	virtual ~BsdSocket()
	{}
	BsdSocket()
	:	socketfd(0)
//	,	bufsize(0)
	,	isGo(false)
	,	isTcp(true)
	{}
	BsdSocket(SOCKET socketfd)
	:	socketfd(socketfd)
//	,	bufsize(0)
	,	isGo(false)
	,	isTcp(true)
	{}
	BsdSocket(const BsdSocket&) = default;
	void SetIsTcp(bool tf)
	{	isTcp=tf;
	}
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
#if 0
	const char* GetBuffer() const
	{	return buffer.get();
	}
#endif
	void Close()
	{	isGo=false;
		if(socketfd)
		{	closesocket(socketfd);
			socketfd=0;
	}	}
	int RecvFrom(char* buffer,unsigned bufsize,sockaddr_in& server_sockaddr)
	{	int slen = sizeof(sockaddr_in);
		if(socketfd<=0)
		{	errorMsg.Set("Socket not open");
			return -1;
		}	
		return recvfrom(socketfd,buffer,bufsize,0,(struct sockaddr *)&server_sockaddr,&slen); 
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
	virtual void OnPacket(int bytes,portable::PacketReader& packet)
	{}
	virtual void ReadPacket() 
	{}
	virtual void OnStop()
	{}
	void GetPeerName(std::string& s) const;
};

class BsdSocketClient
:	public BsdSocket
{	std::thread worker;
//	std::unique_ptr<char[]> buffer;
	char* buffer;
	unsigned bufsize;
	int RecvFrom(char* buffer,unsigned bufsize)
	{	int slen = sizeof(sockaddr_in);
		if(socketfd<=0)
		{	errorMsg.Set("Socket not open");
			return -1;
		}	
		return recvfrom(socketfd,buffer,bufsize,0,(struct sockaddr *)&server_sockaddr,&slen);
	}
	static void Main(BsdSocketClient* self)
    {   self->Run();
    }
protected:
	void Run() override;
	void OnPacket(int bytes,portable::PacketReader& packet) override;
public:
	BsdSocketClient()
	:	buffer(0)
	,	bufsize(0)
	{}
	BsdSocketClient(char* buffer,unsigned bufsize)
	:	buffer(buffer)
	,	bufsize(bufsize)
	{}
	void Close()
	{	Stop();
		if(socketfd)
		{	closesocket(socketfd);
			socketfd=0;
	}	}
	bool Open(const char* serverName,int serverPort);
	void Start() override;
};

class BsdSocketPool
{	
public:
	std::vector<SOCKET> socketfd;
	void Reset(unsigned size)
	{	socketfd.resize(size);
		socketfd.assign(size,0);
	}
#if 0
	BsdSocket Get(unsigned i)
	{	if(i>=socketfd.size())
		{	return BsdSocket(0);
		}
		return BsdSocket(socketfd[i]);
	}
#endif
};

class BsdSocketServer
:	public BsdSocket
{	SocketStartup socketStartup;
protected:
	BsdSocketPool pool;
	unsigned maxStreams;
	SOCKET ListenAccept();
	static void Main(BsdSocketServer* self)
    {   self->Run();
    }
	void Start() override;
	virtual void OnPacket(int bytes,portable::PacketReader& packet) override;
public:
	virtual ~BsdSocketServer()
	{}
	BsdSocketServer()
	:	maxStreams(0)
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
