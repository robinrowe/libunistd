// test-lmdb.cpp
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#include <lmdb/Db.h>
#include <lmdb/Datum.h>
#include <lmdb/Transaction.h>
#include <lmdb/Cursor.h>
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

enum STATUS
{	ok,
	open_failed,
	lock_failed,
	write_failed,
	cursor_failed
};

int main(int argc,char * argv[])
{	lmdb::Db db;
	const char* filename = "/Code/github/libunistd/build/test/lmdb/data.mdb";
//	db.Drop(filename);
	if(!db.Open(filename))//,MDB_CREATE))
	{	return open_failed;
	}
	{	
		lmdb::Datum<int,std::string> data(41,"Hello World");
		lmdb::Transaction tr(db);
		if(!tr)
		{	return lock_failed;
		}		
		cout << "Put: " << data << endl;
		if(!data.Put(tr))
		{	return write_failed;
		}
	}
	{	lmdb::Cursor cursor(db);
		if(!cursor)
		{	return cursor_failed;
		}
		lmdb::Datum<int,std::string> data;
		bool ok = data.GetFirst(cursor);
		while(ok)
		{	cout << "Cursor: " << data << endl;
			if(32==data.key.v)
			{	cursor.DropDatum();
			}
			ok = data.GetNext(cursor);
	}	}
	puts("Done");
	db.Close();
	return 0;
}

/*
	// key: 00865FDC '020 ', data: 00865FE0 '020 3141592 foo bar'
	// key: '020 020 3141592 foo bar', data: '020 3141592 foo bar'
key: '32', data: '100'
key: '41', data: '14000'
key: '540029488', data: '540029488'
*/

