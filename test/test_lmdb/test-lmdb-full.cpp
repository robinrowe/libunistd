// test-lmdb.cpp
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#include <stdio.h>

#include "lmdb.h"
#include "memory.h"

class Lmdb
{	MDB_env *env;
	MDB_dbi dbi;
	MDB_txn *txn;
	int rc;
public:
	~Lmdb()
	{	mdb_txn_abort(txn);
		mdb_close(env, dbi);
		mdb_env_close(env);
	}
	Lmdb()
	{	rc = mdb_env_create(&env);
	}
	bool BeginWrite()
	{	rc = mdb_txn_begin(env,NULL,0,&txn);
		if(rc)
		{	return false;
		}
		return true;
	}
	bool BeginRead()
	{	rc = mdb_txn_begin(env, NULL, MDB_RDONLY, &txn);
		if(rc)
		{	return false;
		}
		return true;
	}
	bool Open(const char* path,const char* dbname)
	{	if(rc)
		{	return false;
		}
		rc = mdb_env_open(env,path,0,0664);
		if(rc)
		{	return false;
		}
		if(!BeginWrite())
		{	return false;
		}
		rc = mdb_open(txn, dbname, 0, &dbi);
		Commit();
		return 0==rc;
	}
	bool operator!() const
	{	return 0!=rc;
	}
	bool Commit()
	{	rc = mdb_txn_commit(txn);
		return 0==rc;
	}
	const char* toString() const
	{	return mdb_strerror(rc);
	}
	MDB_dbi GetDbi()
	{	return dbi;
	}
	MDB_txn* GetTxn()
	{	return txn;
	}
};

class Cursor
{	MDB_cursor *cursor;
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

template <typename K,typename V>
class Datum
{	MDB_val key;
	MDB_val data;
	K k;
	V v;
	int rc;
public:
	Datum(K k,V v)
	{	Set(k,v);
	}
	void Set(K k,V v)
	{	memcpy(&this->k,&k,sizeof(K));
		memcpy(&this->v,&v,sizeof(V));
		key.mv_size = sizeof(K);
		key.mv_data = &this->k;
		data.mv_size = sizeof(V);
		data.mv_data = &this->v;
	}
	bool Put(Lmdb& lmdb)
	{	rc = mdb_put(lmdb.GetTxn(), lmdb.GetDbi(), &key, &data, 0);
		return 0==rc;
	}
	bool Get(Cursor& cursor)
	{	rc = mdb_cursor_get(cursor.GetCursor(), &key, &data, MDB_NEXT);
		if(0!=rc)
		{	return false;
		}
		memcpy(&k,key.mv_data,sizeof(K));
		memcpy(&v,data.mv_data,sizeof(V));
		return true;
	}
	K& Key()
	{	return k;
	}
	V& Value()
	{	return v;
	}
};

int main(int argc,char * argv[])
{	Lmdb db;
	if(!db.Open("/Code/trasec/test",NULL))
	{	return 1;
	}
	if(!db.BeginWrite())
	{	return 2;
	}
	Datum<int,int> datum(41,14000);
	if(!datum.Put(db))
	{	return 3;
	}
	if(!db.Commit()) 
	{	fprintf(stderr, "mdb_txn_commit: %s\n", db.toString());
		return 4;
	}
	if(!db.BeginWrite())
	{	return 5;
	}
	Cursor cursor(db);
	if(!cursor)
	{	return 6;
	}
	while(datum.Get(cursor))
	{	const int& k=datum.Key();
		const int& v=datum.Value();
		printf("key: '%d', data: '%d'\n",k,v);
		if(32==k)
		{	cursor.DropDatum();
		}
	}
	db.Commit();
	return 0;
}

/*
	// key: 00865FDC '020 ', data: 00865FE0 '020 3141592 foo bar'
	// key: '020 020 3141592 foo bar', data: '020 3141592 foo bar'
key: '32', data: '100'
key: '41', data: '14000'
key: '540029488', data: '540029488'
*/

