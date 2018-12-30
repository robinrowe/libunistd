# libunistd/lmdb/CMakeLists.txt

set(NAME liblmdb)
message("--- Building ${NAME} ---")
set(SOURCES
	source/lmdb.h
	source/mdb.c
	source/midl.c
	source/midl.h
	Cursor.h
	Datum.h
	Db.h
	Transaction.h
	Db.cpp
)

add_library(${NAME} ${SOURCES})
add_subdirectory(test)