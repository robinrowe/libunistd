// Db.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License MIT open source

#ifndef LMDB_Db_h
#define LMDB_Db_h

#include <memory.h>
#include "source/lmdb.h"
#include "../portable/no_copy.h"

namespace lmdb {

class Db
{	no_copy(Db);
	MDB_env* env;
	MDB_dbi dbi;
	int rc;
	void Reset()
	{	env = 0;
		dbi = 0;
		rc = 0;
		status = "ok";
	}
	const char* status;
public:
	~Db()
	{	Close();
	}
	Db()
	{	Reset();
		rc = mdb_env_create(&env);
		if(0!=rc)
		{	status = "env_create failed";
	}	}
	const char* Status() const
	{	return status;
	}
	bool operator!() const
	{	return 0!=rc;
	}
	MDB_env* GetEnv()
	{	return env;
	}
	operator MDB_dbi()
	{	return dbi;
	}
	const char* toString() const
	{	return mdb_strerror(rc);
	}
	bool Drop(const char* filename);
	bool Open(const char* filename,int flags = 0,size_t size = 1024 * 1024);
	void Close();	bool Put(MDB_txn* txn,MDB_val* key,MDB_val* value,int flags = 0);
};

}
#endif
