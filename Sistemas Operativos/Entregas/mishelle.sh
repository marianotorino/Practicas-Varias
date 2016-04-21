#!/bin/bash

prompt="@"$(whoami)">"

export RUTA="/usr/bin;/bin;/var/lib/bin"

while true; do

	read -p $(echo $prompt) -a line

	case ${line[0]} in
		"ls" )
		if [ ${#line[@]} -lt 2 ]
		then
			ls -1
		elif [ ${line[1]} == "-l" ]
		then
			stat -c "%a %h %U %G %s %x %" *
		else
			echo Parametros incorrectos
		fi ;;
		"sl" )
		ls -1 -r ;;
		"cd" )
		cd ${line[1]} ;;
		"pwd" )
		pwd ;;
		"cat" )
		cat ${line[1]} ;;
		"tac" )
		tac ${line[1]} ;;
		"prompt" )
		if [ ${#line[@]} -eq 1 ]
		then
			prompt="@"$(whoami)">"
		elif [ ${line[1]} == "largo" ]
		then
			prompt="@YoSoy-"$(whoami)">"
		elif [ ${line[1]} == "uid" ]
		then
			prompt="@"$(id -u)">"
		else
			echo Parametros incorrectos
		fi ;;
		"quiensoy" )
		if [ ${#line[@]} -eq 1 ]
		then
			echo "Yo soy "$(whoami)
		elif [ ${line[1]} == "+h" ]
		then
			echo "Yo soy "$(whoami)" y estoy en la maquina "$(hostname)
		elif [ ${line[1]} == "+inos" ]
		then
			echo "Yo soy "$(whoami)" y tengo el user ID "$(id -u)
		else
			echo Parametros Incorrectos
		fi ;;
		"mkdir" )
		if [ -w $(pwd) ]
		then
			mkdir ${line[1]}
		else
			echo No tiene permiso de escritura en el directorio actual
		fi ;;
		* )
		if [ $(type ${line[0]} | grep "orden interna de la shell" | wc -l) -eq 1 ]
		then
			${line[@]}
		elif [ false #es un comando que empieza con ./ && -x ${line[0]} ]
		then
			${line[@]}
		else
			for path in $(echo $RUTA | tr ; \\n | tac)
			do
				if [ -x $path/${line[0]} ]
				then
					$path/${line[@]}
				fi
			done
			echo El comando ingresado no existe.
		fi
	esac

done
