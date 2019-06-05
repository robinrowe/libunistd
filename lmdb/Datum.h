// Datum.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT 

#ifndef LMDB_Datum_h
#define LMDB_Datum_h

#include <memory.h>
#include <string>
#include <ostream>
#include <lmdb.h>
#include "Db.h"
#include "Transaction.h"
#include "Cursor.h"

namespace lmdb {

template <typename T>
struct Field
{	MDB_val mval;
	T v;
};

template <typename T>
void Encode(Field<T>& d)
{	d.mval.mv_size = sizeof(T);
	d.mval.mv_data = (void*) &d.v;
}

template <typename T>
void Decode(Field<T>& d)
{	d.v = *(T*) d.mval.mv_data;
}

template <typename T>
void Zero(Field<T>& d)
{	memset(&d.v,0,sizeof(T));
	d.mval.mv_size = 0;
	d.mval.mv_data = 0;
}

void Encode(Field<std::string>& d)
{	d.mval.mv_size = d.v.size()+1;
	d.mval.mv_data = (void*) d.v.c_str();
}

void Decode(Field<std::string>& d)
{	d.v = (char*) d.mval.mv_data;
}

void Zero(Field<std::string>& d)
{	d.v.clear();
	d.mval.mv_size = 0;
	d.mval.mv_data = 0;
}

template <typename K,typename V>
class Datum
{	 
public:
	Field<K> key;
	Field<V> value;
	void Encode()
	{	lmdb::Encode(key);
		lmdb::Encode(value);
	}
	void Decode()
	{	lmdb::Decode(key);
		lmdb::Decode(value);
	}
	void Set(const K& key,const V& value)
	{	this->key = key;
		this->value = value;
		Encode();
	}
	Datum(const K& key,const V& value)
	{	this->key.v = key;
		this->value.v = value;
		Encode();
	}
	Datum()
	{	Zero(key);
		Zero(value);
	}
	bool Put(Transaction& t)
	{	return t.Put(&key.mval,&value.mval);
	}
	bool GetFirst(Cursor& cursor)
	{	if(!cursor.GetFirst(&key.mval,&value.mval))
		{	return false;
		}
		Decode();
		return true;
	}
	bool GetNext(Cursor& cursor,MDB_cursor_op op = MDB_NEXT)
	{	if(!cursor.GetNext(&key.mval,&value.mval,op))
		{	return false;
		}
		Decode();
		return true;
	}
	bool GetLast(Cursor& cursor)
	{	if(!cursor.GetLast(&key,&value))
		{	return false;
		}
		Decode();
		return true;
	}
	void Print(std::ostream& os)
	{	os << key.v << " = " << value.v;
	}
};

}

template <typename K,typename V>
std::ostream& operator<<(std::ostream& os,lmdb::Datum<K,V> data)
{	data.Print(os);
	return os;
}

#endif
