#!/bin/bash

for file in $(ls -1)
do
	if [ -d $file ] ; then
		cd $file
		n=1
		for ph in $(find . -maxdepth 1 -iname "*.jpg" | cut -d/ -f2)
		do
			mv $ph "$file-$n.jpg"
			n=$(( $n + 1 ))
		done
		cd ..
	fi
done
