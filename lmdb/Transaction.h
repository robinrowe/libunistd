// Transaction.h
// Created by Robin Rowe on 2018/03/31
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Transaction_h
#define LMDB_Transaction_h

#include "source/lmdb.h"

namespace lmdb {

class Transaction
{	Db& db;
	MDB_txn* txn;
	int rc;
public:
	~Transaction()
	{	Close();
	}
	void Close()
	{	if(!txn)
		{	return;
		}
		mdb_txn_abort(txn);
		txn = 0;
	}
	Transaction(Db& db,int flags = 0) //MDB_RDONLY
	:	rc(0)
	,	txn(0)
	,	db(db)
	{	rc = mdb_txn_begin(db.GetEnv(),0,flags,&txn);
		if(rc)
		{	txn = 0;
	}	}
	bool operator!() const
	{	return 0 != rc || 0 == txn;
	}
	operator MDB_txn*()
	{	return txn;
	}
	bool Commit()
	{	rc = mdb_txn_commit(txn);
		return 0 == rc;
	}
	const char* toString() const
	{	return mdb_strerror(rc);
	}
	bool Put(MDB_val* key,MDB_val* value)
	{	return db.Put(txn,key,value);
	}
};

}
#endif
