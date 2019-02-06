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

AddSources() 
{	if [[ -e ${sources} ]]; then
		echo "Skipping... ${sources} already exists!"
		return
	fi
	echo Creating ${sources}...
	echo ls -1 *.h > ${sources}
	echo ls -1 *.cpp >> ${sources}
	echo ls -1 *.c >> ${sources}
}

AddSources
cat ${sources}


