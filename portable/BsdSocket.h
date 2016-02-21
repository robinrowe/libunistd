// BsdSocket.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocket_h
#define BsdSocket_h

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <memory.h> 
#include <string>
#include <memory>
#include "MsgBuffer.h"
#include "Packet.h"

namespace portable 
{

class BsdSocket
{protected:
	SOCKET socketfd;
	SOCKET newsockfd;
	std::unique_ptr<char[]> buffer;
	unsigned bufsize;
	int receiveLength;
	bool isGo;
	bool isTcp;
	bool isClient;
	sockaddr_in server_sockaddr;
	std::thread worker;
	static void Main(BsdSocket* self)
    {   self->Run();
    }
private:
	int OpenSocket(bool isTcp)
	{	if(isTcp)
		{	return (int) socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		}
		else
		{	return (int) socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	}	}
	bool ListenAccept()
	{   const int backlog = 1; //point-to-point, not SOMAXCONN;
		listen(socketfd,backlog); 
		sockaddr_in cli_addr;
		int clilen = sizeof(cli_addr);
		puts("\nListening...");
		newsockfd = accept(socketfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd < 0) 
		{	perror("ERROR on accept");
			return false;
		}
		return true;
	}   
	void Run()
	{	PacketSizer packetSizer(buffer.get(),bufsize);
		Packet packet(packetSizer);
		while(isGo)
		{	if(!isClient && newsockfd<=0)
			{	ListenAccept();
			}
			const bool isGood = RecvFrom();
			packet.Rewind();
			OnPacket(isGood,packet);
		}
		OnStop();
	}
	bool OpenClient(const char* serverName,int serverPort)
	{	socketfd= OpenSocket(isTcp);
		if(socketfd == -1)			
		{	puts(errorMsg.GetLastError());
			return false;
		}
		memset((char *) &server_sockaddr, 0, sizeof(server_sockaddr));
		server_sockaddr.sin_family = AF_INET;
		server_sockaddr.sin_port = htons((u_short) serverPort);  
//		server_sockaddr.sin_addr.S_un.S_addr = inet_addr(serverName);
		if(1!=inet_pton(AF_INET,serverName,&server_sockaddr.sin_addr))
		{	puts(errorMsg.GetLastError());
			return false;
		}
		const int ok = connect(socketfd, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr));
		if(ok<0) 
		{	perror("ERROR connecting");
			return false;
		}
		return true;
	}
	bool OpenServer(const char* ,int serverPort)
	{	socketfd=OpenSocket(isTcp);
		if(socketfd == -1)
		{	puts(errorMsg.GetLastError());
			return false;
		}
		server_sockaddr.sin_family = AF_INET;
		server_sockaddr.sin_port = htons((u_short) serverPort);
		server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		if(::bind(socketfd, (struct sockaddr*)&server_sockaddr,sizeof(server_sockaddr)) == -1)
		{	puts(errorMsg.GetLastError());
			return false;
		}
		isGo=true;
		return true;
	}
	BsdSocket(BsdSocket&);
	void operator=(BsdSocket&);
public:
	MsgBuffer<120> errorMsg;
	virtual ~BsdSocket()
	{	Close();
	}
	BsdSocket()
	:	socketfd(0)
	,	newsockfd(0)
	,	bufsize(bufsize)
	,	isGo(false)
	,	isTcp(true)
	,	isClient(true)
	,	receiveLength(0)
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
	bool Open(const char* serverName,int serverPort,bool isClient,bool isTcp=true)
	{	this->isClient = isClient;
		this->isTcp = isTcp;
		if(isClient)
		{	return  OpenClient(serverName,serverPort);
		}
		return  OpenServer(serverName,serverPort);
	}	
	void Stop()
	{	if(isGo)
		{	isGo=false;
			SendTo("",0);
	}	}
	void Start(bool isBlocking)
	{	worker=std::thread(Main,this);
		if(isBlocking)
		{	worker.join();
		}
		else
		{	worker.detach();
	}	}
	void Close()
	{	Stop();
		if(socketfd)
		{	closesocket(socketfd);
			socketfd=0;
	}	}
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
	{	return SendTo(packet.get(),packet.length());
	}
	bool RecvFrom()
	{	int slen = sizeof(sockaddr_in);
		if(isClient)
		{	if(socketfd<=0)
			{	errorMsg.Set("Socket not open");
				return false;
			}	
			receiveLength = recvfrom(socketfd,buffer.get(),bufsize,0,(struct sockaddr *)&server_sockaddr,&slen);
		}
		else
		{	if(newsockfd<=0)
			{	errorMsg.Set("Socket not open");
				return false;
			}	
			receiveLength = recvfrom(newsockfd,buffer.get(),bufsize,0,(struct sockaddr *)&server_sockaddr,&slen); 
		}
		if(receiveLength == -1)
		{	puts(errorMsg.GetLastError());
//			printf("Received packet from %socketfd:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
			return false;
		}
		return true;
	}
	const char* GetString() const
	{	return buffer.get();
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
	virtual void OnPacket(bool /* isGood */,Packet&)
	{}
	virtual void OnStop() const
	{}
};

}

#endif
