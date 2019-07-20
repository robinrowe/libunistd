#!/bin/bash

#/c/code/github/libunistd/build/win64/libuuid/Debug/libuuid.lib
#/c/code/github/libunistd/build/win64/lmdb/Debug/liblmdb.lib
#/c/code/github/libunistd/build/win64/portable/Debug/libportable.lib
#/c/code/github/libunistd/build/win64/regex/Debug/libregex.lib
#/c/code/github/libunistd/build/win64/sqlite/Debug/libsqlite.lib
#/c/code/github/libunistd/build/win64/unistd/Debug/libunistd.lib
#/c/code/github/libunistd/build/win64/xxhash/Debug/libxxhash.lib

EchoHeader()
{	local name=$1
	local NAME=${name^^}
	local date=$(date +%Y-%m-%d)
	echo "# ${name}-config.cmake"
	echo "# Created by ${AUTHOR} ${date}"
	echo "# find_package(${NAME} REQUIRED)"
	echo "#   NAMES ${libs[*]}"
	echo "#   PATHS \${${NAME}_PATH} NO_DEFAULT_PATH"
	echo "# include_directories(\${${NAME}_PATH})"
	echo "# if(WIN32)"
	echo "# include_directories(\${${NAME}_INCLUDE_DIRS})"
	echo "#   link_directories(\${${NAME}_LIB_DIRS})"
	echo "#   link_libraries(\${${NAME}_LIBRARIES})"
	echo "# endif(WIN32)"
	echo ""
	echo ""
}

EchoLibs()
{	local lib=$1
	local platform=$2
	local PLATFORM=$3
	local debug=$4
	local release=$5
	local LIB="LIB${lib^^}"
	echo "# --- ${lib} ---"
	echo "set(${LIB}_INCLUDE_DIRS \${CMAKE_CURRENT_LIST_DIR})"
	echo "set(${LIB}_LIBRARIES lib${lib}.lib)"
	echo "set(${LIB}_LIB_DIRS"
	echo "	debug \${CMAKE_CURRENT_LIST_DIR}/build/${platform}/${lib}/${debug}"
	echo "	optimized \${CMAKE_CURRENT_LIST_DIR}/build/${platform}/${lib}/${release} )"
}
	
main()
{	if [ -z "$AUTHOR" ]; then 
		echo "In bash set your name: % export AUTHOR=\"Your Name\""
		exit 1
	fi
	libs=( portable uuid lmdb regex sqlite xxhash)
	EchoHeader ${libs[0]}
	for lib in "${libs[@]}"
	do
		EchoLibs ${lib} win64 WIN32 Debug Release
	done
}

main