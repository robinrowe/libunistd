/* libgen.h: replaces *nix header of same name
// Windows emulation of common *nix functions
// Copyright 2015/6/10 Robin.Rowe@MovieEditor.com
// License MIT (http://opensource.org/licenses/mit-license.php)
*/

#ifndef libgen_h
#define libgen_h

#include <string.h>

#ifdef __cplusplus
extern "C" {
#else
#define inline __inline
#endif

inline
const char* dirname(char *path)
{	return "error";
}

inline 
const char* GetLastChar(const char* path, char sep)
{	const char* p=strchr(path,sep);	
	const char* prev=p;
	while(p)
	{	prev=p;
		p=strchr(p+1,sep);
	}
	return prev;
}

inline
const char* basename(char *path)
{	const char* last=GetLastChar(path,'/');
	if(!last)
	{	last=GetLastChar(path,'\\');
	}
	if(!last)
	{	return path;
	}
	return last+1;
}

#ifdef __cplusplus
}
#endif

#endif