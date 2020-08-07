// CLASS.h 
// Created by AUTHOR DATE
// LICENSE

#ifndef CLASS_h
#define CLASS_h

#include <iostream>

class CLASS
{	CLASS(const CLASS&) = delete;
	void operator=(const CLASS&) = delete;

public:
	~CLASS()
	{}
	CLASS()
	{}
	bool operator!() const
	{	// to-do
		return true;
	}
	std::ostream& Print(std::ostream& os) const;
	std::istream& Input(std::istream& is);
};

inline
std::ostream& operator<<(std::ostream& os,const CLASS& OBJECT)
{	return OBJECT.Print(os);
}


inline
std::istream& operator>>(std::istream& is,CLASS& OBJECT)
{	return OBJECT.Input(is);
}

#endif
