// CommandLine.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef CommandLine_h
#define CommandLine_h

#include <map>
#include <stdlib.h>

class CommandLine
{	std::map<std::string,std::string> data;
public:
	void Read(int argc,const char** argv);
	const char* Get(const char* key)
	{	const auto it = data.find(key);
		if(data.end()==it)
		{	return 0;
		}
		return &it->second;
	}
	bool Get(const char* key,int& i)
	{	const char* value = Get(key);
		if(!value)
		{	return false;
		}
		i = atoi(value);
		return errno != EINVAL;
	}
	bool IsKey(const char* key)
	{	return nullptr != Get(key);
	}
};

#endif
