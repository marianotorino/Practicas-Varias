#!/bin/bash

if [ -d $HOME/backup ] ; then
	rm -R $HOME/backup
fi

mkdir $HOME/backup

for file in $(ls $HOME)
do
	if [ -f $file ] ; then
		cp $HOME/$file $HOME/backup/$(echo $file | tr a-z A-Z)
	fi
done
