// Substring.h
// Copyright 2018/7/25 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef Substring_h
#define Substring_h

#include <string>

namespace portable 
{

class Substring
{public:
	char* text;
	unsigned length;	
	Substring()
	:	text(0)
	,	length(0)
	{}
	void SetEnds(const char* ends)
	{	if(!ends)
		{	length = 0;
			return;
		}
		length = unsigned(ends - text);
	}
	operator std::string() const
	{	return std::string(text,length);
	}
};

}

#endif
