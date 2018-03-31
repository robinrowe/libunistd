// Datum.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Datum_h
#define LMDB_Datum_h

#include "Lmdb.h"
#include <memory.h>

namespace lmdb {

template <typename K,typename V>
class Datum
{	MDB_val mdbKey;
	MDB_val mdbValue;
	K key;
	V value;
	int rc;
	void Init()
	{	mdbKey.mv_size = sizeof(K);
		mdbKey.mv_data = &this->key;
		mdbValue.mv_size = sizeof(V);
		mdbValue.mv_data = &this->value;
	}
public:
	Datum()
	:	rc(0)
	{	Init();
	}
	Datum(const K& key,const V& value)
	:	rc(0)
	{	Set(key,value);
	}
	void Set(const K& key,const V& value)
	{	Init();
		memcpy(&this->key,&key,sizeof(K));
		memcpy(&this->value,&value,sizeof(V));
	}
	bool Put(Lmdb& lmdb)
	{	rc = mdb_put(lmdb.GetTxn(), lmdb.GetDbi(), &mdbKey, &mdbValue, 0);
		return 0==rc;
	}
	bool Get(Cursor& cursor,MDB_cursor_op op=MDB_NEXT)
	{	rc = mdb_cursor_get(cursor.GetCursor(), &mdbKey, &mdbValue, op);
		if(0!=rc)
		{	return false;
		}
		memcpy(&key,mdbKey.mv_data,sizeof(K));
		memcpy(&value,mdbValue.mv_data,sizeof(V));
		return true;
	}
	bool GetFirst(Lmdb& db,MDB_cursor_op op=MDB_FIRST)
	{	if(!db.BeginRead())
		{	return false;
		}
		Cursor cursor(db);
		if(!cursor)
		{	return false;
		}
		const bool isOk=Get(cursor);
		db.Commit();
		return isOk;
	}
	bool GetLast(Lmdb& db)
	{	return GetFirst(db,MDB_LAST);
	}
	K& Key()
	{	return key;
	}
	V& Value()
	{	return value;
	}
	bool Write(Lmdb& db)
	{	if(!db.BeginWrite())
		{	return false;
		}
		if(!Put(db))
		{	return false;
		}
		if(!db.Commit()) 
		{	return false;
		}
		return true;
	}
	bool Get(Lmdb& lmdb,const K& key)
	{	this->key=key;
		mdbKey.mv_size = sizeof(K);
		memcpy(mdbKey.mv_data,&k,sizeof(K));
		rc=mdb_get(lmdb.GetTxn(),lmdb.GetDbi(),&data);
// If the database supports duplicate keys (MDB_DUPSORT) then the first data item for 
// the key will be returned. Retrieval of other items requires the use of mdb_cursor_get().
// 0 on success. Some possible errors are:
// MDB_NOTFOUND - the key was not in the database.
// EINVAL - an invalid parameter was specified.
		if(0!=rc)
		{	return false;
		}
		memcpy(&v,data.mv_data,sizeof(V));
		return true;
	}
};

}
#endif
