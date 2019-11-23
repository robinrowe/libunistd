#!/bin/bash
# cmaker_to_cpp.sh
# Created by Robin Rowe 2019/7/10
# License MIT open source

RenameFiles()
{	for file in *.c; do
		file="$(basename "$file")"
		if [ "$file" = "*.c" ]; then
			return
		fi
	#	pwd
		echo "git mv ${file} ${file}pp"
		git mv ${file} ${file}pp
	done
}

Recurse() 
{	for d in *; do
	if [ -d "$d" ]; then
		(cd -- "$d" && Recurse)
	fi
	RenameFiles
	done
}

Recurse