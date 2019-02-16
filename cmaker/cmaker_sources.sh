#!/bin/bash 
# cmaker_sources.sh
# Created by Robin Rowe 2019/2/6
# License MIT open source

license="MIT open source"
sources=sources.cmake

if [ -z "$AUTHOR" ]; then 
	echo "In bash set your name: % export AUTHOR=\"Your Name\""
	exit 1
fi

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
	AddPath "*.h" ${sources}
	AddPath "*.H" ${sources}
	AddPath "*.cpp" ${sources}
	AddPath "*.cxx" ${sources}
	AddPath "*.c" ${sources}
}

AddSources
#cat ${sources}


