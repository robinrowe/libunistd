// PosixQueue.h
// Copyright 2018/06/29 Robin.Rowe@Cinepaint.org
// License open source MIT

#ifndef PosixQueue_h
#define PosixQueue_h

#include <string>
#include <string.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>

namespace portable {

class PosixQueue
{	std::string name;
	std::vector<char> v;
	mqd_t mq;
	mq_attr attr;
	ssize_t bytesRead;
public:
	~PosixQueue()
	{	Close();
	}
	PosixQueue()
	:	mq(0)
	,	bytesRead(0)
	{	memset(&attr,sizeof(attr),0);
	}
	void Drop()
	{	mq_unlink(name.c_str());
		name.clear();
	}
	void SetFlags(int flags)
	{	attr.mq_flags = flags;
	}
	void SetMaxMsg(int maxmsg)
	{	attr.mq_maxmsg = maxmsg;
	}
	void SetMaxSize(int msgsize)
	{	attr.mq_msgsize = msgsize;
	}
	void SetCurMsgs(int curmsgs)
	{	attr.mq_curmsgs = curmsgs;
	}
//	O_RDONLY, O_WRONLY, O_RDWR, O_CREAT
	bool Open(const char* name,int oflag,size_t bufsize,mode_t mode=0644,bool isAttr=false)
	{	mq_attr* p = 0;
		if(isAttr)
		{	p = &attr;
		}
		mq = mq_open(name,oflag,mode,p);
		if(mq<0)
		{	return false;
		}
		v.resize(bufsize);
		v[0] = 0;
		v[bufsize-1] = 0;
		return true;
	}
	bool operator!() const
	{	return mq<=0;
	}
	void Close()
	{	if(!*this)
		{	return;
		}
		mq_close(mq);
		mq = 0;
	}
	bool IsEqual(const char* s,size_t len) const
	{	if(len>name.length())
		{	len = name.length();
		}
		return !strncmp(&v[0],s,len);
	}
	bool operator==(const char* s) const
	{	return IsEqual(s,strlen(s));
	}
	bool operator!=(const char* s) const
	{	return !IsEqual(s,strlen(s));
	}
	bool Send(const char* msg)
	{	const int ok = mq_send(mq,msg,strlen(msg)+1,0);
		if(ok<0)
		{	return false;
		}
		return true;
	}
	bool Receive()
	{	bytesRead = mq_receive(mq,&v[0],v.size()-1, NULL);
		if(bytesRead < 0)
		{	bytesRead = 0;
			return false;
		}
		if(0 != v[bytesRead-1])
		{	v[bytesRead] = 0;
		}
		return true;
	}
	unsigned BytesRead() const
	{	return bytesRead;
	}
	operator const char*() const
	{	return &v[0];
	}
};

}

#if 0

#include <iostream>
using namespace std;
const char* MSG_STOP="quit";

int main()
{	PosixQueue queue;
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