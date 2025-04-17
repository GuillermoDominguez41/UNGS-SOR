#!/bin/bash

script="ej5.c"
compilado="ej"

gcc -o $compilado $script
./ej

# Adicionalmente eliminamos el archivo compilado luego de su ejecucion.
rm -r $compilado

exit