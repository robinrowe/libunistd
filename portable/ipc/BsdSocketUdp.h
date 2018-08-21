// portable/BsdSocketUdp.h
// Created by Robin Rowe on 2018/4/4
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketUdp_h
#define BsdSocketUdp_h

#include <vector>
#include "BsdSocket.h"

namespace portable 
{

class BsdSocketUdp
:	public BsdSocket
{	
public:
	std::vector<char> v;
	BsdSocketUdp(size_t bufsize)
	:	v(bufsize)
	{	Clear();
	}
	virtual int OpenSocket()
	{	return (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	bool operator!() const
	{	return !IsOpen();
	}
	void Clear()
	{	if(!v.size())
		{	return;
		}
		v[0] = 0;
		v[v.size()-1] = 0;
	}
	bool Receive()
	{	const int bufsize = (int) v.size();
		const int bytes = RecvFrom(&v[0],bufsize,0);
		if(bytes<=0)
		{	v[0] = 0;
			return false;
		}
		if(bytes >= bufsize)
		{	v[bufsize-1] = 0;
			return bufsize;
		}
		v[bytes] = 0;
		return bytes;
	}
	operator const char*() const
	{	return &v[0];
	}
};

}

#endif
