#!/bin/bash

for file in *.c; do
	file="$(basename "$file")"
	echo "git mv ${file} ${file}pp"
	git mv ${file} ${file}pp
done
