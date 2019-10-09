#!/bin/bash 
# cmaker_sources.sh
# Created by Robin Rowe 2019/2/6
# License MIT open source

sources=sources.cmake
cmakelist=CMakeLists.txt
date=$(date +%Y-%m-%d)
library=$1

ReadLicenseFile()
{	if [[ ! -e LICENSE ]]; then
		echo "Missing LICENSE file"
		exit 1
	fi
	read -r license < LICENSE
	echo "License: ${license}"
}

AddPath()
{	shopt -s nullglob
	local files=($1)
	local target=$2
	echo "files = ${files}"
	for file in "${files[@]}"
	do 
		if [ -f "$file" ]; 
		then 
			echo ${file} >> ${target}
	    fi 
	done
}

AddSources() 
{	if [[ -e ${sources} ]]; then
		echo "Skipping... ${sources} already exists!"
		return
	fi
	echo Creating ${sources}...
	echo ${sources} >> ${sources}
	AddPath "*.h" ${sources}
	AddPath "*.H" ${sources}
	AddPath "*.cpp" ${sources}
	AddPath "*.cxx" ${sources}
	AddPath "*.c" ${sources}
}

UpdateCmakeList()
{	if [[ -e ${cmakelist} ]]; then
		echo "${cmakelist} already exists!"
	else
		echo Creating ${cmakelist} for library ${library}...
		echo "# ${library}/CMakeLists.txt" >> ${cmakelist}
		echo "# Created by ${AUTHOR} ${date}" >> ${cmakelist}
		echo "# License: ${license}" >> ${cmakelist}
		echo "" >> ${cmakelist}
		echo "project(${library})" >> ${cmakelist}
		echo "file(STRINGS sources.cmake SOURCES)" >> ${cmakelist}
	fi
	echo "add_library(${library} \${SOURCES})" >> ${cmakelist}
}

main()
{	if [ -z "${library}" ]; then 
		echo 'Usage: % cmaker_sources.sh library_name'
		exit 1
	fi
	if [ -z "$AUTHOR" ]; then 
		echo "In bash set your name: % export AUTHOR=\"Your Name\""
		exit 1
	fi
	echo "cmaker_sources: $PWD"
	ReadLicenseFile
	AddSources
	UpdateCmakeList
	#cat ${sources}
}

main


