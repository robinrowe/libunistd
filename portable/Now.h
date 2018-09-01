// Now.h

#ifndef Now_h
#define Now_h

#include <time.h>

#pragma warning(disable:4996)

class Now
{	time_t t;
public:
	Now()
	{	Sync();
	}
	void Sync()
	{	t=time(NULL);
	}
	bool GetDateTime(char* buffer,size_t size,const char* format = 0) 
	{	struct tm* tmp;
		// the current time as 'YYYY-MM-DD HH:mm:SS'
//		tzset();
		tmp = localtime(&t);
		if(!tmp)
		{	buffer[0]=0;
			return false;
		}
		if(!format)
		{	format = "%a, %d %b %Y %T";
		}
		if(!strftime(buffer,size-1,format,tmp))
		{	return false;
		}
		return true;
	}
};

class NowString
:	public Now
{	const static int bufsize = 32;
	char buffer[bufsize];
public:
	NowString()
	{}
	const char* GetDateTime() 
	{	Now::GetDateTime(buffer,bufsize);
		return buffer;
	}
	const char* GetTime()
	{	Now::GetDateTime(buffer,bufsize,"%H:%M:%S.%f");
		return buffer;
	}
};

#pragma warning(default:4996)
#endif
