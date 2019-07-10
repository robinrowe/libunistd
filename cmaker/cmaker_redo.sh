#!/bin/bash 
# cmaker_project.sh
# Created by Robin Rowe 2019/7/10
# License MIT open source

project=$1

AppendFiles()
{	echo "Appending old files to new files..."
	for filename in *; do
		if [ -d ${filename} ] ; then
			echo "Skipping ${filename}"
			continue
		fi
		local oldfile="$1/${filename}"
		if [ -f ${oldfile} ]; then
			echo "${oldfile} >> ${filename}"
			cat ${oldfile} >> ${filename}
		else
			echo "Not found: ${oldfile}"
		fi
	done
}

main()
{	echo "cmaker_redo: $PWD"
	if [ -z "${project}" ]; then 
		echo 'Usage: % cmaker_redo OldDir'
		exit 1
	fi
	if [ -z "$AUTHOR" ]; then 
		echo "In bash set your name: % export AUTHOR=\"Your Name\""
		exit 1
	fi
	AppendFiles ${project}
}

main
