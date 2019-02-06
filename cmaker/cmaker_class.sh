#!/bin/bash 
# make_class.sh
# Created by Robin Rowe 2019/1/10
# License MIT open source

#set -x
license='MIT open source'
h_file=$0.h
cpp_file=$0.cpp
test_file=$0.test.cpp
outstream=$0.OutStream.h
cmakelist=CMakeLists.txt
sources=sources.cmake
date=$(date +%Y-%m-%d)

if [ -z "$AUTHOR" ]; then 
	echo "In bash set your name: % export AUTHOR=\"Your Name\""
	exit 1
fi

LowerCase()
{	local c=${lower:0:1}
	local body=${lower:1}
	local octal=$(printf '%o' "'${c}")
	octal=$((octal + 40))
	c=$(printf '%b' '\'${octal})
	lower="${c}${body}"
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
	echo Creating ${dst} ...
	LowerCase
	cp ${src} ${dst}
	Sed "s|aCLASS|${lower}|g" ${dst}
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

if [[ ! -e test ]]; then
    mkdir test
fi

for arg; do 
	if [[ -e ${arg}.h ]]; then
		echo "Skipping... ${arg}.h already exists!"
		continue
	fi
	CreateFile ${h_file} "./${arg}.h" ${arg}
	CreateFile ${cpp_file} "./${arg}.cpp" ${arg}
	CreateFile ${test_file} "./test/test_${arg}.cpp" ${arg}
	UpdateCmakeList $arg
	UpdateCmakeSources $arg
done

