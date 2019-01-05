// Transaction.h
// Created by Robin Rowe on 2018/03/31
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Transaction_h
#define LMDB_Transaction_h

#include "source/lmdb.h"
#include "../portable/no_copy.h"

namespace lmdb {

class Transaction
{	no_copy(Transaction);
	MDB_txn* txn;
	int rc;
	const char* status;
public:
	Db& db;
	Transaction(Db& db,unsigned int flags = 0) //MDB_RDONLY
	:	rc(0)
	,	txn(0)
	,	db(db)
	,	status("ok")
	{	if(!Begin(flags))
		{	txn = 0;
	}	}
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
	bool Begin(unsigned int flags = 0)
	{	MDB_txn* parent = 0;
		MDB_env* env = db.GetEnv();
		rc = mdb_txn_begin(env,parent,flags,&txn);
		switch(rc)
		{	default:
				status = "unknown";
				return false;
			case MDB_PANIC: // A fatal error occurred earlier and the environment must be shut down.
				status = "unknown";
				return false;
			case MDB_MAP_RESIZED: // Another process wrote data beyond this MDB_env's mapsize. See mdb_env_set_mapsize().
				status = "map resized";
				return false;
			case MDB_READERS_FULL: // Reader lock table is full. See mdb_env_set_maxreaders().
				status = "readers full";
				return false;
			case ENOMEM: // out of memory
				status = "out of memory";
				return false;
			case 0:
				break;	
		} 
		return true;
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
	bool Put(MDB_val* key,MDB_val* value)
	{	return db.Put(txn,key,value);
	}
};

}
#endif
