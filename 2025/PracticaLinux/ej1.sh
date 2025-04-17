#!/bin/bash

function Error(){
	echo "Error, no se ingreso ningun parametro"
}

if test $# -lt 1
then
	Error
else
	usuario=$1
	echo "Hola $usuario"
fi

exit
