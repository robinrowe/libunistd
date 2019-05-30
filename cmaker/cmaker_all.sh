#!/bin/sh
# cmaker_all.sh
# Copyright 2019/5/27 Robin.Rowe@Cinepaint.org
# License open source MIT

Walkdir()
{	echo "Walkdir $1"
	for pathname in "$1"/*; do
		if [ -d "$pathname" ]; then
			dir="${pathname##*/}"
			echo "Walking ${pathname} ${dir}..."
			cd ${pathname};cmaker_project.sh ${dir};cmaker_sources.sh
			Walkdir "${pathname}"
		fi
	done
}

cmaker_top.sh "${PWD##*/}"
Walkdir "${PWD}"
echo "List files created..."
find . -name CMakeLists.txt
find . -name sources.cmake
