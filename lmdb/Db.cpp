// Db.cpp

#include "Db.h"
#include "Transaction.h"

namespace lmdb {

bool Db::Open(const char* path,const char* dbname,size_t size)
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
	Transaction tr(*this);
	rc = mdb_open(tr,dbname,0,&dbi);
	return 0==rc;
}

}