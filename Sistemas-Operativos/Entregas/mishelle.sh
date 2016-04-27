#!/bin/bash

clear

prompt="@"$(whoami)">"

export RUTA="$SO/externalSO;/bin;/usr/bin;/usr/local/bin"

#Debug mode on/off
dm=false
if [[ $# -gt 0 && $1 == "-d" ]] ; then
	DEBCOLOR='\033[1;32m'
	NOCOLOR='\033[0m'
	dm=true
	echo -e ${DEBCOLOR}DEBUG MODE ON${NOCOLOR}
fi

function debugprint(){
	if [ "$dm" = true ] ; then
		echo -e "${DEBCOLOR}DEBUG${NOCOLOR}: $1"
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
			if [[ -d $file ]] ; then			
				data=$(ls -ld $file)
			else
				data=$(ls -l $file)
			fi
			hexperm=$(stat -c "%a" $(echo $data | cut -d" " -f9))
			echo ${data:0:1}$hexperm${data:10}
		done
	else
		for dir in ${*:2}
		do
			if [[ -d $dir && -r $dir ]] ; then
				debugprint "files in $dir"
				echo $dir:			
				ls -1 $dir $reverse &
				wait
			else
				echo El parametro $dir no es un directorio o no posee permisos.
			fi
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

mkdirfunc(){
	if [ $# -gt 0 ] ; then
		if [ ${1:0:1} == "/" ] ; then
			debugprint "es un directorio absoluto"
			dir=$1
		else
			debugprint "es un directorio relativo"
			dir="$(pwd)/"$1
		fi
		debugprint "creando al directorio $dir"
		dashp="false"
		index=1
		args=${@:1}
		for param in ${@:1}
		do
			if [ $param == "-p" ] ; then			
				debugprint "encontrado el parametro -p"				
				indexp=$index
				debugprint "argumentos: $(echo $args | tr '\n' ' ' )"
				newargs=$(echo ${args//-p} | tr '\n' ' ')
				debugprint "nuevos argumentos: $nweargs"				
				dashp="true"
				break
			fi
			let index++
		done
		if [ "$dashp" == "true" ] ; then
			aux=$dir
			while [ ! -e $aux ]
			do
				directories[]=$aux
				aux=$(echo $aux | rev | cut -d"/" -f2- | rev)
			done
			for path in directories
			do
				mkdirfunc $path ${@:1} #pero sin -p
			done
		else
			if [[ -w $(echo $dir | rev | cut -d"/" -f2- | rev) ]]
			then
				mkdir $dir ${@:2} 2> /dev/null
				if [ $? -ne 0 ] ; then
					echo No se pudo crear el directorio
				fi
			else
				echo No tiene permiso de escritura en el directorio
			fi
		fi
	else
		echo Debe especificar un directorio para crear
	fi
}

default(){
	if [ $(type -f $1 2> /dev/null | grep "builtin" | wc -l) -eq 1 ]
	then
		debugprint "comando interno de bash"
		command $* 
	elif [[ ${1:0:2} == "./" && -x $1 ]]
	then
		debugprint "comando con ./"
		$* &
	else
		noexists=true
		for path in $(echo $RUTA | tr ";" "\n" | tac)
		do
			debugprint "buscando el comando en la ruta $path"
			debugprint $path/$1
			if [ -x $path/$1 ]
			then
				debugprint "encontrado"
				noexists=false
				$path/$* &
				wait
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
	if [ -z "$cmd" ] ; then
		continue
	fi
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
		mkdirfunc $args ;;
		* )
		default $cmd $args
	esac

done
