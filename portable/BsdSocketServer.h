// portable/BsdSocketServer.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketServer_h
#define BsdSocketServer_h

#include "BsdSocket.h"
#include "BsdSocketStartup.h"
#include "BsdSocketPool.h"

namespace portable 
{

class BsdSocketServer
:	public BsdSocket
{	BsdSocketStartup socketStartup;
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
	{	(void)slot;
		(void)fd;
		return false;
	}
	void Start() override
	{	listenWorker=std::thread(ListenMain,this);
		listenWorker.detach();
		if(isPacketRun)
		{	packetWorker=std::thread(PacketMain,this);
			packetWorker.detach();
	}	}
	virtual void ListenRun();
	virtual void OnConnect(SOCKET sock) const
	{	(void) sock;
	}
	virtual void PacketRun();
};

}
#endif
