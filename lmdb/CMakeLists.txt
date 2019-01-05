# libunistd/lmdb/CMakeLists.txt

# git clone git://git.openldap.org/openldap.git --branch OPENLDAP_REL_ENG_2_4

set(NAME liblmdb)
set(LDAP liblmdb)
message("--- Building ${NAME} ---")

set(LIBLMDB
	${LDAP}/lmdb.h
	${LDAP}/midl.h
	${LDAP}/mdb.c
	${LDAP}/midl.c
)

set(SOURCES
	${LIBLMDB}
	Cursor.h
	Datum.h
	Db.h
	Transaction.h
	Db.cpp
)

#add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
#necessary for building mdb_dkey for mtest6:
add_definitions(-DMDB_DEBUG)

#For detecting double-free():
#add_definitions(-DPRINT_MALLOC)

set(INCDIR 
	${CMAKE_CURRENT_SOURCE_DIR}/../lmdb/liblmdb
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_library(${NAME} ${SOURCES})
target_include_directories(${NAME} PUBLIC ${INCDIR})
add_subdirectory(test)