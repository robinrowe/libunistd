// portable/Folder.h
// Copyright 2016/1/16 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Folder_h
#define Folder_h


#ifdef _WIN32
#include <ShlObj.h>
#endif

namespace portable 
{

void GetHomePath(std::string& folderPath,bool isForwardSlashes=true)
{
#ifdef _WIN32
	TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, path))) 
	{	folderPath= path;
		if(isForwardSlashes)
		{	std::replace(folderPath.begin(), folderPath.end(), '\\', '/');
	}	}
#else
	//bug
#endif
}

}

#endif