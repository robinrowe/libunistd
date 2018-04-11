# libunistd/sqlite/CMakeLists.txt

set(NAME libsqlite)
message("--- Building ${NAME} ---")
set(SOURCES
	source/sqlite3.h
	source/sqlite3ext.h
	source/sqlite3.c
)

add_library(${NAME} ${SOURCES})

#shell.c
