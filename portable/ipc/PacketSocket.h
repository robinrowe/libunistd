// portable/PacketSocket.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef PacketSocket_h
#define PacketSocket_h
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
#include "PacketReader.h"

#pragma warning(disable:4265)
#ifndef _WIN32
	typedef int SOCKET;
	#define closesocket close
#endif

namespace portable 
{

class PacketSocket
{protected:
	SOCKET socketfd;
	bool isGo;
	bool isConnected;
	sockaddr_in server_sockaddr;
	std::thread packetWorker;
	virtual int OpenSocket()
	{	puts("OpenSocket");
		return 0;
	}
protected:
	virtual void Run()
	{}
	virtual unsigned OnPacket(unsigned bytes,portable::PacketReader& packet)
	{	(void)bytes;
		(void)packet;
		return 0;
	}
	virtual void OnStop()
	{	puts("Socket stream stopping");
		isConnected = false;
	}
public:
	MsgBuffer<120> errorMsg;
	virtual ~PacketSocket()
	{	// Close(); Don't do this, might be a temp copy
	}
	PacketSocket()
	:	socketfd(0)
	,	isGo(false)
	,	isConnected(false)
	{}
	PacketSocket(SOCKET socketfd)
	:	socketfd(socketfd)
	,	isGo(false)
	,	isConnected(false)
	{}
//	PacketSocket(const PacketSocket&) = default;
	bool IsGood() const
	{	return socketfd > 0;
	}
	bool IsOpen() const
	{	return isGo;
	}
	bool SendTo(const char* msg,unsigned len)
	{	if(!IsGood())
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
	{	
#if 0
		static VerboseCounter counter(600);
		counter++;
		if (counter)
		{	printf("Packet #%u send %i\n", packet.GetPacketId(), packet.GetPacketSize());
		}
#endif
		return SendTo(packet.GetPacket(),packet.GetPacketSize());
	}
	int RecvFrom(char* buffer,unsigned bufsize,unsigned offset=0)
	{	socklen_t slen = sizeof(sockaddr_in);
		if(socketfd<=0)
		{	errorMsg.Set("Socket not open");
			return -1;
		}	
		return recvfrom(socketfd,buffer+offset,bufsize-offset,0,(struct sockaddr *)&server_sockaddr,&slen);
	}
	void SocketReset(const char* msg = nullptr)
	{	socketfd=0;
		if(msg)
		{	puts(msg);
	}	}
	virtual void Stop()
	{	if(isGo)
		{	isGo=false;
			SendTo("",0);
	}	}
	virtual void Start()
	{	puts("Soscket stream starting");
	}
	void GetPeerName(std::string& s) const
	{	return GetPeerName(socketfd,s);
	}
	static bool SetReuse(SOCKET socketfd,int isReuse=1)
	{	return setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const char*) &isReuse, sizeof(int)) > 0;
	}
	virtual void OnSocketError(const char* msg,unsigned len)
	{	(void)msg;
		(void)len;
		puts(errorMsg.GetLastError());
	}
	bool SendEmptyPacket()
	{	return SendTo("", 0);
	}
	static bool GetIp(const char* hostname,std::string& ip);
	static void GetPeerName(SOCKET sock, std::string& s);
	bool SetAsyncMode(bool isAsync = true);
	bool Open(const char* serverName, int serverPort,bool isReuseSocket = true);
	virtual void Close();
};

}

#endif
