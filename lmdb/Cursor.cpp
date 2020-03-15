// Cursor.cpp
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#include "LightningDb.h"

#define VERBOSE(x) if(lightningDb.isVerbose) puts(x)

namespace lmdb {

Cursor::Cursor(LightningDb& lightningDb,Transaction& tr,int flags)
:	lightningDb(lightningDb)
,	mCursor(0)
,	rc(-1)
{	rc = mdb_cursor_open(tr,lightningDb.GetDbi(),&mCursor);
	VERBOSE("LMDB: Cursor ok");
}

bool Cursor::Get(Item& item,MDB_cursor_op op)
{	rc = mdb_cursor_get(mCursor,item.GetKey(),item.GetVal(),op);
	const int ok = 0;
	if(ok == rc)
	{	VERBOSE("LMDB: Cursor get ok");
		return item.Retrieve();
	}
	VERBOSE("LMDB: Cursor get not found");
	return false;
}

bool Cursor::Drop(bool isDelAllKeys)
{	const unsigned flags = isDelAllKeys ? 0:MDB_NODUPDATA;
	rc = mdb_cursor_del(mCursor,flags);
	const int ok = 0;
	if(ok == rc)
	{	VERBOSE("LMDB: Cursor drop ok");
		return true;
	}
	switch(rc)
	{	default:
			VERBOSE("LMDB: Cursor unknown drop failure");
			break;
		case EACCES:
			VERBOSE("LMDB: Cursor failed to write read-only transaction");
			break;
		case EINVAL:
			VERBOSE("LMDB: Cursor invalid parameter");
			break;
	}
	return false;
}

}
