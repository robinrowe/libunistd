// Cursor.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Cursor_h
#define LMDB_Cursor_h

#include <portable/no_copy.h>
#include "Transaction.h"
#include "Item.h"

namespace lmdb {

class LightningDb;

class Cursor
{	no_copy(Cursor);
	Transaction tr;
	MDB_cursor* mCursor;
	int rc;
	bool Retrieve(MDB_val* key,MDB_val* value,MDB_cursor_op op = MDB_NEXT)
	{	rc = mdb_cursor_get(mCursor,key,value,op);
		return 0 == rc;
	}
public:
	Cursor(LightningDb& lightningDb);
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
	bool Drop(bool isDelAllKeys=false)
	{	const unsigned flags = isDelAllKeys ? 0:MDB_NODUPDATA;
		rc = mdb_cursor_del(mCursor,flags);
		return 0 == rc;
	}
	bool GetFirst(Item& item)
	{	return Get(item,MDB_FIRST);
	}
	bool GetLast(Item& item)
	{	return Get(item,MDB_LAST);
	}
	bool Get(Item& item,MDB_cursor_op op = MDB_NEXT);
};

}
#endif
