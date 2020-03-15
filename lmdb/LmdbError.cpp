// LmdbError.cpp
// Robin Rowe 2020/3/14

#include "LmdbError.h"

/** @defgroup  errors	Return Codes
 *
 *	BerkeleyDB uses -30800 to -30999, we'll go under them
 *	@{
**/

#define MDB_SUCCESS	 0
#define MDB_KEYEXIST	(-30799)
#define MDB_NOTFOUND	(-30798)
#define MDB_PAGE_NOTFOUND	(-30797)
#define MDB_CORRUPTED	(-30796)
#define MDB_PANIC		(-30795)
#define MDB_VERSION_MISMATCH	(-30794)
#define MDB_INVALID	(-30793)
#define MDB_MAP_FULL	(-30792)
#define MDB_DBS_FULL	(-30791)
#define MDB_READERS_FULL	(-30790)
#define MDB_TLS_FULL	(-30789)
#define MDB_TXN_FULL	(-30788)
#define MDB_CURSOR_FULL	(-30787)
#define MDB_PAGE_FULL	(-30786)
#define MDB_MAP_RESIZED	(-30785)
#define MDB_INCOMPATIBLE	(-30784)
#define MDB_BAD_RSLOT		(-30783)
#define MDB_BAD_TXN			(-30782)
#define MDB_BAD_VALSIZE		(-30781)
#define MDB_BAD_DBI		(-30780)

namespace lmdb {

void LmdbError::MapSymbols()
{	Map("Successful result",MDB_SUCCESS);
	Map("key/data pair already exists",MDB_KEYEXIST);
	Map("key/data pair not found (EOF)",MDB_NOTFOUND);
	Map("Requested page not found - this usually indicates corruption",MDB_PAGE_NOTFOUND);
	Map("Located page was wrong type",MDB_CORRUPTED);
	Map("Update of meta page failed or environment had fatal error",MDB_PANIC);
	Map("Environment version mismatch",MDB_VERSION_MISMATCH);
	Map("File is not a valid LMDB file",MDB_INVALID);
	Map("Environment mapsize reached",MDB_MAP_FULL);
	Map("Environment maxdbs reached",MDB_DBS_FULL);
	Map("Environment maxreaders reached",MDB_READERS_FULL);
	Map("Too many TLS keys in use - Windows only",MDB_TLS_FULL);
	Map("Txn has too many dirty pages",MDB_TXN_FULL);
	Map("Cursor stack too deep - internal error",MDB_CURSOR_FULL);
	Map("Page has not enough space - internal error",MDB_PAGE_FULL);
	Map("Database contents grew beyond environment mapsize",MDB_MAP_RESIZED);
	Map("Operation and DB incompatible, or DB type changed.",MDB_INCOMPATIBLE);
	Map("Invalid reuse of reader locktable slot",MDB_BAD_RSLOT);
	Map("Transaction must abort, has a child, or is invalid",MDB_BAD_TXN);
	Map("Unsupported size of key/DB name/data, or wrong DUPFIXED size",MDB_BAD_VALSIZE);
	Map("The specified DBI was changed unexpectedly",MDB_BAD_DBI);
}

}
