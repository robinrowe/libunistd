// Astring.h
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef Astring_h
#define Astring_h

#include <array>
#include <string.h>

#pragma warning(disable:4996)

namespace portable
{

template <unsigned SIZE>
class Astring
:	public std::array<char,SIZE+1>
{	bool IsOverflow(size_t pos)
	{	return pos > size;
	}
public:
	Astring()
	{	clear();
	}
	Astring(const char* s)
	{	assign(s);
	}
	Astring(const char* s,size_t pos,size_t count)
	{	assign(s,pos,count);
	}
	Astring(const Astring& s)
	{	assign(s);
	}
	void clear()
	{	this->data()[0] = 0;
	}
	bool empty() const
	{	return 0 == this->data()[0];
	}
	size_t size() const 
	{	return strlen(this->data());
	}
	size_t length() const 
	{	return size();
	}
	size_t capacity() const 
	{	return SIZE;
	}
	const char* c_str() const
	{	return this->data();
	}
	void ends(size_t len = SIZE)
	{	if(len > SIZE)
		{	len = SIZE;
		}
		this->data()[len] = 0;
	}
	Astring& append(const char* s)
	{	strncat(this->data(),s,SIZE);
		ends();
		return *this;
	}
	Astring& append(const Astring& s)
	{	return append(s.this->data());
	}
	Astring& operator+=(const char* s)
	{	return append(s);
	}
	Astring& operator+=(const Astring& s)
	{	return append(s);
	}
	Astring& assign(const char* s)
	{	strncpy(this->data(),s,SIZE);
		ends();
		return *this;
	}
	Astring& assign(const char* s,size_t pos)
	{	return assign(s+pos);
	}
	Astring& assign(const char* s,size_t pos,size_t count)
	{	s+=pos;
		const size_t len = (count <= SIZE) ? count:SIZE;
		strncpy(this->data(),s,len);
		ends(len);
		return *this;
	}
	Astring& assign(const Astring& s,size_t pos,size_t count)
	{	return assign(s.this->data(),pos,count);
	}
	Astring& assign(const Astring& s)
	{	return assign(s.this->data());
	}
	Astring& operator=(const char* s)
	{	return assign(s);
	}
	Astring& operator=(const Astring& s)
	{	return assign(s);
	}
	char* operator[](size_t pos)
	{	if(IsOverflow(pos))
		{	return 0;
		}
		return this->data()[pos];
	}
	const char* operator[](size_t pos) const
	{	if(IsOverflow(pos))
		{	return 0;
		}
		return this->data()[pos];
	}
	bool operator==(const char* s) const
	{	if(!s)
		{	return false;
		}
		return !strcmp(this->data(),s);
	}
	bool operator==(const Astring& s) const
	{	return *this == s.this->data();
	}
	bool operator!=(const char* s) const
	{	if(!s)
		{	return true;
		}
		return 0 != strcmp(this->data(),s);
	}
	bool operator!=(const Astring& s) const
	{	return *this != s.this->data();
	}

};

}

#pragma warning(default:4996)

#endif
