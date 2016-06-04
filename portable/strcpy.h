// strcpy.h
// Copyright 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef strcpy_h
#define strcpy_h

#include <string.h>
#include <string>

inline
void strcpy(std::string& s, const char* s2)
{	s=s2;
}

inline
void strcpy(std::string& s, std::string& s2)
{	s=s2;
}

inline
void strcpy(std::string& s, const std::string& s2)
{	s=s2;
}

inline
const char* strstr(const std::string& s, const char* cs)
{	return strstr(s.c_str(),cs);
}

inline
const char* strstr(const std::string& s, const std::string& s2)
{	return strstr(s.c_str(),s2.c_str());
}

#endif
