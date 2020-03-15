// Field.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// License open source MIT 

#ifndef LMDB_Field_h
#define LMDB_Field_h

#include <memory.h>
#include <string>
#include <string_view>
#include "liblmdb/lmdb.h"

namespace lmdb {

struct Field
{	MDB_val mval;
	size_t offset;
	Field()
	{	Zero();
	}
	void ToMval(void* data,size_t size)
	{	mval.mv_data = data;
		mval.mv_size += size;
	}
	void ToMval(std::string& s)
	{	ToMval((void*) s.c_str(),s.size()+1);
	}
	size_t GetCapacity() const
	{	return mval.mv_size;
	}
	bool IsOverflow() const
	{	if(offset > GetCapacity())
		{	return true;
		}
		return false;
	}
	const char* GetPtr() const
	{	const char* p = (const char*) mval.mv_data;
		return p;
	}
	const char* GetPtr(size_t offset) const
	{	const char* p = GetPtr();
		if(!p)
		{	return 0;
		}
		return p + offset;
	}
	bool FromMval(void* data,size_t size)
	{	offset += size;
		if(IsOverflow())
		{	return false;
		}
		const char* p = GetPtr();
		memcpy(data,p,size);
		return true;
	}
	bool FromMval(std::string& s,char delimiter)
	{	const char* p = GetPtr(offset);
		if(!p)
		{	return false;
		}
		std::string_view sv;
		if(!GetField(sv,p,delimiter))
		{	return false;
		}
		s = sv;
		offset += s.size() + 1;
		return true;
	}
	bool GetField(std::string_view& sv,const char* p,char delimiter) const
	{	const size_t capacity = GetCapacity();
		if(offset > capacity)
		{	return false;
		}
		const size_t remaining = capacity - offset;
		const char* end = (const char*) memchr((void*) p,delimiter,remaining);
		size_t length = end - p;
		sv = std::string_view(p,length);
		return true;
	}
	void* GetData()
	{	return mval.mv_data;
	}
	size_t GetSize() const
	{	return mval.mv_size;
	}
	size_t GetOffset() const
	{	return offset;
	}
	void ResetOffset()
	{	offset = 0;
	}
	void Zero()
	{	mval.mv_size = 0;
		mval.mv_data = 0;
		offset = 0;
	}
};

}
#endif
