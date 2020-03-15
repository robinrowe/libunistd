// ProductData.h
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#ifndef ProductData_h
#define ProductData_h

#include <string>
#include <iostream>
#include <vector>
#include <lmdb/Item.h>
using namespace std;

struct ProductData
:	public lmdb::Item
{	int id;
	std::string product;
	std::string description;
	std::string buffer;
	ProductData()
	:	id(0)
	{}
	virtual void Set() override
	{	key.ToMval(&id,sizeof(id));
		buffer = product;
		buffer += delimiter;
		buffer += description;
		buffer += delimiter;
		value.ToMval(buffer);
	}
	virtual bool Retrieve() override
	{	key.ResetOffset();
		if(!key.FromMval(&id,sizeof(id)))
		{	return false;
		}
		value.ResetOffset();
		if(!value.FromMval(product,delimiter))
		{	return false;
		}
		if(!value.FromMval(description,delimiter))
		{	return false;
		}
		return true;
	}
	virtual std::ostream& Print(std::ostream& os) const override
	{	return os << id << ", " << product << ", " << description;
	}
};
		//Buffer((void*) name.c_str(),name.size()+1);
#endif