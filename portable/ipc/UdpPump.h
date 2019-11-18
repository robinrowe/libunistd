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
{	
protected:
	bool Receive() override
	{	if(!udpSocket.Receive())
		{	TRACE(0);
			return false;
		}
		return true;
	}
public:
	BsdSocketUdp udpSocket;
	~UdpPump()
	{	Close();
	}
	UdpPump(const char* pumpName,size_t bufsize,bool isVerbose = true)
	:	Pump(pumpName)
	,	udpSocket(bufsize)
	{	udpSocket.SetVerbose(isVerbose);
	}
	void SetTrace(bool isTrace = true)
	{	udpSocket.SetTrace(isTrace);
	}
	bool Open(const char* serverName,unsigned port)
	{	if(!udpSocket.Open(serverName,port))
		{	TRACE(0);
			return false;
		}
		return true;
	}
	bool Bind()
	{	if(!udpSocket.Bind())
		{	TRACE(0);
			return false;
		}
		return true;
	}
	bool Connect()
	{	if(!udpSocket.Connect())
		{	TRACE(0);
			return false;
		}
		return true;
	}
	bool operator!() const
	{	return UdpPump::operator!();
	}
	void Close()
	{	udpSocket.Close();
	}
	bool Send(const char* msg)
	{	return udpSocket.Send(msg);
	}
	bool Send(const char* command,const char* data)
	{	char* msg = &udpSocket.v[0];
		strlcpy(msg,command,udpSocket.v.size()-1);
		strlcpy(msg+strlen(command),data,strlen(msg));
		return Send(msg);
	}
	bool SendReply(const char* data)
	{	char* msg = &udpSocket.v[0];
		char* p = strchr(msg,' ');
		if(!p)
		{	return false;
		}
		strlcpy(p+1,data,udpSocket.v.size()-1 - (p - msg));
		return Send(msg);
	}
	unsigned BytesRead() const
	{	return udpSocket.BytesRead();
	}
	const char* c_str() const
	{	return &udpSocket.v[0];
	}
	char* c_str()
	{	return &udpSocket.v[0];
	}
	bool IsCommand(const char* cmd,size_t length) const
	{	const int isDifferent = memcmp(&udpSocket.v[0],cmd,length);
		return !isDifferent;
	}
	bool IsCommand(const char* cmd) const
	{	const size_t length = strlen(cmd);
		return IsCommand(cmd,length);
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