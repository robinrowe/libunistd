// CommandLine.h
// Libunistd Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef CommandLine_h
#define CommandLine_h

#include <map>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace portable
{

class CommandLine
{	std::map<std::string,std::string> data;
	int argc;
	const char** argv;
public:
	CommandLine()
	{	argc = 0;
		argv = nullptr;
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
	const char* operator[](int i) const
	{	if(i<0)
		{	i = argc+i;
		}
		if(i>=argc)
		{	return "";
		}
		return argv[i];
	}
	bool IsKey(const char* key) const
	{	const auto it = data.find(key);
		return data.end()!=it;
	}
	std::string toString();
};

}

#endif
