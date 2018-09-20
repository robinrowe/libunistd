// portable/Db.h
// Robin.Rowe@Cinepaint.org
// 2015/8/3

#ifndef PortableDb_h
#define PortableDb_h

namespace portable {

class Db
{
public:
	typedef int (*DbCallback)(void*,int,char**,char**);
	virtual ~Db()
	{	Close();
	}
	virtual bool IsExist(const char* dbName) const = 0;
	virtual bool IsOpen() const = 0;
	virtual bool Open(const char* dbName) = 0;
	virtual void Close()
	{}
	virtual bool Exec(const char* ,DbCallback )
	{   return false;
	}
	bool IsFile(const char* filename) const
	{
	#ifdef WIN32
		struct __stat64 st;
		const int err = _stat64(filename, &st);
		if(err!=0)
		{	return false;
		}
		return true;
	#else
		struct stat st;
		const int err = stat(filename, &st);
		if(err!=0)
		{	return false;
		}
		return true;
	#endif
	}
	void DropFile(const char* filename)
	{   remove(filename);
	}
};

}

#endif
