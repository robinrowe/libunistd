// Text.h
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef Text_h
#define Text_h

//#include <array>
#include <string.h>

#pragma warning(disable:4996)

namespace portable
{

template <unsigned width>
class Text
//:	public std::array<char,width+1>
{	char data[width+1];
	bool IsOverflow(size_t pos) const
	{	return pos > width;
	}
public:
	Text()
	{	clear();
	}
	Text(const char* s)
	{	assign(s);
	}
    Text(const char* s,size_t count)
    {   assign(s, 0, count);
    }
	Text(const char* s,size_t pos,size_t count)
	{	assign(s,pos,count);
	}
	Text(const Text& s)
	{	assign(s);
	}
	void clear()
	{	data[0] = 0;
	}
	bool empty() const
	{	return 0 == data[0];
	}
	size_t size() const 
	{	return strlen(data);
	}
	size_t length() const 
	{	return size();
	}
	size_t capacity() const 
	{	return width;
	}
	const char* c_str() const
	{	return this->data();
	}
	operator const char*() const
	{	return c_str();
	}
	void ends(size_t len = width)
	{	if(len > width)
		{	len = width;
		}
		data[len] = 0;
	}
	Text& append(const char* s)
	{	strncat(data,s,width);
		ends();
		return *this;
	}
	Text& append(const Text& s)
	{	return append(s.data);
	}
	Text& operator+=(const char* s)
	{	return append(s);
	}
	Text& operator+=(const Text& s)
	{	return append(s);
	}
	Text& assign(const char* s)
	{	strncpy(data,s,width);
		ends();
		return *this;
	}
	Text& assign(const char* s,size_t pos)
	{	return assign(s+pos);
	}
	Text& assign(const char* s,size_t pos,size_t count)
	{	s+=pos;
		const size_t len = (count <= width) ? count:width;
		strncpy(data,s,len);
		ends(len);
		return *this;
	}
	Text& assign(const Text& s,size_t pos,size_t count)
	{	return assign(s.data,pos,count);
	}
	Text& assign(const Text& s)
	{	return assign(s.data);
	}
	Text& operator=(const char* s)
	{	return assign(s);
	}
	Text& operator=(const Text& s)
	{	return assign(s);
	}
	char* operator[](size_t pos)
	{	if(IsOverflow(pos))
		{	return 0;
		}
		return data[pos];
	}
	const char* operator[](size_t pos) const
	{	if(IsOverflow(pos))
		{	return 0;
		}
		return data[pos];
	}
	bool operator==(const char* s) const
	{	if(!s)
		{	return false;
		}
		return !strcmp(data,s);
	}
	bool operator==(const Text& s) const
	{	return *this == s.data;
	}
	bool operator!=(const char* s) const
	{	if(!s)
		{	return true;
		}
		return 0 != strcmp(data,s);
	}
	bool operator!=(const Text& s) const
	{	return *this != s.data;
	}

};

}

#pragma warning(default:4996)

#endif
