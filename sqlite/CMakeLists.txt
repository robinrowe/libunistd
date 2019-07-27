# libunistd/sqlite/CMakeLists.txt

project(libsqlite)
set(CMAKE_STATIC_LIBRARY_PREFIX "") #Avoid Linux "liblib__.a"
message("--- Building ${PROJECT_NAME} ---")

set(SOURCES
	source/sqlite3.h
	source/sqlite3ext.h
	source/sqlite3.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_executable(sqlite ${SOURCES} source/shell.c)
