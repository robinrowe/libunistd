// CommandLine.h
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef CommandLine_h
#define CommandLine_h

#include <map>
#include <stdlib.h>
#include <string.h>

namespace portable
{

class CommandLine
{	std::map<std::string,std::string> data;
	const char* last;
public:
	CommandLine()
	{	last="";
	}
	CommandLine(int argc,const char** argv)
	{	Set(argc,argv);
	}
	void Set(int argc,const char** argv);
    void Append(const char* keyval);
	const char* Get(const char* key) const
	{	const auto it = data.find(key);
		if(data.end()==it)
		{	return nullptr;
		}
		const char* value = it->second.c_str();
        return value;
	}
	bool Get(const char* key,int& i) const
	{	const char* value = Get(key);
		if(!value)
		{	return false;
		}
		i = atoi(value);
		return errno != EINVAL;
	}
	const char* GetLast() const
	{	return last;
	}
	bool IsKey(const char* key) const
	{	const auto it = data.find(key);
		return data.end()!=it;
	}
};

}

#endif
