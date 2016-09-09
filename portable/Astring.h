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
	{	data()[0] = 0;
	}
	bool empty() const
	{	return 0 == data()[0];
	}
	size_t size() const 
	{	return strlen(data());
	}
	size_t length() const 
	{	return size();
	}
	size_t capacity() const 
	{	return SIZE;
	}
	const char* c_str() const
	{	return data();
	}
	void ends(size_t len = SIZE)
	{	if(len > SIZE)
		{	len = SIZE;
		}
		data()[len] = 0;
	}
	Astring& append(const char* s)
	{	strncat(data(),s,SIZE);
		ends();
		return *this;
	}
	Astring& append(const Astring& s)
	{	return append(s.data());
	}
	Astring& operator+=(const char* s)
	{	return append(s);
	}
	Astring& operator+=(const Astring& s)
	{	return append(s);
	}
	Astring& assign(const char* s)
	{	strncpy(data(),s,SIZE);
		ends();
		return *this;
	}
	Astring& assign(const char* s,size_t pos)
	{	return assign(s+pos);
	}
	Astring& assign(const char* s,size_t pos,size_t count)
	{	s+=pos;
		const size_t len = (count <= SIZE) ? count:SIZE;
		strncpy(data(),s,len);
		ends(len);
		return *this;
	}
	Astring& assign(const Astring& s,size_t pos,size_t count)
	{	return assign(s.data(),pos,count);
	}
	Astring& assign(const Astring& s)
	{	return assign(s.data());
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
		return data()[pos];
	}
	const char* operator[](size_t pos) const
	{	if(IsOverflow(pos))
		{	return 0;
		}
		return data()[pos];
	}
	bool operator==(const char* s) const
	{	if(!s)
		{	return false;
		}
		return !strcmp(data(),s);
	}
	bool operator==(const Astring& s) const
	{	return *this == s.data();
	}
	bool operator!=(const char* s) const
	{	if(!s)
		{	return true;
		}
		return 0 != strcmp(data(),s);
	}
	bool operator!=(const Astring& s) const
	{	return *this != s.data();
	}

};

}

#pragma warning(default:4996)

#endif
