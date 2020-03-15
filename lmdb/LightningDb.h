// Db.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License MIT open source

#ifndef LMDB_Db_h
#define LMDB_Db_h

#include <memory.h>
#include <portable/no_copy.h>
#include "liblmdb/lmdb.h"
#include "Item.h"
#include "Transaction.h"
#include "Cursor.h"

namespace lmdb {

class LightningDb
{	no_copy(LightningDb);
	MDB_env* env;
	MDB_dbi dbi;
	int rc;
	void Reset()
	{	env = 0;
		dbi = 0;
		rc = 0;
		status = "LMDB: ok";
		isVerbose = false;
	}
	const char* status;
	bool IsInvalid(MDB_txn* txn,MDB_val* key,MDB_val* value);
public:
	bool isVerbose;
	~LightningDb()
	{	Close();
	}
	LightningDb();
	const char* Status() const
	{	return status;
	}
	bool operator!() const
	{	return 0!=rc;
	}
	MDB_env* GetEnv()
	{	return env;
	}
	MDB_dbi GetDbi()
	{	return dbi;
	}
	const char* toString() const
	{	return mdb_strerror(rc);
	}
	bool SetToInMemory()
	{	return !mdb_env_set_flags(env,MDB_NOSYNC | MDB_WRITEMAP,1);
	}	
	const char* GetVersion() const
	{	return mdb_version(0,0,0);
	}
	const char* GetErrorMsg(int err) const
	{	return mdb_strerror(err);
	}
	bool Drop(const char* filename);
	bool Open(const char* filename,int flags = MDB_WRITE,size_t size = 1024 * 1024);
	void Close();	
	bool Put(MDB_txn* txn,MDB_val* key,MDB_val* value,int flags = 0);
	bool Get(MDB_txn* txn,MDB_val* key,MDB_val* value);
	bool Drop(MDB_txn* txn,MDB_val* key,MDB_val* value);
};

}
#endif
