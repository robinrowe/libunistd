#!/bin/bash
# cmaker_legacy.sh
# Created by Robin Rowe 2019/11/23
# License MIT open source

cmakelist=CMakeLists.txt
date=$(date +%Y-%m-%d)
project=$1
project_file="$0.txt"

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

CreateCmakeList() 
{	if [[ -e ${cmakelist} ]]; then
		echo "Skipping... ${cmakelist} already exists!"
		return
	fi
	if [[ -e ${cmakelist} ]]; then
		echo "Skipping... ${cmakelist} already exists!"
		return
	fi
	echo Creating ${cmakelist} for project ${project}...
	cp ${project_file} ${cmakelist}
	Sed "s/PROJECT/${project}/g" ${cmakelist}
	Sed "s/DATE/${date}/g" ${cmakelist}
	Sed "s/AUTHOR/${AUTHOR}/g" ${cmakelist}
	Sed "s/LICENSE/${license}/g" ${cmakelist}
}

AddSubdirs()
{	for dir in */; do
		dir=${dir%?};
		if [ "$dir" != "docs" ] && [ "$dir" != "license" ]; then
			echo "$dir"
			echo "add_subdirectory(${dir})" >> ${cmakelist}
		fi
	done
}

main()
{	if [ -z "${project}" ]; then 
		echo 'Usage: % cmaker_top Project_Name'
		exit 1
	fi
	if [ -z "$AUTHOR" ]; then 
		echo "In bash set your name: % export AUTHOR=\"Your Name\""
		exit 1
	fi
	ReadLicenseFile
	CreateCmakeList
	AddSubdirs
#	cat ${cmakelist}
}

main


CmakeFiles()
{	for file in *.cpp; do
		file="$(basename "$file")"
		if [ "$file" = "*.cpp" ]; then
			return
		fi
	#	pwd
		if [ ! -f "$FILE" ]; then
			echo "$FILE does not exist"
		fi
		echo "git mv ${file} ${file}pp"
		git mv ${file} ${file}pp
	done
}

Recurse() 
{	for d in *; do
	if [ -d "$d" ]; then
		(cd -- "$d" && Recurse)
	fi
	CmakeFiles
	done
}

Recurse