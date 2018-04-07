// Db.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Db_h
#define LMDB_Db_h

#include "source/lmdb.h"
#include "Transaction.h"
#include "Datum.h"
#include "Cursor.h"
#include <memory.h>

namespace lmdb {

class Db
{	MDB_env *env;
	MDB_dbi dbi;
	int rc;
	void Reset()
	{	env = 0;
		dbi = 0;
		rc = 0;
	}
public:
	~Db()
	{	mdb_close(env, dbi);
		mdb_env_close(env);
		Reset();
	}
	Db()
	{	Reset();
		rc = mdb_env_create(&env);
	}
	bool Open(const char* path,const char* dbname,size_t size = 1024 * 1024)
	{	if(rc)
		{	return false;
		}
		rc = mdb_env_set_mapsize(env,size);
		if(rc)
		{	return false;
		}
		rc = mdb_env_set_maxdbs(env, 4);
		if(rc)
		{	return false;
		}
		rc = mdb_env_open(env,path,0,0664);
		if(rc)
		{	return false;
		}
		Transaction tr(env);
		rc = mdb_open(tr,dbname,0,&dbi);
		return 0==rc;
	}
	bool operator!() const
	{	return 0!=rc;
	}
	operator MDB_env*()
	{	return env;
	}
	operator MDB_dbi()
	{	return dbi;
	}
	const char* toString() const
	{	return mdb_strerror(rc);
	}
	bool Put(MDB_txn* txn,MDB_val* key,MDB_val* value)
	{	rc = mdb_put(txn,dbi,key,value, 0);
		return 0 == rc;
	}
#if 0
	bool Get(const K& key)
	{
		this->key = key;
		mdbKey.mv_size = sizeof(K);
		memcpy(mdbKey.mv_data, &k, sizeof(K));
		rc = mdb_get(lmdb.GetTxn(), lmdb.GetDbi(), &data);
		// If the database supports duplicate keys (MDB_DUPSORT) then the first data item for 
		// the key will be returned. Retrieval of other items requires the use of mdb_cursor_get().
		// 0 on success. Some possible errors are:
		// MDB_NOTFOUND - the key was not in the database.
		// EINVAL - an invalid parameter was specified.
		if (0 != rc)
		{
			return false;
		}
		memcpy(&v, data.mv_data, sizeof(V));
		return true;
	}
#endif

};

inline
Cursor::Cursor(Db& db)
:	tr(db)
{	rc = mdb_cursor_open(tr, db, &cursor);
}

}
#endif
