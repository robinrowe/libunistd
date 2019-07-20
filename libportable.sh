#!/bin/bash

#/c/code/github/libunistd/build/win64/libuuid/Debug/libuuid.lib
#/c/code/github/libunistd/build/win64/lmdb/Debug/liblmdb.lib
#/c/code/github/libunistd/build/win64/portable/Debug/libportable.lib
#/c/code/github/libunistd/build/win64/regex/Debug/libregex.lib
#/c/code/github/libunistd/build/win64/sqlite/Debug/libsqlite.lib
#/c/code/github/libunistd/build/win64/unistd/Debug/libunistd.lib
#/c/code/github/libunistd/build/win64/xxhash/Debug/libxxhash.lib

libs=( libuuid liblmdb libportable libregex libsqlite libxxhash)

EchoLibs()
{	local lib=$1
	local platform=$2
	local PLATFORM=$3
	local debug=$4
	local release=$5
	local LIB=${lib^^}
	echo "set(${LIB}_INCLUDE_DIR \${CMAKE_CURRENT_LIST_DIR})"
	echo "set(${LIB}_INCLUDE_DIRS \${${LIB}_INCLUDE_DIR})"
	echo "if(${PLATFORM})"
	echo "	set(${LIB}_LIBRARY"
	echo "	debug ${CMAKE_CURRENT_LIST_DIR}/build/${platform}/${debug}/${lib}.lib"
	echo "	optimized ${CMAKE_CURRENT_LIST_DIR}/build/${platform}/${release}/${lib}.lib )"
	echo "endif(${PLATFORM})"
	echo "set(${LIB}_LIBRARIES \${${LIB}_LIBRARY})"
	echo ""
}
	
for lib in "${libs[@]}"
do
	EchoLibs ${lib} win64 WIN32 unistd/Debug unistd/Release

	
done