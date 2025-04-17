#!/bin/bash

function errorValidacionParametro(){
    echo "Error, debe ingresar un nombre de carpeta valido (alfanumerico y sin caracteres especiales)"
}

function validarParametro(){
    if [ $# -eq 1 ] && [[ $1 =~ ^[a-zA-Z0-9_-]+$ ]]; then
        return 0
    else
        return 1
    fi
}

function crearCarpeta(){
    if [ -d "$nombre_carpeta" ]
    then
        echo "La carpeta ya existe"
    else
        mkdir "$nombre_carpeta"
        echo "Carpeta creada"
    fi
}

function crearArchivo(){
    local archivo_texto=$1
    > "$archivo_texto"

    local archivos_home=$(ls -la "$HOME" 2>/dev/null | grep -v '^total')
    local cantidad_archivos=$(echo "$archivos_home" | wc -l)
    local fecha_hora="Fecha y hora: $(date)"

    if [ $cantidad_archivos -gt 1 ]; then # Verificar si hay más de un archivo listado
        echo "Se encontraron $cantidad_archivos archivos en el directorio '$HOME'."
        echo "$archivos_home" >> "$archivo_texto"
        echo "$fecha_hora" >> "$archivo_texto"
    else
        echo "No se encontraron archivos en el directorio '$HOME'." >> "$archivo_texto."
        echo "$fecha_hora" >> "$archivo_sarchivo_textoalida"
    fi

    echo "El resumen con los archivos se encuentra en el archivo '$archivo_texto'."
}

if validarParametro $1;
then
    nombre_carpeta=$1
    archivo_salida="contenido_home.txt"

    cd "$HOME"
    crearCarpeta "$nombre_carpeta"
    cd $nombre_carpeta
    crearArchivo "$archivo_salida"
    cat "$archivo_salida"

    read -p "Presione Enter para finalizar..."
else
    errorValidacionParametro
fi

exit