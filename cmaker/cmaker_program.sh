#!/bin/bash 
# cmaker_program.sh
# Created by Robin Rowe 2019/1/11
# License MIT open source

cmakelist=CMakeLists.txt
date=$(date +%Y-%m-%d)
main_file="$0.main.cpp"
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

CreateMainCpp()
{	local src=$1
	local exe=$2
	local dst="$2.cpp"
	if [[ -e ${dst} ]]; then
		echo "Skipping... ${dst} already exists!"
		return
	fi
	echo Creating ${dst} ...
	cp ${src} ${dst}
	Sed "s/EXECUTABLE/${exe}/g" ${dst}
	Sed "s/DATE/${date}/g" ${dst}
	Sed "s/AUTHOR/${AUTHOR}/g" ${dst}
	Sed "s/LICENSE/${license}/g" ${dst}
}

UpdateCmakeList()
{	exe=$1
	echo "Updating ${cmakelist} with executable ${exe}..."
	echo "add_executable(${exe} ${exe}.cpp)" >> ${cmakelist}
#	echo '' >> ${cmakelist}
}

main()
{	if [ -z "$AUTHOR" ]; then 
		echo "In bash set your name: % export AUTHOR=\"Your Name\""
		exit 1
	fi
	ReadLicenseFile
	for arg in "${args[@]}"; do
		CreateMainCpp ${main_file} ${arg}
		UpdateCmakeList $arg
		echo "cmaker_program.sh ${arg}" >> CHANGES.md
	done
}

main

