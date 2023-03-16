/* libgen.h: replaces *nix header of same name
// Windows emulation of common *nix functions
// Copyright 2015/6/10 Robin.Rowe@MovieEditor.com
// License MIT (http://opensource.org/licenses/mit-license.php)
*/

#ifndef libgen_h
#define libgen_h

#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#else
//#define inline __inline
#endif

inline
const char* dirname(char *path)
{	char drive[_MAX_DRIVE] = { 0 };
	char dir[_MAX_DIR] = { 0 };
	errno_t error = _splitpath_s(path, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
	if (error)
	{	return NULL;
	}
	char result[_MAX_PATH] = { 0 };
	error = _makepath_s(result, drive, dir, NULL, NULL);
	if (error)
	{	return NULL;
	}
	// Modifying path in-place to match GNU implementation's behavior.
	error = strncpy_s(path, strlen(path) + 1, result, _MAX_PATH);
	if (error && error != STRUNCATE)
	{	return NULL;
	}
	return path;
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