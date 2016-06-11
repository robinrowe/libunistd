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
#include "AtomicCounter.h"

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
	std::thread packetWorker;
	int OpenSocket(bool isTcp=true)
	{	if(isTcp)
		{	return (int) socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		}
		return (int) socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}
protected:
	virtual void Run()
	{}
	virtual int OnPacket(int bytes,portable::PacketReader& packet)
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
		{	OnSocketError(msg,len);
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
	static void GetPeerName(SOCKET sock,std::string& s); 
	void GetPeerName(std::string& s) const
	{	return GetPeerName(socketfd,s);
	}
	static bool SetReuse(SOCKET socketfd,int isReuse=1)
	{	return setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const char*) &isReuse, sizeof(int)) > 0;
	}
	virtual void OnSocketError(const char* msg,unsigned len)
	{	puts(errorMsg.GetLastError());
	}
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
		puts("Started socket thread");
	}
};

class BsdSocketPool
{	
public:
	std::vector<SOCKET> socketfd;
	AtomicCounter<unsigned> counter;	
	void Reset(unsigned size)
	{	socketfd.resize(size);
		socketfd.assign(size,0);
		counter=0;
	}
	SOCKET* GetSlot();
	SOCKET* GetZombieSlot();
	bool ReleaseSlot(SOCKET* sock);
	void BsdSocketPool::ReleaseSlot(unsigned slot)
	{	if(slot < socketfd.size())
		{	socketfd[slot]=0;
	}	}
};

class BsdSocketServer
:	public BsdSocket
{	SocketStartup socketStartup;
protected:
	std::thread listenWorker;
	BsdSocketPool pool;
	const unsigned bufsize;
	bool isPacketRun;
	SOCKET ListenAccept();
	static void ListenMain(BsdSocketServer* self)
    {   self->ListenRun();
    }
	static void PacketMain(BsdSocketServer* self)
    {   self->PacketRun();
    }
public:
	virtual ~BsdSocketServer()
	{}
	BsdSocketServer(unsigned bufsize)
	:	bufsize(bufsize)
	,	isPacketRun(true)
	{}
	unsigned GetConnectionCount() const
	{	return pool.counter;
	}
	bool Open(int serverPort,int maxStreams,bool isPacketRun=true);
	void Close()
	{	Stop();
		if(socketfd)
		{	closesocket(socketfd);
			socketfd=0;
	}	}
	virtual bool Login(SOCKET* slot,SOCKET fd)
	{	return false;
	}
	void Start() override
	{	listenWorker=std::thread(ListenMain,this);
		listenWorker.detach();
		if(isPacketRun)
		{	packetWorker=std::thread(PacketMain,this);
			packetWorker.detach();
	}	}
	virtual void ListenRun();
	virtual void PacketRun();
};

}

#ifdef UNREAL_ENGINE
#include <HideWindowsPlatformTypes.h>
#endif

#endif
