#!/bin/bash

if [ $# -lt 2 ] ; then
	echo Debe especificar el modo de ejecucion y un nickname
	exit 1
fi

if [ $1 == 's' ] ; then
	while true ;
	do
		read message
		printf "$(date) %s dice:\r\n%s\r\n" $2 $message
	done | nc -k -l 127.0.0.1 1234
elif [ $1 == 'c' ] ; then
	while true ;
	do
		read msg
		printf "$(date) %s dice:\r\n%s\r\n" $2 $msg
	done | nc 127.0.0.1 1234
else
	echo Modo de ejecucion incorrecto
	exit 2
fi
