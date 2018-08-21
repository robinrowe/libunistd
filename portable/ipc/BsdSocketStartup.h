// portable/BsdSocketStartup.h
// Created by Robin Rowe on 11/27/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef BsdSocketStartup_h
#define BsdSocketStartup_h

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
{
public:
	BsdSocketStartup()
	{
#ifdef _WIN32
		WORD version_requested=MAKEWORD(2,0);
		WSADATA data;
		WSAStartup(version_requested,&data);
#endif
	}
	~BsdSocketStartup()
	{	
#ifdef _WIN32
		WSACleanup();
#endif
	}
};

}

#endif
