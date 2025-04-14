#!/bin/bash

nombre_carpeta="carpetaPrueba"
nombre_archivo="archivoPrueba.txt"

mkdir $nombre_carpeta 
cd $nombre_carpeta
echo "texto de prueba para incluir" > "$nombre_archivo"
cd ..
rm -r $nombre_carpeta

exit