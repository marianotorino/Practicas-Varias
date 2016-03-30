#!/bin/bash

for site in $*
do
	nc $site 80 -w 1
	if [ ! $? -gt 0 ] ; then
		echo $site
	fi
done
