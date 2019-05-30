#!/bin/bash 
# cmaker_library.sh
# Created by Robin Rowe 2019/4/22
# License MIT open source

cmakelist=CMakeLists.txt
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

UpdateCmakeList()
{	name=$1
	echo "Updating ${cmakelist} with library ${name}..."
	echo "add_library(${name} STATIC ${name}.cpp)" >> ${cmakelist}
	echo '' >> ${cmakelist}
}

main()
{	if [ -z "$AUTHOR" ]; then 
		echo "In bash set your name: % export AUTHOR=\"Your Name\""
		exit 1
	fi
	ReadLicenseFile
	for arg in "${args[@]}"; do
		UpdateCmakeList $arg
	done
}

main

