// Transaction.h
// Created by Robin Rowe on 2018/03/31
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Transaction_h
#define LMDB_Transaction_h

#include <portable/no_copy.h>
#include "liblmdb/lmdb.h"

#define MDB_WRITE 0

namespace lmdb {

class LightningDb;

class Transaction
{	no_copy(Transaction);
	MDB_txn* txn;
	int rc;
	const char* status;
public:
	LightningDb& db;
	Transaction(LightningDb& db,unsigned int flags = MDB_RDONLY); //MDB_WRITE
	~Transaction()
	{	Commit();
	}
	bool Commit()
	{	if(!txn)
		{	return false;
		}
		rc = mdb_txn_commit(txn);
		txn = 0;
		return 0 == rc;
	}
	void Abort()
	{	if(!txn)
		{	return;
		}
		mdb_txn_abort(txn);
		txn = 0;
	}
	bool operator!() const
	{	return 0 == txn;
	}
	operator MDB_txn*()
	{	return txn;
	}
	const char* toString() const
	{	return mdb_strerror(rc);
	}
	bool Begin(unsigned int flags = 0);
	bool Put(Item& item,int flags = 0);
	bool Get(Item& item);
	bool Drop(Item& item);
};

}
#endif
