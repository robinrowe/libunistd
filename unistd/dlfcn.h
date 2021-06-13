// dlfcn.h 9/20/04
// Copyright 2004 Robin.Rowe@MovieEditor.com
// License OSI MIT

#ifndef DLFCN_H
#define DLFCN_H

#include "unistd.h"

#if 0
#define dlopen(mLibName,unused) LoadLibrary(mLibName)
#define dlsym(mHandle,funcname) GetProcAddress((HMODULE)(mHandle),funcname)
#define dlclose(mHandle) FreeLibrary((HMODULE)(mHandle))
inline
char* dlerror() 
{	return 0;
}
#endif

enum 
{	RTLD_LAZY,
	RTLD_NOW,
	RTLD_GLOBAL,
	RTLD_LOCAL,
	RTLD_NODELETE,
	RTLD_NOLOAD,
	RTLD_DEEPBIND
};

inline
void* dlopen(const char* filename, int flag)
{	return LoadLibraryA(filename);
}

inline
const char* dlerror(void)
{	enum { len = 60 };
	static char msg[len];
	msg[0] = 0;
	const int wsaError = GetLastError();
	const DWORD num = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		wsaError,
		0,//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&msg,
		len - 1,
		NULL);
	msg[len - 1] = 0;
	return msg;
}

inline
void* dlsym(void* handle, const char* function)
{	return GetProcAddress((HMODULE) handle, function);
}

inline
int dlclose(void* handle)
{	const int success = 0;
	const int fail = -1;
	if(!FreeLibrary((HMODULE)(handle)))
	{	return fail;
	}
	return success;
}
	
#endif


