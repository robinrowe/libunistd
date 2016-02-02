// BsdSocket.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocket_h
#define BsdSocket_h

#include <thread>
#include <unistd.h>
#include <memory.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <memory>
#include "MsgBuffer.h"
#include "Packet.h"

namespace portable 
{

class BsdSocket
{protected:
	SOCKET s;
	std::unique_ptr<char[]> buffer;
	unsigned bufsize;
#if 0
	int slen;
	int length;
	int recv_len;
#endif
	bool isGo;
	sockaddr_in sin;
	std::thread worker;
	int OpenSocket(bool isTcp)
	{	if(isTcp)
		{	return (int) socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		}
		else
		{	return (int) socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}	}
	bool Listen(Packet& packet)
	{	if(s<=0)
		{	return false;
		}
		sockaddr_in si_other;
		int slen = sizeof(sockaddr_in);
		const int recv_len = recvfrom(s, buffer.get(), bufsize, 0, (struct sockaddr *) &si_other, &slen);
		if(recv_len == -1)
		{	puts(errorMsg.GetSocketError());
			return false;
		}
//		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		return true;
	}
	void Run(Packet& packet)
	{	while(isGo)
		{	OnPacket(Listen(packet));
		}
		OnStop();
	}
    static void Main(BsdSocket* self)
    {   PacketSizer packetSizer(self->buffer.get(),self->bufsize);
		Packet packet(packetSizer);
		self->Run(packet);
    }
public:
	MsgBuffer<120> errorMsg;
	virtual ~BsdSocket()
	{	Close();
	}
	BsdSocket()
	:	s(0)
	,	buffer(0)
	,	bufsize(bufsize)
#if 0
	,	slen(sizeof(sockaddr_in))
	,	bufsize(0)
	,	recv_len(0)
	,	length(0)
#endif
	,	isGo(false)
	{}
	void Resize(unsigned bufsize)
	{	if(!bufsize)
		{	this->bufsize = 0;
			buffer.reset();
			return;
		}
		buffer=std::unique_ptr<char[]>(new char[bufsize]);
		this->bufsize = bufsize;
		//buffer=std::make_unique<char[]>(bufsize);
	}
	void Stop()
	{	if(isGo)
		{	isGo=false;
			SendTo("",0);
	}	}
	void Start(bool isWait)
	{	worker=std::thread(Main,this);
		if(isWait)
		{	worker.join();
		}
		else
		{	worker.detach();
	}	}
	void Close()
	{	Stop();
		if(s)
		{	closesocket(s);
			s=0;
	}	}
	bool OpenClient(const char* serverName,int serverPort,bool isTcp=true)
	{	s = OpenSocket(isTcp);
		if(s == -1)
		{	puts(errorMsg.GetSocketError());
			return false;
		}
		memset((char *) &sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons((u_short) serverPort);  
//		sin.sin_addr.S_un.S_addr = inet_addr(serverName);
		if(1!=inet_pton(AF_INET,serverName,&sin.sin_addr))
		{	puts(errorMsg.GetSocketError());
			return false;
		}
		return true;
	}
	bool SendTo(const char* msg,unsigned len)
	{	if(s<=0)
		{	return false;
		}
		int slen = sizeof(sockaddr_in);
		if(sendto(s,msg,len,0,(struct sockaddr *)&sin,slen)==-1)
		{	puts(errorMsg.GetSocketError());
			return false;
		}
		return true;
	}
	bool SendTo(Packet& packet)
	{	return SendTo(packet.get(),packet.length());
	}
	bool RecvFrom()
	{	if(s<=0)
		{	return false;
		}
		int slen = sizeof(sockaddr_in);
		if(recvfrom(s,buffer.get(),bufsize,0,(struct sockaddr *)&sin,&slen) == -1)
		{	puts(errorMsg.GetSocketError());
			return false;
		}
		return true;
	}
	const char* GetString() const
	{	return buffer.get();
	}
	bool OpenServer(int serverPort,bool isTcp)
	{	s=OpenSocket(isTcp);
		if(s == -1)
		{	puts(errorMsg.GetSocketError());
			return false;
		}
		sin.sin_family = AF_INET;
		sin.sin_port = htons((u_short) serverPort);
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		if(bind(s, (struct sockaddr*)&sin,sizeof(sin)) == -1)
		{	puts(errorMsg.GetSocketError());
			return false;
		}
		isGo=true;
		return true;
	}
#if 0
	bool Append(const char* data,unsigned length)
	{	if (this->length + length > bufsize)
		{	return false;
		}
		memcpy(buffer.get()+this->length,data,length);
		this->length+=length;
		return true;
	}
	bool SendBuffer()
	{	if(!length)
		{	return true;
		}
		const bool ok=SendTo(buffer.get(),length);
		length=0;
		return ok;
	}
#endif
	virtual void OnPacket(bool /* isGood */)
	{}
	virtual void OnStop() const
	{}
};

}

#endif
