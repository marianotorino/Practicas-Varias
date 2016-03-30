#!/bin/bash

if [ $# -gt 0 ] ; then
	while [ true ]
	do
		echo $(expr $(ps -aux | grep $1 | wc -l) - 1)
		sleep 15
	done
fi
