# libunistd/lmdb/CMakeLists.txt

project(libregex)
message("--- Building ${PROJECT_NAME} ---")
set(SOURCES
	regex.h
	regex.c
)

add_library(${PROJECT_NAME} ${SOURCES})
