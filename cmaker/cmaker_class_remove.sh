#!/bin/bash 
# cmaker_class_remove.sh
# Created by Robin Rowe 2019/3/26
# License MIT open source

#set -x
h_file=$0.h
cpp_file=$0.cpp
test_file=$0.test.cpp
outstream=$0.OutStream.h
cmakelist=CMakeLists.txt
sources=sources.cmake
date=$(date +%Y-%m-%d)
args=("$@")

RemoveFile() 
{	local dst=$1
	echo Removing ${dst}...
	git rm ${dst}
}

main()
{	for arg in "${args[@]}"; do
		RemoveFile "./${arg}.h" 
		RemoveFile "./${arg}.cpp" 
		RemoveFile "./test/test_${arg}.cpp" 
	done
}

main

