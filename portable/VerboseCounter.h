// VerboseCounter.h
// Created by Robin Rowe on 8/26/2016
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef VerboseCounter_h
#define VerboseCounter_h

namespace portable
{

class VerboseCounter
{	unsigned counter;
	unsigned maxMod;
public:
	VerboseCounter(unsigned maxMod)
	:	counter(0)
	,	maxMod(maxMod)
	{}
	VerboseCounter& operator++(int)
	{	counter++;
		return *this;
	}
	operator bool() const
	{	return !(counter % maxMod);
	}
};

}
#endif