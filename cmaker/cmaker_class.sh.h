// CLASS.h 
// Created by AUTHOR DATE
// License LICENSE

#ifndef CLASS_h
#define CLASS_h

#include <iostream>

class CLASS
{	CLASS(const CLASS&) = delete;
	void operator=(const CLASS&) = delete;

public:
	std::ostream& Print(std::ostream& os) const;
	~CLASS()
	{}
	CLASS()
	{}
	bool operator!() const
	{	return true;
	}
};

inline
std::ostream& operator<<(std::ostream& os,const CLASS& OBJECT)
{	return OBJECT.Print(os);
}

#endif
