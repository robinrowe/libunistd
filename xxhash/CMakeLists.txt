# libunistd/xxHash/CMakeLists.txt

set(HEADERS
	xxhash.h
)

set(SOURCES
	xxhash.c
	xxhsum.c
)

add_library( libxxhash
	${HEADERS} ${SOURCES}
)

target_include_directories (libxxhash PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")