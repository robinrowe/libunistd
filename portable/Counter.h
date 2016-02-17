// Counter.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef Counter_h
#define Counter_h

#include <string>

class Counter
{	unsigned count;
	std::string s;
public:
	Counter()
	:	count(0)
	{}
	const char* operator ()(const char* msg) 
	{	count++;
		s=std::to_string(count);
		s.append(": ");
		s.append(msg);
		return s.c_str();
	}
};

#endif
