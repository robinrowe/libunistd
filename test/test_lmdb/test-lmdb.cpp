// test-lmdb.cpp
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#include <lmdb/LightningDb.h>
#include <lmdb/Item.h>
#include <lmdb/Transaction.h>
#include <lmdb/Cursor.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "ProductData.h"
using namespace std;

enum STATUS
{	ok,
	open_failed,
	lock_failed,
	write_failed,
	cursor_failed
};

int main(int argc,char * argv[])
{	lmdb::LightningDb db;
//	const char* filename = "/Code/github/libunistd/build/test/lmdb/data.mdb";
	const char* filename = "./test-lmdb";
#if 1
	db.Drop(filename);
#endif
	if(!db.Open(filename,MDB_CREATE))
	{	return open_failed;
	}
	lmdb::Transaction tr(db);
	if(!tr)
	{	return lock_failed;
	}	
	{	
		ProductData data;
		data.id = 41;
		data.product = "Widget";
		data.description = "Generic";
		cout << "Put: " << data << endl;
		if(!tr.Put(data))
		{	return write_failed;
		}
		data.id = 32;
		data.product = "Gadget";
		data.description = "Special";
		cout << "Put: " << data << endl;
		if(!tr.Put(data))
		{	return write_failed;
		}
		tr.Commit();
	}
	{	lmdb::Cursor cursor(db,tr,0);
		if(!cursor)
		{	return cursor_failed;
		}
		ProductData data;
		bool ok = cursor.GetFirst(data);
		while(ok)
		{	cout << "Cursor: " << data << endl;
			if(32==data.id)
			{	cursor.Drop();
				cout << "Dropped id #" << 32 << endl;
			}
			ok = cursor.Get(data);
	}	}
	puts("Done");
	db.Close();
	return 0;
}

/* Output:

	LMDB: ok
	LMDB: drop db failed
	LMDB: Transaction ok
	LMDB: open
	LMDB: Transaction ok
	Put: 41, Widget, Generic
	LMDB: put ok
	Put: 32, Gadget, Special
	LMDB: put ok
	LMDB: Transaction ok
	LMDB: Cursor ok
	LMDB: Cursor get ok
	Cursor: 32, Gadget, Special
	LMDB: Cursor drop ok
	Dropped id #32
	LMDB: Cursor get ok
	Cursor: 41, Widget, Generic
	LMDB: Cursor get not found
	Done

*/

