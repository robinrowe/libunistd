// portable/BsdSocketTcp.h
// Created by Robin Rowe on 2018/4/4
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketTcp_h
#define BsdSocketTcp_h

#include "BsdSocket.h"

namespace portable 
{

class BsdSocketTcp
:	public BsdSocket
{public:
	virtual int OpenSocket()
	{	return (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	bool operator!() const
	{	return !IsGood();
	}
};

}

}

#endif
