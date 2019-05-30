#!/bin/sh
# rename_cpp.sh
# Copyright 2019/5/27 Robin.Rowe@Cinepaint.org
# License open source MIT

RenameCpp()
{	printf '%s\n' "$1"
	mv $1 $1pp
}

Walkdir()
{	for pathname in "$1"/*; do
		if [ -d "$pathname" ]; then
			Walkdir "$pathname"
		elif [ -e "$pathname" ]; then
			case "$pathname" in *.c)
				RenameCpp $pathname
			esac
		fi
	done
}

Walkdir .
find . -name "*.cpp"
