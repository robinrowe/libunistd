// SqlStatement.cpp
// Copyright 2015/8/4 Robin.Rowe@cinepaint.org 

#include "SqlStatement.h"
#include <memory.h>

namespace sqlite {

void SqlStatement::GetEscapedString(vector<char>& v,const char* from,unsigned size)
{	if(!memchr(from,'\\',size) && !memchr(from,0,size))
    {   v.resize(size+2+1);//need extra 2 bytes for bracketing ''
        char* to=&v[0];
        *to='\'';
        to++;
        memcpy(to,from,size);
        to+=size;
        *to='\'';
        to++;
        *to=0;
        return;
    }
#if 0
    v.resize(2*size+1+2);//need extra 2 bytes for bracketing ''
    char* to=&v[0];
//	unsigned long adjustSize=mysql_real_escape_string(mysql, to, from, size);
	*to++='\'';
	for(unsigned i=0;i<size;i++)
	{	const char c=from[i];
		switch(c)
		{case '\\':
		case '\'':
			*to++='\\';
			break;	
		case 0:
			*to++='\\';
			*to++='0';
			continue;
		}
		*to++=c;
	}
	*to++='\'';
	*to=0;
#endif
}

bool SqlStatement::setBareString(const char* data)
{	// Exec blah ? ? ?
	const int i=GetOffset('?');
	if(i<0)
	{	return false;
	}
    statement.replace(i,1,data);
	return true;
}

bool SqlStatement::setString(const char* data,unsigned len)
{	// Exec blah ? ? ?
	const int i=GetOffset('?');
	if(i<0)
	{	return false;
	}
	vector<char> v;
    GetEscapedString(v,data,len);
    const char* escaped=&v[0];
	statement.replace(i,1,escaped);
	return true;
}

bool SqlStatement::setByte(char c)
{	// Exec blah ? ? ?
    const int i=(int) statement.find("?");//bug
	if(i<0)
	{	return false;
	}
	char escaped[5];
	char* p=escaped;
	*p='\'';
	p++;
	if(c=='\\' || c=='\'' || c==0)
	{	*p='\\';
		p++;
	}
	*p=c;
	p++;
	*p='\'';
	p++;
	*p=0;
	statement.replace(i,1,escaped);
	return true;
}

bool SqlStatement::setBinary(const char* data,unsigned size)
{	// Exec blah ? ? ?
    const int i=(int) statement.find("?");//bug
	if(i<0)
	{	return false;
	}
	vector<char> v;
	GetEscapedString(v,data,size);
	const char* escaped=&v[0];
	statement.replace(i,1,escaped);
	return true;
}

bool SqlStatement::setVarbinary(vector<char>& bytes,int key)
{	// Exec blah ? ? ?
    const int var1=(int) statement.find("?");//bug
	if(var1<0)
	{	return false;
	}
    const int var2=(int) statement.find("?",var1+1);//bug
	if(var2<0)
	{	return false;
	}
    portable::Int x(key);
    statement.replace(var2,1,x.toString());
	vector<char> v;
    GetEscapedString(v,&bytes[0],(unsigned int) bytes.size());
	const char* escaped=&v[0];
	statement.replace(var1,1,escaped);
	return true;
}

}
