// libsqlite/Sqlite.h
// Robin.Rowe@Cinepaint.org
// 2015/8/3

#ifndef SqliteDb_h
#define SqliteDb_h

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "source/sqlite3.h"
#include <string>
//#include <QDebug>

namespace sqlite {

class DbBase
{
public:
	typedef int (*DbCallback)(void*,int,char**,char**);
	virtual ~DbBase()
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
};

inline
bool IsFile(const char* filename)
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

inline
void DropFile(const char* filename)
{   remove(filename);
}

class Db
:	public DbBase
{	sqlite3* db;
	bool isOpen;
	const char* errorMsg;
	void SetErrorMsg()
	{	errorMsg=sqlite3_errmsg(db);
		status=errorMsg;
		//qDebug()<<"Error "<<errorMsg;
	}
public:
	std::string status;
	Db()
	:	db(0),
		errorMsg(0),
		isOpen(false)
	{}
	~Db()
	{   Close();
	}
	bool IsExist(const char* dbName) const
	{	return IsFile(dbName);
	}
	bool IsOpen() const
	{	return isOpen;
	}
	bool Open(const char* dbName)//"test.db"
	{	const int rc = sqlite3_open_v2(dbName, &db,SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,0);
		isOpen=(rc==SQLITE_OK);
		if(isOpen)
		{   status="Opened database ";
			status.append(dbName);
		}
		else
		{	SetErrorMsg();
		}
		//qDebug()<<status.c_str();
		return isOpen;
	}
	const char* GetErrorMsg() const
	{	return errorMsg;
	}
	void Close()
	{	if(isOpen)
		{	sqlite3_close(db);
			isOpen=false;
		}
	}
	bool Exec(const char* sql,DbCallback callback = 0)
	{	//qDebug()<<sql;
		const int rc = sqlite3_exec(db, sql, callback, this,(char**) &errorMsg);
		if(rc!=SQLITE_OK)
		{   SetErrorMsg();
			return false;
		}
		return true;
	}
	std::string GetLastInsertRowId()
	{   //const char* sqlLastInsertRowId = "SELECT seq from sqlite_sequence WHERE name=?;";
		const char* sqlLastInsertRowId = "SELECT last_insert_rowid();";
		std::string lastRowId;
		const int rc = sqlite3_exec(db, sqlLastInsertRowId, GetLastInsertRowIdCb, &lastRowId,(char**) &errorMsg);
		if(rc!=SQLITE_OK)
		{   return "";
		}
		return lastRowId;
	}
	static int PrintCb(void*, int cols, char **value, char **colName)
	{	for(int i=0; i<cols; i++)
		{	printf("%s = %s\n", colName[i], value[i] ? value[i] : "NULL");
		}
		return 0;
	}
	static int OkCb(void*, int cols, char **value, char **colName)
	{	for(int i=0; i<cols; i++)
		{	printf("%s = %s\n", colName[i], value[i] ? value[i] : "NULL");
		}
		return 0;
	}
	static int GetLastInsertRowIdCb(void* lastRowIdPtr, int cols, char **value, char **)
	{	if(cols<1)
		{   return 0;
		}
		std::string* lastRowId = (std::string*) lastRowIdPtr;
		*lastRowId=value[0];
		return 0;
	}
};

#if 0
int main(int argc, char* argv[])
{	Sqlite db;
	if(!db.Open("test.db"))
	{	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return 1;
	}
	fprintf(stderr, "Opened database successfully\n");
	const char* sql = "CREATE TABLE COMPANY("
		 "ID INT PRIMARY KEY     NOT NULL,"
		 "NAME           TEXT    NOT NULL,"
		 "AGE            INT     NOT NULL,"
		 "ADDRESS        CHAR(50),"
		 "SALARY         REAL );";
	if(db.Exec(sql,Sqlite::Print))
	{	fprintf(stderr, "SQL error: %s\n", db.GetErrorMsg());
		return 2;
	}
	else
	{	fprintf(stdout, "Table created successfully\n");
	}
	sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
		 "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
		 "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
		 "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
		 "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
		 "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
		 "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
		 "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";
	if(db.Exec(sql,Sqlite::Print))
	{	fprintf(stderr, "SQL error: %s\n", db.GetErrorMsg());
		return 3;
	}
	else
	{	fprintf(stdout, "Table inserted successfully\n");
	}
	sql = "SELECT * from COMPANY";
	if(db.Exec(sql,Sqlite::Print))
	{	fprintf(stderr, "SQL error: %s\n", db.GetErrorMsg());
		return 4;
	}
	else
	{	fprintf(stdout, "Table queried successfully\n");
	}
	return 0;
}
#endif
//$g++ test.c -l sqlite3

}

#endif
