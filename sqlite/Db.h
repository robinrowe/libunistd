// sqlite/SqliteDb.h
// Robin.Rowe@Cinepaint.org
// 2015/8/3

#ifndef SqliteDb_h
#define SqliteDb_h

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <portable/Db.h>
#include "source/sqlite3.h"

namespace sqlite {

class Db
:	public portable::Db
{	sqlite3* db;
	bool isOpen;
	bool isTrace;
	const char* errorMsg;
	void SetErrorMsg()
	{	errorMsg=sqlite3_errmsg(db);
		status=errorMsg;
		//qDebug()<<"Error "<<errorMsg;
	}
	void Trace(const char* msg)
	{	if(!isTrace)
		{	return;
		}
		printf("TRACE SQL: %s\n",msg);
		//qDebug()<<sql;
	}
public:
	std::string status;
	Db()
	:	db(0)
	,	errorMsg(0)
	,	isOpen(false)
	,	isTrace(false)
	{}
	~Db()
	{   Close();
	}
	operator sqlite3*()
	{	return db;
	}
	void SetTrace(bool isTrace = true)
	{	this->isTrace = isTrace;
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
	bool Exec(const char* sql,DbCallback callback,void* object)
	{	if(!sql)
		{	return false;
		}
		Trace(sql);
		const int rc = sqlite3_exec(db, sql, callback, object,(char**) &errorMsg);
		if(rc!=SQLITE_OK)
		{   SetErrorMsg();
			return false;
		}
		return true;
	}
	long long GetLastInsertRowId()
	{	return sqlite3_last_insert_rowid(db);
	}
	bool GetLastInsertRowId(std::string& s)
	{   //const char* sqlLastInsertRowId = "SELECT seq from sqlite_sequence WHERE name=?;";
		const char* sqlLastInsertRowId = "SELECT last_insert_rowid();";
// bug...
		const int rc = sqlite3_exec(db, sqlLastInsertRowId, GetLastInsertRowIdCb, &s,(char**) &errorMsg);
		if(rc!=SQLITE_OK)
		{   return false;
		}
		return true;
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

}

#endif
