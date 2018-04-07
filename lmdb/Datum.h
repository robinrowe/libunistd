// Datum.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef LMDB_Datum_h
#define LMDB_Datum_h

#include <memory.h>
#include "source/lmdb.h"

namespace lmdb {

template <typename T>
class Datum
{	MDB_val val;
	T v;
public:
	Datum(const T& v)
	:	v(v)
	{	val.mv_size = sizeof(v);
		val.mv_data = &this->v;
	}
	Datum()
	{	val.mv_size = sizeof(v);
		val.mv_data = &this->v;
		memset(&v,0,sizeof(v));
	}
	operator const T&() const
	{	return v;
	}
	operator MDB_val*()
	{	return &val;
	}
};

}
#endif
