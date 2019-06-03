#!/bin/bash

for file in *.c; do
	file="$(basename "$file")"
	echo "mv ${file} ${file}pp"
	mv ${file} ${file}pp
done
