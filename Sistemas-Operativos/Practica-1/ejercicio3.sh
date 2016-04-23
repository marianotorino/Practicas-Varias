#!/bin/bash

files=()

for file in $(ls -R)
do
	if [ -x $file ] ; then
		files+=($file)
	fi
done

echo ${files[@]} | sort -r

cantidad(){
	echo ${#files[@]}
}
