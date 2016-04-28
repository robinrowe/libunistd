// homedir.h
// Copyright 2016 Robin.Rowe@cinepaint.org
// License open source MIT

#ifndef Homedir_h
#define Homedir_h

#ifdef _WIN32

#ifdef UNREAL_ENGINE
#include <AllowWindowsPlatformTypes.h>
#endif
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

inline
bool GetHomedir(std::string& path)
{	wchar_t wbuffer[MAX_PATH+1];
	if (SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, wbuffer) != S_OK)
	{	return false;
	}
	char buffer[MAX_PATH*4+1];
	const int buflen = WideCharToMultiByte(CP_UTF8, 0, wbuffer, lstrlenW(wbuffer), buffer, MAX_PATH*4, NULL, NULL);
	if (buflen <= 0)
	{	return false;
	}
	buffer[buflen] = 0;
	path = buffer;
	return true;
}

#ifdef UNREAL_ENGINE
#include <HideWindowsPlatformTypes.h>
#endif

#else

#include <stdlib.h>

inline
bool GetHomedir(std::string& path)
{	const char* dir = getenv("HOMEDIR");
	if(!dir)
	{	return false;
	}
	path = dir;
	return true;
}

#endif

#endif
