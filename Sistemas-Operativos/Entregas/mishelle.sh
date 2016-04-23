#!/bin/bash

clear

prompt="@"$(whoami)">"

export RUTA="$SO/externalSO;/bin;/usr/bin;/usr/local/bin"

#Debug mode on/off
dm=false
if [[ $# -gt 0 && $1 == "-d" ]] ; then
	DC='\033[1;32m'
	NC='\033[0m'
	dm=true
	echo -e ${DC}DEBUG MODE ON${NC}
fi

function debugprint(){
	if [ "$dm" = true ] ; then
		echo -e "${DC}DEBUG${NC}: $1"
	fi
}




function lsfunc(){
	reverse=""
	if [ $1 -eq 1 ]
	then
		reverse="-r"
	fi
	if [ $# -eq 1 ]
	then			
		ls -1 $reverse &
		wait
	elif [ $2 == "-l" ]
	then
		for file in $(ls $reverse)
		do
			debugprint "Haciendo ls -l de $file"
			data=$(ls -l $file)
			hexperm=$(stat -c "%a" $(echo $data | cut -d" " -f9))
			echo ${data:0:1}$hexperm${data:10}
		done
	else
		for dir in ${*:2}
		do
			debugprint "files in $dir"
			echo $dir:			
			ls -1 $dir $reverse &
			wait
		done
	fi
}

promptfunc(){
	if [ $# -eq 0 ]
	then
		prompt="@"$(whoami)">"
	elif [ $1 == "largo" ]
	then
		prompt="@YoSoy-"$(whoami)">"
	elif [ $1 == "uid" ]
	then
		prompt="@"$(id -u)">"
	else
		echo Parametros incorrectos
	fi
}

quiensoyfunc(){
	if [ $# -eq 0 ]
	then
		echo "Yo soy "$(whoami)
	elif [ $1 == "+h" ]
	then
		echo "Yo soy "$(whoami)" y estoy en la maquina "$(hostname)
	elif [ $1 == "+inos" ]
	then
		echo "Yo soy "$(whoami)" y tengo el user ID "$(id -u)
	else
		echo Parametros Incorrectos
	fi
}

default(){
	if [ $(type -f $1 2> /dev/null | grep "builtin" | wc -l) -eq 1 ]
	then
		debugprint "comando interno de bash"
		$* 
	elif [[ ${1:0:2} == "./" && -x $1 ]]
	then
		debugprint "comando con ./"
		$*
	else
		noexists=true
		for path in $(echo $RUTA | tr ";" "\n" | tac)
		do
			debugprint "buscando el comando en la ruta $path"
			debugprint $path/$1
			if [ -x $path/$1 ]
			then
				debugprint "encontrado en la ruta $path"
				noexists=false
				$path/$*
				break
			fi
		done
		if [ "$noexists" = true ] ; then
			echo "No se pudo encontrar el comando $1."
		fi
	fi
}

echo "            _     _     "
echo "  _ __ ___ (_)___| |__  "
echo " | '_ \` _ \| / __| '_ \\"
echo " | | | | | | \__ \ | | |"
echo " |_| |_| |_|_|___/_| |_|"
echo "                        "

while true; do

	read -p $(echo $prompt) -a line

	cmd=${line[0]}
	args=${line[@]:1}

	debugprint "ejecutando comando: $cmd con argumentos: $args"
	
	case $cmd in
		"ls" )
		lsfunc 0 $args ;;
		"sl" )
		lsfunc 1 $args ;;
		"cd" )
		eval cd $args 
		debugprint "cambio al directorio $(pwd)" ;;
		"pwd" )
		pwd ;;
		"cat" )
		cat $args &
		wait ;;
		"tac" )
		tac $args &
		wait ;;
		"prompt" )
		promptfunc $args ;;
		"quiensoy" )
		quiensoyfunc $args ;;
		"mkdir" )
		if [[ -w $(pwd) ]]
		then
			mkdir $args
		else
			echo No tiene permiso de escritura en el directorio actual
		fi ;;
		* )
		default $cmd $args
	esac

done
