/* sample-mdb.c - MDB toy/sample
 *
 * Do a line-by-line comparison of this and sample-bdb.c
 */
/*
 * Copyright 2012 Howard Chu, Symas Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted only as authorized by the OpenLDAP
 * Public License.
 *
 * A copy of this license is available in the file LICENSE in the
 * top-level directory of the distribution or, alternatively, at
 * <http://www.OpenLDAP.org/license.html>.
 */
#include <stdio.h>
#include <lmdb.h>

int main(int argc,char * argv[])
{
	int rc;
	MDB_env *env;
	MDB_dbi dbi;
	MDB_val key, data;
	MDB_txn *txn;
	MDB_cursor *cursor;
	char sval[32];

	rc = mdb_env_create(&env);
	rc = mdb_env_open(env, "/Code/github/libunistd/build/test", 0, 0664);
	rc = mdb_txn_begin(env, NULL, 0, &txn);
	rc = mdb_open(txn, NULL, 0, &dbi);
#if 0
	mdb_txn_abort(txn);
	rc = mdb_txn_begin(env, NULL, 0, &txn);
#endif
	key.mv_size = sizeof(int);
	int k=32;
	key.mv_data = &k;//sval;
	int d=100;
	data.mv_size = sizeof(int);//sizeof(sval);
	data.mv_data = &d;//sval;
#pragma warning(disable:4996)
	sprintf(sval, "%03x %d foo bar", 32, 3141592);
	rc = mdb_put(txn, dbi, &key, &data, 0);
	rc = mdb_txn_commit(txn);
#define HIDE_DOUBLE_FREE_BUG
#ifndef HIDE_DOUBLE_FREE_BUG
	rc = mdb_txn_commit(txn);
/*
mdb.c:3470 		rc = MDB_BAD_TXN;
mdb.c:3471		goto fail; // Jumps away here due to double-commit...
...
mdb.c:5076	free(env->me_dirty_list);
mdb.c:5076	free(env->me_txn0); // Double-free crashes here...
*/
#endif
	if (rc) {
		fprintf(stderr, "mdb_txn_commit: (%d) %s\n", rc, mdb_strerror(rc));
		goto leave;
	}
	rc = mdb_txn_begin(env, NULL, MDB_RDONLY, &txn);
	rc = mdb_cursor_open(txn, dbi, &cursor);
	while ((rc = mdb_cursor_get(cursor, &key, &data, MDB_NEXT)) == 0) {
		int* k=(int*)key.mv_data;
		int* d=(int*)data.mv_data;
		printf("key: '%d', data: '%d'\n",*k,*d);
/*
#if 1
		printf("key: '%s', data: '%s'\n",
			(char *) key.mv_data,
			(char *) data.mv_data);
#else
		printf("key: %p '%.*s', data: %p '%.*s'\n",
			key.mv_data,  (int) key.mv_size,  (char *) key.mv_data,
			data.mv_data, (int) data.mv_size, (char *) data.mv_data);
#endif
*/
	}
	// key: 00865FDC '020 ', data: 00865FE0 '020 3141592 foo bar'
	// key: '020 020 3141592 foo bar', data: '020 3141592 foo bar'
	mdb_cursor_close(cursor);
	mdb_txn_abort(txn);
leave:
	mdb_close(env, dbi);
	mdb_env_close(env);
	return 0;
}