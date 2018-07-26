// VBuffer.h
// Copyright 2018/7/25 Robin.Rowe@cinepaint.org
// License open source MIT/BSD

#ifndef VBuffer_h
#define VBuffer_h

#include <vector>

namespace portable 
{

class VBuffer
:	public std::vector<char>
{	
public:
	VBuffer()
	{}
	VBuffer(size_t bufsize)
	{	resize(bufsize);
	}
	operator char*()
	{	return &(*this)[0];
	}
	operator const char*() const
	{	return &(*this)[0];
	}
};

}

#endif
