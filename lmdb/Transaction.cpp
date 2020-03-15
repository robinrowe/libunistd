// Transaction.cpp
// Created by Robin Rowe on 2018/03/31
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#include "LightningDb.h"
#include "Transaction.h"

namespace lmdb {

Transaction::Transaction(LightningDb& db,unsigned int flags)
:	rc(0)
,	db(db)
,	status("ok")
{	if(!Begin(flags))
	{	txn = 0;
}	}

bool Transaction::Begin(unsigned int flags)
{	MDB_txn* parent = 0;
//		MDB_env* env = db.GetEnv();
	rc = mdb_txn_begin(db.GetEnv(),parent,flags,&txn);
	const int ok = 0;
	switch(rc)
	{	default:
			status = "unknown";
			return false;
		case ok:
			break;	
		case MDB_PANIC: // A fatal error occurred earlier and the environment must be shut down.
			status = "unknown";
			return false;
		case MDB_MAP_RESIZED: // Another process wrote data beyond this MDB_env's mapsize. See mdb_env_set_mapsize().
			status = "map resized";
			return false;
		case MDB_READERS_FULL: // Reader lock table is full. See mdb_env_set_maxreaders().
			status = "readers full";
			return false;
#if 0
		case ENOMEM: // out of memory
			status = "out of memory";
			return false;
#endif
	} 
	return true;
}

bool Transaction::Put(Item& item,int flags)
{	item.Set();
	return db.Put(txn,item.GetKey(),item.GetVal());
}

}
