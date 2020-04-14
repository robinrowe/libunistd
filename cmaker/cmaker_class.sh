#!/bin/bash 
# cmaker_class.sh
# Created by Robin Rowe 2019/1/10
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

ReadLicenseFile()
{	if [[ ! -e LICENSE ]]; then
		echo "Missing LICENSE file"
		exit 1
	fi
	read -r license < LICENSE
	echo "License: ${license}"
}

Sed()
{	local arg=$1
	local file=$2
	sed "${arg}" ${file} > ${file}.tmp
	mv -f ${file}.tmp ${file}
}

CreateFile() 
{	local src=$1
	local dst=$2
	local arg=$3
	lower=$3
	lower="${lower,}"
	if [ "$lower" = "$3" ]; then
		echo "FAILED! Can't lowercase $3"
		exit
	fi
	echo Creating ${dst}...
	cp ${src} ${dst}
	Sed "s|OBJECT|${lower}|g" ${dst}
	Sed "s|CLASS|${arg}|g" ${dst}
	Sed "s|DATE|${date}|g" ${dst}
	Sed "s|AUTHOR|${AUTHOR}|g" ${dst}
	Sed "s|LICENSE|${license}|g" ${dst}
}

UpdateCmakeList()
{	local arg=$1
	echo "Updating ${cmakelist} with $arg..."
	echo "add_executable(test_${arg} test/test_${arg}.cpp)" >> ${cmakelist}
	echo "add_test(test_${arg} test_${arg})" >> ${cmakelist}
}

UpdateCmakeSources()
{	local arg=$1
	if [[ ! -e ${sources} ]]; then
		echo ${sources} > ${sources}
	fi
	echo "${arg}.h" >> ${sources}
	echo "${arg}.cpp" >> ${sources}
}

CreateDocs()
{	local docfile=CHANGES.md
	echo "cmaker: class ${arg}" >> ${docfile}
}

main()
{	if [ -z "$AUTHOR" ]; then 
		echo "In bash set your name: % export AUTHOR=\"Your Name\""
		exit 1
	fi
	ReadLicenseFile
	if [[ ! -e test ]]; then
		mkdir test
	fi
	for arg in "${args[@]}"; do
		if [[ -e ${arg}.h ]]; then
			echo "Skipping... ${arg}.h already exists!"
			continue
		fi
		CreateFile ${h_file} "./${arg}.h" ${arg}
		CreateFile ${cpp_file} "./${arg}.cpp" ${arg}
		CreateFile ${test_file} "./test/test_${arg}.cpp" ${arg}
		UpdateCmakeList $arg
		UpdateCmakeSources $arg
		CreateDocs
	done
}

main

