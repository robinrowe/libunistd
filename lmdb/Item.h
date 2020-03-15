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
{
#if 0
	std::unique_ptr<char[]> data;
	char* p;
	unsigned capacity;
#endif
protected:
	Field key;
	Field value;
	char delimiter;
public:
	Item()
	:	delimiter('|')
	{}
#if 0
	Item(unsigned capacity)
	:	capacity(capacity)
	{	data = std::make_unique<char[]>(capacity);
		p = data.get();
	}
#endif
	MDB_val* GetKey()
	{	return &key.mval;
	}
	MDB_val* GetVal()
	{	return &value.mval;
	}
#if 0
	bool Buffer(void* item,size_t size)
	{	p+=size;
		if(GetSize()<0)
		{	return false;
		}
		memcpy(p,item,size);
		return true;
	}
	void* GetData() const
	{	return data.get();
	}
	int GetSize() const
	{	const int size = int(p - data.get());
		if(unsigned(size) > capacity)
		{	return -size;
		}
		return size;
	}
	unsigned GetCapacity() const
	{	return capacity;
	}
	bool IsOverflow() const
	{	return GetSize() < 0;
	}
#endif
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

