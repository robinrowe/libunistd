// test-lmdb.cpp
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#include <lmdb/Db.h>
#include <stdio.h>

int main(int argc,char * argv[])
{	lmdb::Db db;
	const char* dbPath = "/Code/github/libunistd/test/lmdb";
	const char* dbName = "test-lmdb.lmdb";
	if(!db.Open(dbPath,0))//dbName))
	{	return 1;
	}
	{	lmdb::Transaction tr(db);
		lmdb::Datum<int> key(41);
		lmdb::Datum<int> value(14000);
		if (!db.Put(tr,key,value))
		{	return 2;
	}	}
	{	lmdb::Transaction tr(db);
		lmdb::Datum<int> key(41);
		lmdb::Datum<int> value(14000);
		if (!db.Put(tr, key, value))
		{	return 2;
	}	}
	lmdb::Cursor cursor(db);
	if(!cursor)
	{	return 4;
	}
	lmdb::Datum<int> key;
	lmdb::Datum<int> value;
	bool ok = cursor.GetFirst(key,value);
	while(ok)
	{	printf("key: '%d', data: '%d'\n",(int)key,(int)value);
		if(32==key)
		{	cursor.DropDatum();
		}
		ok = cursor.GetNext(key,value);
	}
	puts("Done");
	return 0;
}

/*
	// key: 00865FDC '020 ', data: 00865FE0 '020 3141592 foo bar'
	// key: '020 020 3141592 foo bar', data: '020 3141592 foo bar'
key: '32', data: '100'
key: '41', data: '14000'
key: '540029488', data: '540029488'
*/

