// Cursor.cpp
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#include "LightningDb.h"

namespace lmdb {

Cursor::Cursor(LightningDb& lightningDb)
:	tr(lightningDb,MDB_RDONLY)
,	mCursor(0)
,	rc(-1)
{	if(!tr)
	{	return;
	}		
	rc = mdb_cursor_open(tr,lightningDb.GetDbi(),&mCursor);
}

bool Cursor::Get(Item& item,MDB_cursor_op op)
{	rc = mdb_cursor_get(mCursor,item.GetKey(),item.GetVal(),op);
	const int ok = 0;
	if(ok != rc)
	{	return false;
	}
	return item.Retrieve();
}

}

#if 0


	bool GetFirst(MDB_val* key, MDB_val* value)
	{	return GetNext(key,value,MDB_NEXT);
	}
	bool GetLast(MDB_val* key, MDB_val* value)
	{	return GetNext(key,value,MDB_LAST);
	}

template <typename T>
struct Field2
:	public Field
{	Field2()
	{}
	Field2(T* data,size_t size)
	{	Set(data,size);
	}
	void Set(T* data,size_t size)
	{	Set(data,size);
	}
	const T* GetData()
	{	return (const T*) FieldBase::GetData();
	}
};

template <typename K>
class Item2
:	public Blob
{	 
public:
	Field<K> key;
	FieldBase value;
	void Encode()
	{	lmdb::Encode(key);
		lmdb::Encode(value);
	}
	void Decode()
	{	lmdb::Decode(key);
		lmdb::Decode(value);
	}
	void Set(K* key,V* value)
	{	this->key = key;
		this->value = value;
		Encode();
	}
	Item(K* key,V* value)
	:	key(key)
	,	value(value)
	{	Encode();
	}
	bool Put(Transaction& t)
	{	return t.Put(&key.mval,&value.mval);
	}
	void Print(std::ostream& os)
	{	os << key.v << " = " << value.v;
	}
};

std::ostream& operator<<(std::ostream& os,const lmdb::Item& item)
{	return item.Print(os);
}
#endif

#if 0
bool LightningDb::Get(const K& key)
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
