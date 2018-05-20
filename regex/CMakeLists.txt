# libunistd/lmdb/CMakeLists.txt

set(NAME libregex)
message("--- Building ${NAME} ---")
set(SOURCES
	regex.h
	regex.c
)

add_library(${NAME} ${SOURCES})
