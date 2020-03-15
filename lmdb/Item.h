// Item.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT 

#ifndef LMDB_Item_h
#define LMDB_Item_h

#include <memory.h>
#include <string>
#include <ostream>
#include "Field.h"

namespace lmdb {

class Item
{protected:
	Field key;
	Field value;
	char delimiter;
public:
	Item()
	:	delimiter('|')
	{}
	MDB_val* GetKey()
	{	return &key.mval;
	}
	MDB_val* GetVal()
	{	return &value.mval;
	}
	virtual void Set() = 0;
	virtual bool Retrieve() = 0;
	virtual std::ostream& Print(std::ostream& os) const = 0;
};	

}

inline
std::ostream& operator<<(std::ostream& os,const lmdb::Item& item)
{	return item.Print(os);
}

#endif
