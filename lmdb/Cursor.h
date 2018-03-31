// Cursor.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Cursor_h
#define LMDB_Cursor_h

#include "Lmdb.h"

namespace lmdb {

class Cursor
{	MDB_cursor* cursor;
	Lmdb& lmdb;
	int rc;
public:
	~Cursor()
	{	mdb_cursor_close(cursor);
	}
	Cursor(Lmdb& lmdb)
	:	lmdb(lmdb)
	{	rc = mdb_cursor_open(lmdb.GetTxn(),lmdb.GetDbi(),&cursor);
	}
	bool operator!() const
	{	return 0!=rc;
	}
	MDB_cursor* GetCursor()
	{	return cursor;
	}
	bool DropDatum(bool isDelAllKeys=false)
	{	const unsigned flags = isDelAllKeys ? 0:MDB_NODUPDATA;
		rc = mdb_cursor_del(cursor,flags);
		return 0==rc;
	}
};

}
#endif
