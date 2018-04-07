// Cursor.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Cursor_h
#define LMDB_Cursor_h

#include "source/lmdb.h"
#include "Db.h"
#include "Cursor.h"
#include "Transaction.h"

namespace lmdb {

class Db;

class Cursor
{	MDB_cursor* cursor;
	Transaction tr;
	int rc;
public:
	~Cursor()
	{	Close();
	}
	void Close()
	{	if(!cursor)
		{	return;
		}
		mdb_cursor_close(cursor);
		tr.Close();
		cursor = 0;
	}
	Cursor(Db& db);
	bool operator!() const
	{	return 0 != rc;
	}
	operator MDB_cursor*()
	{	return cursor;
	}
	bool DropDatum(bool isDelAllKeys=false)
	{	const unsigned flags = isDelAllKeys ? 0:MDB_NODUPDATA;
		rc = mdb_cursor_del(cursor,flags);
		return 0 == rc;
	}
	bool GetNext(MDB_val* key,MDB_val* value,MDB_cursor_op op = MDB_NEXT)
	{	rc = mdb_cursor_get(cursor,key,value,op);
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
