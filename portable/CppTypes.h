// libportable/CppTypes.h
// 2014/11/24 Robin.Rowe@cinepaint.org
// License open source MIT

#ifndef CppTypes_h
#define CppTypes_h

#include <stdio.h>

namespace portable {

#ifdef WIN32
#define snprintf _snprintf
#endif

class Bool
{	bool tf;
public:
	Bool(const char* cs,bool def=false)
	:	tf(false)
	{	if(!cs)
		{	tf=def;
			return;
		}
		if(*cs=='t'||*cs=='T')
		{	tf=true;
	}	}
    const char* toString() const
    {   return tf? "T":"F";
    }
	operator bool() const
	{	return tf;
}	};

#pragma warning (disable:4996)

template<typename T>
size_t StringToType(const char* cs,const char* formatting,T& t)
{	const size_t size=sscanf(cs,formatting,&t);
	return size;
}

template<typename T>
size_t TypeToString(char* buffer,size_t bufsize,const char* formatting,const T& t)
{   const size_t size=snprintf(buffer,bufsize,formatting,t);
    if(size>0)
    {   buffer[size]=0;
    }
    return size;
}

#pragma warning (default:4996)

template<typename T>
class CppInBuffer
{	
public:
	CppInBuffer(const char* cs,short& x)
	{	StringToType(cs,"%hd",x);
	}
	CppInBuffer(const char* cs,unsigned short& x)
	{	StringToType(cs,"%hu",x);
	}
	CppInBuffer(const char* cs,int& x)
	{	StringToType(cs,"%d",x);
	}
	CppInBuffer(const char* cs,unsigned int& x)
	{	StringToType(cs,"%u",x);
	}
	CppInBuffer(const char* cs,long& x)
	{	StringToType(cs,"%ld",x);
	}
	CppInBuffer(const char* cs,unsigned long& x)
	{	StringToType(cs,"%lu",x);
	}
	CppInBuffer(const char* cs,long long& x)
	{	StringToType(cs,"%lld",x);
	}
	CppInBuffer(const char* cs,unsigned long long& x)
	{	StringToType(cs,"%llu",x);
	}
	CppInBuffer(const char* cs,float& x)
	{	StringToType(cs,"%f",x);
	}
	CppInBuffer(const char* cs,double& x)
	{	StringToType(cs,"%lf",x);
	}
};

template<typename T>
class CppOutBuffer
{
public:
    CppOutBuffer(char* buffer,unsigned size,short& x)
    {	TypeToString(buffer,size,"%hd",x);
    }
    CppOutBuffer(char* buffer,unsigned size,unsigned short& x)
    {	TypeToString(buffer,size,"%hu",x);
    }
    CppOutBuffer(char* buffer,unsigned size,int& x)
    {	TypeToString(buffer,size,"%d",x);
    }
    CppOutBuffer(char* buffer,unsigned size,unsigned int& x)
    {	TypeToString(buffer,size,"%u",x);
    }
    CppOutBuffer(char* buffer,unsigned size,long& x)
    {	TypeToString(buffer,size,"%ld",x);
    }
    CppOutBuffer(char* buffer,unsigned size,unsigned long& x)
    {	TypeToString(buffer,size,"%lu",x);
    }
    CppOutBuffer(char* buffer,unsigned size,long long& x)
    {	TypeToString(buffer,size,"%lld",x);
    }
    CppOutBuffer(char* buffer,unsigned size,unsigned long long& x)
    {	TypeToString(buffer,size,"%llu",x);
    }
    CppOutBuffer(char* buffer,unsigned size,float& x)
    {	TypeToString(buffer,size,"%f",x);
    }
    CppOutBuffer(char* buffer,unsigned size,double& x)
    {	TypeToString(buffer,size,"%lf",x);
    }
};

template <typename T>
class CppType
{   T x;
//unsigned long long l2 = 18'446'744'073'709'550'592llu; // C++14
    enum {bufsize=28};
    char buffer[bufsize];
public:
    CppType<T>(T x)
    :	x(x)
    {}
    CppType<T>(const char* data)
    :	x(0)
    {	if(!data)
        {   return;
        }
        strncat(buffer,data,bufsize-1);
        CppInBuffer<T> temp(buffer,x);
    }
    const char* toString()
    {	if(!buffer[0])
        {   CppOutBuffer<T> temp(buffer,bufsize,x);
        }
        return buffer;
    }
    operator const char*()
    {   return toString();
    }
    operator T() const
    {	return x;
    }
	void SetSeparators(char comma=',')
	{	const size_t width=strlen(buffer);
		while(width>3)
		{	const size_t commas=width/3-1;
			buffer[buffsize-width-commas]=buffer[bufsize-width];
			if(1==width%3)
			{	buffer[buffsize-width-commas+1]=comma;
			}
			width--;
	}	}	
};

typedef CppType<short> Short;
typedef CppType<int> Int;
typedef CppType<unsigned> Unsigned;
typedef CppType<long long> LongLong;
typedef CppType<unsigned long long> UnsignedLongLong;
typedef CppType<float> Float;
typedef CppType<double> Double;

}

#endif
