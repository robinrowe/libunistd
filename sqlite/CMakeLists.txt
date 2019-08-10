# libunistd/sqlite/CMakeLists.txt

project(libsqlite)
message("--- Building library ${PROJECT_NAME} ---")

set(SOURCES
	source/sqlite3.h
	source/sqlite3ext.h
	source/sqlite3.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_executable(sqlite ${SOURCES} source/shell.c)
