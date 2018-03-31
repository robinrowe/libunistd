# libunistd/lmdb/CMakeLists.txt

set(NAME liblmdb)
message("--- Building ${NAME} ---")
set(SOURCES
	source/lmdb.h
	source/mdb.c
	source/midl.c
	source/midl.h
)

add_library(${NAME} ${SOURCES})
#target_include_directories (${NAME} "${CMAKE_CURRENT_SOURCE_DIR}/..")