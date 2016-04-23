#!/bin/bash

for OUTPUT in $(cat /etc/passwd | cut -d: -f1,3)
do
	uid=`echo $OUTPUT | cut -d: -f2`
	if [ $uid -gt 1000 ] ; then
		echo $OUTPUT | cut -d: -f1 >> /tmp/usuarios.txt
	fi
done
