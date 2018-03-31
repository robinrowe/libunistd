// LightningDb.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef Lmdb_h
#define Lmdb_h

#include "source/lmdb.h"
#include <memory.h>

namespace lmdb {

class Lmdb
{	MDB_env *env;
	MDB_dbi dbi;
	MDB_txn *txn;
	int rc;
	void Reset()
	{	env = 0;
		dbi = 0;
		txn = 0;
		rc = 0;
	}
public:
	~Lmdb()
	{	mdb_txn_abort(txn);
		mdb_close(env, dbi);
		mdb_env_close(env);
		Reset();
	}
	Lmdb()
	{	Reset();
		rc = mdb_env_create(&env);
	}
	bool BeginWrite()
	{	rc = mdb_txn_begin(env,0,0,&txn);
		if(rc)
		{	return false;
		}
		return true;
	}
	bool BeginRead()
	{	rc = mdb_txn_begin(env, 0, MDB_RDONLY, &txn);
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

}
#endif
