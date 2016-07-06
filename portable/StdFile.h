// StdFile.h
// Created by Robin Rowe on 12/5/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT

#ifndef StdFile_h
#define StdFile_h

#include <stdio.h>
#include <sys/stat.h>
#include <vector>

namespace portable
{

class StdFile
{protected:
	FILE* fp;
	size_t bytes;
public:
	StdFile()
	:	fp(0)
	,	bytes(0)
	{}
	~StdFile()
	{	Close();
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
		return 0!=fp;
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
	int Read(char* data,size_t length)
	{	if(!fp)
		{	return 0;
		}
		bytes=fread(data,1,length,fp);
		return bytes;
	}
	int Write(const char* data,size_t length)
	{	if(!data || !fp)
		{	return false;
		}
		bytes=fwrite(data,1,length,fp);
		return bytes==length;
	}
	int Write(char c)
	{	return Write(&c,1);
	}
	int Write(const char* string)
	{	if(!string)
		{	return false;
		}
		const size_t length=strlen(string);	
		return Write(string,length);
	}
	int WriteNull()
	{	return Write("",1);
	}
	void Close()
	{	if(fp>0)
		{	fclose(fp);
		}
		fp=0;
	}
	bool Seek(long offset)
	{	if(!IsGood())
		{	return false;
		}
		const int err=fseek(fd,0,offset);
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
		const long size=ftell(fd);
		return size;
	}
	void Rewind()
	{	if(IsGood())
		{	rewind(fd);
	}	}
};

template <typename T>
bool Slurp(T& file,std::vector<char>& buffer)
{	if(!file.SeekEnd())
	{	return false;
	}
	const long size=file.Tell();
	file.Rewind();
	buffer.resize(size);
	char* data=&buffer[0];
	bytes=file.Read(data,size);
	return size==bytes;
}

template <typename T>
bool Slurp(T& file,std::vector<std::string>& line)
{	std::vector<char> buffer;
	if(!Slurp(buffer))
	{	return false;
	}
	const char* p = &buffer[0];
	const char* end = p+bytes;
	const char* word = p;
	while(p<end)
	{	switch(*p)
		{default:
			break;
		case '\r':
		case '\n':
			line.push_back(std::move(std::string(word,p-word)));
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
	return true;
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
	return err!=0;
#else
	const int err=unlink(filename);
	return err!=0;
#endif
}

}

#endif
