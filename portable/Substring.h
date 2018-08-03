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
	void SetEnd(const char* ends,int offset = 0)
	{	if(!ends)
		{	length = 0;
			return;
		}
		ends += offset;
		if(ends < text)
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
