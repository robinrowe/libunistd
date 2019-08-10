# libunistd/xxhash/CMakeLists.txt

project(libxxhash)
message("--- Building library ${PROJECT_NAME} ---")

set(SOURCES
	xxhash.h
	xxhash.c
	xxhsum.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
