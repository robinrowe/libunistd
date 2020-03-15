// LmdbError.h
// Robin Rowe 2020/3/14

#ifndef LmdbError_h
#define LmdbError_h

#include <string>
#include <map>
#include <iostream>

namespace lmdb {

class LmdbError
{	std::map<int,const char*> lookup;
	void MapSymbols();
	void Map(const char* msg,int errorCode)
	{	lookup[errorCode] = msg;
	}
	std::string unknown;
public:
	LmdbError()
	{	MapSymbols();
	}
	const char* GetErrorMsg(int errorCode)
	{	const auto it = lookup.find(errorCode);
		if (it == lookup.end())
		{	unknown = "Unknown LMDB error #";
			unknown += std::to_string(errorCode);
			return unknown.c_str();
		}
		return it->second;
	}
};

}

#endif
