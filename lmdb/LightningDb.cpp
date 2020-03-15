// LightningDb.cpp

#include <string>
#include <stdio.h>
#include <lmdb.h>
#include "LightningDb.h"
#include "Transaction.h"

namespace lmdb {

LightningDb::LightningDb()
{	Reset();
	rc = mdb_env_create(&env);
	if(0!=rc)
	{	status = "env_create failed";
}	}

/*
	flags	Special options for this database. This parameter must be set to 0 or by bitwise OR'ing together one or more of the values described here.
MDB_REVERSEKEY Keys are strings to be compared in reverse order, from the end of the strings to the beginning. By default, Keys are treated as strings and compared from beginning to end.
MDB_DUPSORT Duplicate keys may be used in the database. (Or, from another perspective, keys may have multiple data items, stored in sorted order.) By default keys must be unique and may have only a single data item.
MDB_INTEGERKEY Keys are binary integers in native byte order, either unsigned int or size_t, and will be sorted as such. The keys must all be of the same size.
MDB_DUPFIXED This flag may only be used in combination with MDB_DUPSORT. This option tells the library that the data items for this database are all the same size, which allows further optimizations in storage and retrieval. When all data items are the same size, the MDB_GET_MULTIPLE and MDB_NEXT_MULTIPLE cursor operations may be used to retrieve multiple items at once.
MDB_INTEGERDUP This option specifies that duplicate data items are binary integers, similar to MDB_INTEGERKEY keys.
MDB_REVERSEDUP This option specifies that duplicate data items should be compared as strings in reverse order.
MDB_CREATE Create the named database if it doesn't exist. This option is not allowed in a read-only transaction or a read-only environment.
*/
bool LightningDb::Open(const char* filename,int flags,size_t size)
{	if(rc)
	{	status = "not ready";
		return false;
	}
#if 0
	rc = mdb_env_set_mapsize(env,size);
	if(rc)
	{	status = "mapsize failed";
		return false;
	}
	rc = mdb_env_set_maxdbs(env, 4);
	if(rc)
	{	status = "maxdbs failed";
		return false;
	}
#endif
	std::string s(filename);
	char* path = (char*) s.c_str();
	char* sep = strrchr(path,'/');
	if(!sep)
	{	sep = strrchr(path,'\\');
	}
	const char* dbname = "";
	if(sep)
	{	*sep = 0;
		dbname = sep+1;
	}
	dbname = 0;
	rc = mdb_env_open(env,path,0,0664);
	if(rc)
	{	status = "env_open failed";
		return false;
	}
	Transaction tr(*this);
	if(!tr)
	{	status = "txn failed";
		return false;
	}
	rc = mdb_dbi_open(tr,dbname,flags,&dbi);
	switch(rc)
	{	default:
			status = "unknown";
			return false;
		case MDB_NOTFOUND:
			status = "not found";
			return false;
		case MDB_DBS_FULL: // too many databases have been opened. See mdb_env_set_maxdbs().
			status = "dbs_full";
			return false;
		case 0:
			break;
	}
	return true;
}

/*	flags	Special options for this operation. This parameter must be set to 0 or by bitwise OR'ing together one or more of the values described here.
MDB_NODUPDATA - enter the new key/data pair only if it does not already appear in the database. This flag may only be specified if the database was opened with MDB_DUPSORT. The function will return MDB_KEYEXIST if the key/data pair already appears in the database.
MDB_NOOVERWRITE - enter the new key/data pair only if the key does not already appear in the database. The function will return MDB_KEYEXIST if the key already appears in the database, even if the database supports duplicates (MDB_DUPSORT). The data parameter will be set to point to the existing item.
MDB_RESERVE - reserve space for data of the given size, but don't copy the given data. Instead, return a pointer to the reserved space, which the caller can fill in later - before the next update operation or the transaction ends. This saves an extra memcpy if the data is being generated later. LMDB does nothing else with this memory, the caller is expected to modify all of the space requested. This flag must not be specified if the database was opened with MDB_DUPSORT.
MDB_APPEND - append the given key/data pair to the end of the database. This option allows fast bulk loading when keys are already known to be in the correct order. Loading unsorted keys with this flag will cause a MDB_KEYEXIST error.
MDB_APPENDDUP - as above, but for sorted dup data.
*/
bool LightningDb::Put(MDB_txn* txn,MDB_val* key,MDB_val* value,int flags)
{	if(!txn)
	{	status = "txn invalid";
		return false;
	}
	if(!key)
	{	status = "key invalid";
		return false;
	}
	if(!value)
	{	status = "value invalid";
		return false;
	}
	rc = mdb_put(txn,dbi,key,value,flags);
	const int ok = 0;
	if(ok!=rc)
	{	return false;
	}
	return true;
}

bool LightningDb::Drop(const char* filename)
{	const int err = remove(filename);
	return !err;
}

void LightningDb::Close()
{	if(env)
	{	mdb_dbi_close(env,dbi);
		mdb_env_close(env);
	}
	Reset();
}

}