// test-lmdb.cpp
// Created by Robin Rowe on 12/21/2015
// Copyright (c) 2015 Robin.Rowe@CinePaint.org
// OpenLDAP Public License

#include <lmdb/Lmdb.h>
#include <lmdb/Cursor.h>
#include <lmdb/Datum.h>
#include <stdio.h>

int main(int argc,char * argv[])
{	lmdb::Lmdb db;
	const char* dbPath = "/Code/github/libunistd/test/lmdb";
	if(!db.Open(dbPath,NULL))
	{	return 1;
	}
	lmdb::Datum<int,int> datum(41,14000);
	if(!datum.Write(db))
	{	return 2;
	}
	if(!db.BeginWrite())
	{	return 3;
	}
	lmdb::Cursor cursor(db);
	if(!cursor)
	{	return 4;
	}
	while(datum.Get(cursor))
	{	const int& k=datum.Key();
		const int& v=datum.Value();
		printf("key: '%d', data: '%d'\n",k,v);
		if(32==k)
		{	cursor.DropDatum();
		}
	}
	db.Commit();
	return 0;
}

/*
	// key: 00865FDC '020 ', data: 00865FE0 '020 3141592 foo bar'
	// key: '020 020 3141592 foo bar', data: '020 3141592 foo bar'
key: '32', data: '100'
key: '41', data: '14000'
key: '540029488', data: '540029488'
*/

