// portable/BsdSocketServer.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketServer_h
#define BsdSocketServer_h

#include "PacketSocket.h"
#include "BsdSocketPool.h"

namespace portable 
{

class BsdSocketServer
:	public PacketSocket
{	
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
	{	SetAsyncMode();
	}
	unsigned GetConnectionCount() const
	{	return pool.GetCount();
	}
	bool Open(int serverPort,int maxStreams,bool isPacketRun=true);
	void Close() override
	{	Stop();
		PacketSocket::Close();
	}
	virtual bool Login(SOCKET fd)
	{	(void)fd;
		return false;// nobody can login, override function to set true
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
	virtual void OnDisonnect(SOCKET sock) const
	{	(void)sock;
	}
	virtual void PacketRun();
};

}
#endif
