// dlfcn.h 9/20/04
// Copyright 2004 Robin.Rowe@MovieEditor.com
// License OSI MIT

#ifndef DLFCN_H
#define DLFCN_H

#include "unistd.h"

#define dlopen(mLibName,unused) LoadLibrary(mLibName)
#define dlsym(mHandle,funcname) GetProcAddress((HMODULE)(mHandle),funcname)
#define dlclose(mHandle) FreeLibrary((HMODULE)(mHandle))
inline
char* dlerror() 
{	return 0;
}

#endif


