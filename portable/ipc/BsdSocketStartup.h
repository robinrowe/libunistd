// portable/BsdSocketStartup.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketStartup_h
#define BsdSocketStartup_h

#ifdef _WIN32

#ifdef UNREAL_ENGINE
#include <AllowWindowsPlatformTypes.h>
#endif
#include <winsock2.h>
#ifdef UNREAL_ENGINE
#include <HideWindowsPlatformTypes.h>
#endif

namespace portable 
{
	
class BsdSocketStartup
{	static BsdSocketStartup startup;
	BsdSocketStartup()
	{	WORD version_requested=MAKEWORD(2,0);
		WSADATA data;
		WSAStartup(version_requested,&data);
	}
public:
	~BsdSocketStartup()
	{	WSACleanup();
	}
};

}
#endif

#endif
