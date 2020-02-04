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
{	static bool isSingleton;
public:
    BsdSocketStartup()
    {   if(!isSingleton)
        {  	WSADATA data; 
            const WORD version_requested = MAKEWORD(2, 0);
            WSAStartup(version_requested, &data);
            isSingleton = true;
    }   }
	~BsdSocketStartup()
	{	if(isSingleton)
        {   WSACleanup();
            isSingleton = false;
    }	}
};

}
#else
namespace portable 
{
	
class BsdSocketStartup
{	
public:
    BsdSocketStartup()
	{}
};

}

#endif

#endif
