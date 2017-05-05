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
#include "../vcpp/unistd.h"
#include "../vcpp/arpa/inet.h"
#include "../vcpp/netinet/in.h"
#include "MsgBuffer.h"
#include "PacketReader.h"
#include "AtomicCounter.h"
#include "VerboseCounter.h"

#pragma warning(disable:4265)

namespace portable 
{

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
	virtual unsigned OnPacket(unsigned bytes,portable::PacketReader& packet)
	{	(void)bytes;
		(void)packet;
		return 0;
	}
	virtual void OnStop()
	{	puts("Soscket stream stopping");
	}
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
	void Close()
	{	puts("Socket close");
		isGo=false;
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
	{	puts("Soscket stream starting");
	}
	static void GetPeerName(SOCKET sock,std::string& s); 
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
	bool SetBlockingMode(bool isBlocking = true)
	{	if(!IsGood())
		{	return false;
		}
#ifdef _WIN32
		unsigned long mode = isBlocking ? 0 : 1;
		return (ioctlsocket(socketfd, FIONBIO, &mode) == 0) ? true : false;
#else
		int flags = fcntl(socketfd, F_GETFL, 0);
		if (flags < 0) 
		{	return false;
		}
		flags = isBlocking ? (flags&~O_NONBLOCK) : (flags | O_NONBLOCK);
		return (fcntl(socketfd, F_SETFL, flags) == 0) ? true : false;
#endif
	}
	static bool GetIp(const char* hostname,std::string& ip);
};

}

#endif
