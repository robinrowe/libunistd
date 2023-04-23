// FieldMap.h
// Copyright 2018/8/3 Robin.Rowe@cinepaint.org
// License MIT open source

#ifndef FieldMap_h
#define FieldMap_h

#include <map>
#include <string_view>
using namespace std;

template <class T> 
struct string_view_compare 
{	bool operator() (const T& x, const T& y) const 
	{	return x.compare(y)<0;
}	};

class FieldMap
{	char* p;
	std::string_view empty;
//	std::map<std::string_view,std::string_view,string_view_compare<std::string_view> > m;
	std::map<unsigned,std::string_view> m;
	std::string_view StripFieldname(std::string_view text) const
	{	if(text.empty())
		{	return text;
		}
		auto offset = text.find(": ");
		if(offset == text.npos)
		{	return text;
		}
		text.remove_prefix(offset+2);
		return text;
	}
public:
	FieldMap()
	{	p = nullptr;
	}
	void Clear()
	{	p=0;
		m.clear();
	}
	void Set(char* data)
	{	p = data;
	}
	std::string_view& Find(int key) 
	{	auto it = m.find(key);
		if(it == m.end())
		{	return empty;
		}
		return it->second;
	}
	const std::string_view& Find(int key) const 
	{	auto it = m.find(key);
		if(it == m.end())
		{	return empty;
		}
		return it->second;
	}
	std::string_view& operator[](int key)
	{	return Find(key);
	}
	std::string_view operator[](int key) const
	{	return Find(key);
	}
	bool AdvancePast(const char* text)
	{	char* seek = strstr(p,text);
		if(!seek)
		{	return false;
		}
		p = seek + 2;
		return true;	
	}
	char* SkipWhitespace()
	{	while(isspace(*p))
		{	p++;
		}
		return p;
	}
	char* Find(char c)
	{	return strchr(p,c);
	}
	bool IsInvalid(unsigned i) const
	{	return m.size()<=i;
	}
	string_view GetFieldData(unsigned i) 
	{	if(IsInvalid(i))
		{	return empty;
		}
		return m[i];
	}
#if 0
	string_view GetFieldData(unsigned i) const
	{	if(IsInvalid(i))
		{	return empty;
		}
		return m[i];
	}
#endif
	string_view operator[](unsigned i) 
	{	return StripFieldname(GetFieldData(i));
	}

	char* Parse(const char* fieldName,unsigned i)
	{	
		if(IsInvalid(i))
		{	return 0;
		}
		char* found = strstr(p,fieldName);
		if(!found)
		{	return 0;
		}
		m[i] = found;
		p = found + 1;
		char* ends = strchr(p,'\n');
		if(ends)
		{	*ends= 0;
			p = ends + 1;
		}
		return found;
	}

};

#endif
