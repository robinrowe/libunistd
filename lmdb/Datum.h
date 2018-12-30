// Datum.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Datum_h
#define LMDB_Datum_h

#include <memory.h>
#include "source/lmdb.h"
#include "Db.h"
#include "Transaction.h"
#include "Cursor.h"

namespace lmdb {

template <typename T>
void SetDatum(const T& v,MDB_val* val)
{	val->mv_size = sizeof(T);
	val->mv_data = val;
}

template <typename T>
void ZeroDatum(T& t)
{	memset(&t,0,sizeof(T));
}

template <typename K,typename V>
class Datum
{	MDB_val k;
	MDB_val v;
public:
	K key;
	V value;
	void Set()
	{	SetDatum(key,&k);
		SetDatum(value,&v);
	}
	void Set(const K& key,const V& value)
	{	this->key = key;
		this->value = value;
		Set();
	}
	Datum(const K& key,const V& value)
	{	Set(key,value);
	}
	Datum()
	{	ZeroDatum(key);
		ZeroDatum(value);
		Set();
	}
	bool Put(Transaction t)
	{	return t.Put(&k,&v);
	}
	bool GetFirst(Cursor& cursor)
	{	return cursor.GetFirst(&k,&v);
	}
	bool GetNext(Cursor& cursor,MDB_cursor_op op = MDB_NEXT)
	{	return cursor.GetNext(&k,&v,op);
	}
	bool GetLast(Cursor& cursor)
	{	return cursor.GetLast(&k,&v);
	}
};

}
#endif
