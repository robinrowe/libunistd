// Cursor.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Cursor_h
#define LMDB_Cursor_h

#include <portable/no_copy.h>
#include "Db.h"
#include "Cursor.h"
#include "Transaction.h"

namespace lmdb {

class Db;

class Cursor
{	no_copy(Cursor);
	Transaction tr;
	MDB_cursor* mCursor;
	int rc;
public:
	Cursor(Db& db)
	:	tr(db,MDB_RDONLY)
	,	mCursor(0)
	,	rc(-1)
	{	if(!tr)
		{	return;
		}		
		rc = mdb_cursor_open(tr,tr.db,&mCursor);
	}
	~Cursor()
	{	Close();
	}
	bool operator!() const
	{	return 0 != rc;
	}
	operator MDB_cursor*()
	{	return mCursor;
	}
	void Close()
	{	if(mCursor)
		{	mdb_cursor_close(mCursor);
		}
		tr.Abort();
		mCursor = 0;
	}
	bool DropDatum(bool isDelAllKeys=false)
	{	const unsigned flags = isDelAllKeys ? 0:MDB_NODUPDATA;
		rc = mdb_cursor_del(mCursor,flags);
		return 0 == rc;
	}
	bool GetNext(MDB_val* key,MDB_val* value,MDB_cursor_op op = MDB_NEXT)
	{	rc = mdb_cursor_get(mCursor,key,value,op);
		return 0 == rc;
	}
	bool GetFirst(MDB_val* key, MDB_val* value)
	{	return GetNext(key,value,MDB_NEXT);
	}
	bool GetLast(MDB_val* key, MDB_val* value)
	{	return GetNext(key,value,MDB_LAST);
	}
};

}
#endif
