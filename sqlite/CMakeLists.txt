# libunistd/sqlite/CMakeLists.txt

set(NAME libsqlite)
message("--- Building ${NAME} ---")
set(SOURCES
	source/sqlite3.h
	source/sqlite3ext.h
	source/sqlite3.cpp
)

add_library(${NAME} ${SOURCES})
add_executable(sqlite ${SOURCES} shell.cpp)

