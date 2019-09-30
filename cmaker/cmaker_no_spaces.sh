#!/bin/bash
# no_spaces.sh 
# 2019/9/13 Robin.Rowe@cinepaint.org

echo "Renaming filenames that have spaces..."
for file in *
do
	file2="${file// /_}"
	if [ "$file" == "$file2" ]; then
		continue
	fi
	echo "Renaming $file => $file2"
	mv "$file" "$file2"
done
