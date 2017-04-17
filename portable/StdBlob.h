// StdBlob.h
// Libunistd Copyright (c) 2016 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef StdBlob_h
#define StdBlob_h

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

namespace portable
{

class StdBlob
{protected:
	const char* data;
	size_t bytes;
	size_t offset;
public:
	StdBlob(const char* data,size_t bytes=0)
	:	data(data)
	,	bytes(bytes)
	,	offset(0)
	{	if(!bytes)
		{	bytes = strlen(data);
		}
	}
	~StdBlob()
	{}
	bool IsGood() const
	{	return nullptr!=data;
	}
	bool IsFeof()
	{	return IsGood()? offset>=bytes:false;
	}
	bool Open(const char*,const char*)
	{	return IsGood();
	}
	int Read(char* data,size_t length)
	{	if(!IsGood())
		{	return 0;
		}
		if(length+offset>=bytes)
		{	length = bytes - offset;
		}
		memcpy(data,this->data,length);
		offset += length;
		return (int) bytes;
	}
	void Skip(unsigned charCount)
	{	if(charCount+offset>=bytes)
		{	offset = bytes;
		}
		else
		{	offset += charCount;
	}	}	
	void SkipLine()
	{	if(!IsGood())
		{	return;
		}
		while(offset<bytes)
		{	int ch = data[offset];
			offset++;
			if (ch == '\n')
			{	return;
		}	}
	}	
	void GetLine(std::vector<char>& line)
	{	if(!IsGood())
		{	return;
		}
		for(unsigned i=0;i<line.size();i++)
		{	if (data[offset]== '\n')
			{	line[i] = 0;
				return;
			}
			else
			{	line[i] = data[offset];
			}
			offset++;
		}
		line[line.size()-1] = 0;
	}
	void Close()
	{}
	bool Seek(long offset)
	{	if(!IsGood())
		{	return false;
		}
		if(offset>=(long) bytes)
		{	return false;
		}
		this->offset = offset;
		return true;
	}
	bool SeekEnd()
	{	this->offset = bytes;
		return true;
	}
	long Tell() const
	{	if(!IsGood())
		{	return false;
		}
		return (long) offset;
	}
	void Rewind()
	{	if(IsGood())
		{	offset = 0;
	}	}
#ifdef __GNUC__
	int fscanf(const char* const format,...) __attribute__ ((format(scanf, 2, 3)))
	{	va_list args;
		va_start(args, format);
		int retval = vsscanf(data+offset,format, args);
		va_end(args);
		SkipLine();
		return retval;
	}
#else
	int fscanf(const char* const format,...)
	{	va_list args;
		va_start(args, format);
		int retval = vsscanf(data+offset,format, args);
		va_end(args);
		SkipLine();
		return retval;
	}
#endif
};

}
#endif
