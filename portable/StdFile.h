// StdFile.h
// Created by Robin Rowe on 12/5/2015
// Libunistd Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef StdFile_h
#define StdFile_h

#ifdef _WIN32
#include "../vcpp/unistd.h"
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <vector>

namespace portable
{

class StdFile
{protected:
	FILE* fp;
	size_t bytes;
public:
	StdFile()
	:	fp(nullptr)
	,	bytes(0)
	{}
	~StdFile()
	{	Close();
	}
	FILE* GetFp()
	{	return fp;
	}
    size_t GetLastBytes() const
    {   return bytes;
    }
	bool IsGood() const
	{	return nullptr!=fp;
	}
	bool IsFeof()
	{	return IsGood()? 0!=feof(fp):false;
	}
/* flags:
"r"	read only: file must exist
"w"	write only: delete then create an empty 
"a"	append: create file if doesn't exist, write at end, no seek
"r+" read/write: existing file only
"w+" read/write: delete and create empty file
"a+" same as w+ but only write to end and write repositions at end
"rb" read binary...
*/
#pragma warning (disable : 4996)
	bool Open(const char* filename,const char* flags)
	{	fp = fopen(filename,flags);
		return IsGood();
	}
#pragma warning (default : 4996)
	bool OpenReadOnly(const char* filename)
	{	return Open(filename,"rb");
	}
	bool OpenWriteOnly(const char* filename)
	{	return Open(filename,"wb");
	}
	bool OpenAppend(const char* filename)
	{	return Open(filename,"ab");
	}
	bool Read(char* data,size_t length)
	{	if(!IsGood())
		{	return false;
		}
		bytes=fread(data,1,length,fp);
		return bytes>0;
	}
	void Skip(unsigned charCount)
	{	for(unsigned i=0;i<charCount && IsGood();i++)
		{	(void) getc(fp);
			bytes++;
	}	}	
	void SkipLine()
	{	if(!IsGood())
		{	return;
		}
		for(;;)
		{	int ch = getc(fp);
			bytes++;
			if (ch == '\n' || ch == EOF)
			{	return;
		}	}
	}	
	bool GetLine(char* s,unsigned size)
	{	if(!IsGood())
		{	return false;
		}
		return nullptr != fgets(s,size,fp);
	}
	bool GetLine(std::vector<char>& line)
	{	return GetLine(&line[0],line.size());
	}
	bool Write(const char* data,size_t length)
	{	if(!data || !IsGood())
		{	return false;
		}
		bytes=fwrite(data,1,length,fp);
		return bytes==length;
	}
	bool Write(char c)
	{	return Write(&c,1);
	}
	bool Write(const char* string)
	{	if(!string)
		{	return false;
		}
		const size_t length=strlen(string);	
		return Write(string,length);
	}
	bool WriteNull()
	{	return Write("",1);
	}
	void Close()
	{	if(IsGood())
		{	fclose(fp);
		}
		fp=nullptr;
	}
	bool Seek(long offset)
	{	if(!IsGood())
		{	return false;
		}
		const int err=fseek(fp,0,offset);
		if(err)
		{	return false;
		}
		return true;
	}
	bool SeekEnd()
	{	return Seek(SEEK_END);
	}
	long Tell() const
	{	if(!IsGood())
		{	return false;
		}
		const long size=ftell(fp);
		return size;
	}
	void Rewind()
	{	if(IsGood())
		{	rewind(fp);
	}	}
#ifdef __GNUC__
	int fscanf(const char* const format,...) __attribute__ ((format(scanf, 2, 3)))
#else
	int fscanf(const char* const format,...)
#endif
	{	va_list argList;
		va_start(argList,format);
		bytes = vfscanf(fp, format, argList);
		va_end(argList);
		return bytes;
	}
};

// Slurp StdFile or StdDevice:

template <typename T>
bool Slurp(T& file,std::vector<char>& buffer)
{	if(!file.SeekEnd())
	{	return false;
	}
	const long size=file.Tell();
	file.Rewind();
	buffer.resize(size);
	char* data=&buffer[0];
	const auto bytes=file.Read(data,size);
	return size==bytes;
}

template <typename T>
bool Slurp(T& file,std::vector<std::string>& line)
{	std::vector<char> buffer;
	if(!Slurp(file,buffer))
	{	return false;
	}
	const char* p = &buffer[0];
	const char* end = p+buffer.size();
	const char* word = p;
	while(p<end)
	{	switch(*p)
		{default:
			break;
		case '\r':
		case '\n':
			line.push_back(std::string(word,p-word));// not std::move(), "error: moving a temporary object prevents copy elision"

			if('\r'==*p)
			{	word=p+2;
			}
			else
			{	word=p+1;
			}			
		}
		p++;
	}
	return true;
}

inline
bool IsFile(const char* filename)
{
#ifdef WIN32	
	struct __stat64 st;
	const int err = _stat64(filename, &st);
#else
	struct stat st;
	const int err = stat(filename, &st);
#endif
	if(err!=0)
	{	return false;
	}
	return 0!=S_ISREG(st.st_mode);
}

inline
bool IsDir(const char* path)
{
#ifdef WIN32	
	struct __stat64 st;
	const int err = _stat64(path, &st);
#else
	struct stat st;
	const int err = stat(path, &st);
#endif
	if(err!=0)
	{	return false;
	}
	return 0!=S_ISDIR(st.st_mode);
}

inline
long long GetFileSize(const char* filename)
{
#ifdef WIN32	
	struct __stat64 st;
	const int err = _stat64(filename, &st);
#else
	struct stat st;
	const int err = stat(filename, &st);
#endif
	if(err!=0)
	{	return -1;
	}
	return st.st_size;
}

inline
bool DeleteFile(const char* filename)
{	
#ifdef WIN32	
	const int err=_unlink(filename);
#else
	const int err=unlink(filename);
#endif
	return err!=0;
}

}

#endif
