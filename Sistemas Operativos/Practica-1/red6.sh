#!/bin/bash

if [ $# -lt 2 ] ; then
	echo Debe especificar el modo de ejecucion y un nickname
	exit 1
fi

if [ $1 == 's' ] ; then
	nc -k -l 127.0.0.1 1234
elif [ $1 == 'c' ] ; then
	nc 127.0.0.1 1234
else
	echo Modo de ejecucion incorrecto
	exit 2
fi
