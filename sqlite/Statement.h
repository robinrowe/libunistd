// libsqlite/Statement.h
// Robin.Rowe@Cinepaint.org
// 2015/8/3

#ifndef SqliteStatement_h
#define SqliteStatement_h

#include "Db.h"

namespace sqlite {

class Statement
{	const char* query;
	sqlite3_stmt* stmt;
	int position;
public:
	Statement(Db& db,const char* query)
	:	query(query)
	,	position(1)
	,	stmt(0)
	{	const int rc = sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
		if(SQLITE_OK != rc)
	{	stmt = 0;
	}	}
	~Statement()
	{	if(!stmt)
		{	return;
		}
		sqlite3_finalize(stmt);
	}
	bool operator!() const
	{	return 0 == stmt;
	}
	bool Bind(const char* v,int size)
	{	if(!stmt)
		{	return false;
		}
		const int rc = sqlite3_bind_text(stmt,position++,v,size,SQLITE_STATIC);
		return SQLITE_OK == rc;
	}
	bool Bind(int v)
	{	if(!stmt)
		{	return false;
		}
		const int rc = sqlite3_bind_int(stmt,position++,v);
		return SQLITE_OK == rc;
	}
	void Get(double& v,int position)
	{	if(!stmt)
		{	return;
		}
		v = sqlite3_column_double(stmt,position);
	}	
	void Get(int& v,int position)
	{	if(!stmt)
		{	return;
		}
		v = sqlite3_column_int(stmt,position);
	}	
	void Get(long long& v,int position)
	{	if(!stmt)
		{	return;
		}
		v = sqlite3_column_int64(stmt,position);
	}	
	void Get(const char*& v,int position)
	{	if(!stmt)
		{	return;
		}
		const unsigned char* s = sqlite3_column_text(stmt,position);
		v = (const char*) s;	
	}	
	bool Step()
	{	if(!stmt)
		{	return false;
		}
		const int rc = sqlite3_step(stmt); 
		if(rc != SQLITE_ROW) 
		{	return false;
		}
		return true;
	}
};

#if 0





	sqlite3_stmt *ppStmt;  /* OUT: Statement handle */
		const char *pzTail;    /* OUT: Pointer to unused portion of zSql */
		int ok = sqlite3_prepare_v2(db,sql,strlen(sql),&ppStmt,&pzTail);
sqlite3_step()
sqlite3_column_int64()
sqlite3_column_text()
sqlite3_finalize()

sqlite3_close()

	}
	bool Query(const char* sql,long long& v)
	bool Query(const char* sql,std::string& v)
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
#endif
}

#endif
