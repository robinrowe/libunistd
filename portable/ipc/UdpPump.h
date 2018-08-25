// portable/UdpPump.h
// Copyright 2018/08/17 Robin.Rowe@Cinepaint.org
// License open source MIT

#ifndef UdpPump_h
#define UdpPump_h

#include <unistd.h>
#include <string>
#include <string.h>
#include <vector>
#include "../pump/Pump.h"
#include "../SystemLog.h"
#include "BsdSocketUdp.h"

namespace portable {

class UdpPump
:	public Pump
{	BsdSocketUdp udpServer;
	ssize_t bytesRead;
	bool isVerbose;
protected:
	bool Receive() override
	{	bytesRead = udpServer.Receive();
		if(bytesRead < 0)
		{	bytesRead = 0;
			TRACE(0);
			return false;
		}
		return true;
	}
public:
	~UdpPump()
	{	Close();
	}
	UdpPump(const char* pumpName,unsigned bufsize)
	:	Pump(pumpName)
	,	udpServer(bufsize)
	,	bytesRead(0)
	,	isVerbose(false)
	{}
	void SetVerbose(bool isVerbose = true)
	{	this->isVerbose = isVerbose;
	}
	bool Open(const char* serverName,unsigned port)
	{	if(!udpServer.Open(serverName,port))
		{	TRACE(0);
			return false;
		}
		if(!udpServer.Bind())
		{	TRACE(0);
			return false;
		}
		return true;
	}
	bool operator!() const
	{	return UdpPump::operator!();
	}
	void Close()
	{	udpServer.Close();
	}
	bool Send(const char* msg)
	{	return udpServer.Send(msg);
	}
	bool Send(const char* command,const char* data)
	{	char* msg = &udpServer.v[0];
		strlcpy(msg,command,udpServer.v.size()-1);
		strlcpy(msg+strlen(command),data,strlen(msg));
		return Send(msg);
	}
	bool SendReply(const char* data)
	{	char* msg = &udpServer.v[0];
		char* p = strchr(msg,' ');
		if(!p)
		{	return false;
		}
		strlcpy(p+1,data,udpServer.v.size()-1 - (p - msg));
		return Send(msg);
	}
	unsigned BytesRead() const
	{	return bytesRead;
	}
	const char* c_str() const
	{	return &udpServer.v[0];
	}
	char* c_str()
	{	return &udpServer.v[0];
	}
	bool IsCommand(const char* cmd) const
	{	const size_t length = strlen(cmd);
		const int isDifferent = memcmp(&udpServer.v[0],cmd,length);
		return !isDifferent;
	}
};

}

#if 0

#include <iostream>
using namespace std;
const char* MSG_STOP="quit";

int main()
{	UdpPump queue;
	if(!queue.Open("/test_queue",O_RDWR,1024))
	{	perror("mq_open");
		return 1;
	}
	cout << "Send to server (enter \"quit\" to stop it):\n";
	while(queue != MSG_STOP)
	{	cout<< "> ";
		cin.getline(buffer,MAX_SIZE);
		int ok = mq_send(mq, buffer, strlen(buffer)+1, 0);
		if(ok<0)
		{	perror("mq_send");
			return 2;
	}	}
	return 0;
}

#endif
#endif