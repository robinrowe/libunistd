// SqlStatement.h
// Copyright 2015/8/4 Robin.Rowe@cinepaint.org

#ifndef SqlStatement_h
#define SqlStatement_h

//#include "../libportable/Epoch.h"
//#include "../libportable/DateTime.h"
//#include "../libportable/BigDecimal.h"
//#include "../portable/CppTypes.h"
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <string.h>
using namespace std;

namespace sqlite {

class SqlStatement
{	std::string statement;
    int GetOffset(char c) const
    {	const char* p=strchr(statement.c_str(),c);
        if(!p)
        {	return -1;
        }
        return (int) (p-statement.c_str());
    }
    void GetEscapedString(vector<char>& v,const char* from,unsigned size);
    bool GetEscapedString(vector<char>& v,const char* from)
    {	if(!from)
        {	return false;
        }
        const unsigned len=unsigned(strlen(from));
        GetEscapedString(v,from,len);
        return true;
    }
public:
    operator const char*() const
    {   return statement.c_str();
    }
    SqlStatement(const char* sql)
    {	statement=sql;
    }
    bool setBareString(const char* data);
    bool setString(const char* data,unsigned size);
    bool setString(const char* data)
    {   if(!data)
        {   return false;
        }
        const unsigned int len=(unsigned int) strlen(data);
        return setString(data,len);
    }
    bool setDouble(double v)
    {	portable::Double data(v);
        return setString(data);
	}
    bool setByte(char data);
    bool setVector(vector<char>& bytes)
    {	return setBinary(&bytes[0],(unsigned int) bytes.size());
	}
	bool setVarbinary(vector<char>& bytes,int key);
    bool setBinary(const char* data,unsigned size);
#if 0
    bool setDate(const Date& date)
	{	const String s(date.toString());
		if(!s.size())
        {	return setBareString("null");
		}
        return setString(s);
	}
	bool setNow(int col)
	{	Epoch now(isNow);
        return setEpoch(now);
	}
    bool setEpoch(const Epoch& epoch)
    {	return setString( epoch.toSqlString());
	}
    bool setTimestamp(Timestamp t)
	{	Epoch epoch(t);
        return setString(epoch.toString());
	}
    void setBigDecimal(const BigDecimal amount)
    {	setBareString(amount.toString());
	}
#endif
    void setUnsignedBigInt(unsigned long long x)
    {	portable::UnsignedLongLong data(x);
        setBareString(data);
	}
    void setBigInt(long long x)
    {	portable::LongLong data(x);
        setBareString(data);
	}
    void setInt(int x)
    {	portable::Int data(x);
        setBareString(data);
	}
    void setUnsigned(unsigned x)
    {	portable::Unsigned data(x);
        setBareString(data);
	}
    void setBoolean(int x)
	{	int tf=1;
		if(x=='F' || x=='f' || x==0)
		{	tf=0;
		}
        setInt(tf);
	}
	bool setBool(int,bool tf)
	{	const int i=GetOffset('?');
		if(i<0)
		{	return false;
		}
		const char* c=tf? "'T'":"'F'";//SQL needs ' marks.
		statement.replace(i,1,c);
		return true;
	}
};

}

#endif
 
