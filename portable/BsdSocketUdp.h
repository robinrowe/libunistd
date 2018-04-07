// portable/BsdSocketUdp.h
// Created by Robin Rowe on 2018/4/4
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketUdp_h
#define BsdSocketUdp_h

#include "BsdSocket.h"

namespace portable 
{

class BsdSocketUdp
:	public BsdSocket
{
public:
	virtual int OpenSocket()
	{	return (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	bool operator!() const
	{	return !IsGood();
	}
};

}

#endif
