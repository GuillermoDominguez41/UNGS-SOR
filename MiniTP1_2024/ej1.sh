#!/bin/bash

function ErrorValidacionParametro(){
    echo "Error, debe ingresar un nombre de carpeta valido (alfanumerico y sin caracteres especiales)"
}

function ValidarParametro(){
    if [ $# -eq 1 ] && [[ $1 =~ ^[a-zA-Z0-9_-]+$ ]]; then
        return 0
    else
        return 1
    fi
}

function CrearCarpeta(){
    if [ -d "$nombre_carpeta" ]
    then
        echo "La carpeta ya existe"
    else
        mkdir "$nombre_carpeta"
        echo "Carpeta creada"
    fi
}

function BuscarArchivos(){
    local ruta=$1
    local extension=$2
    find "$ruta" -type f -name "*.$extension" -exec ls -l {} \; 2>/dev/null
}

function crearArchivo(){
    archivo_salida="listado_archivos.txt" > "$archivo_salida"

    # Buscamos todos los archivos con extensión .txt y listamos sus permisos.
    # Ademas, ocultamos la salida predeterminada del comando find.
    archivos_txt=$(BuscarArchivos / txt)
    archivos_listados=$(echo "$archivos_txt" | wc -l)
    fecha_hora="Fecha y hora: $(date)"

    if [ $archivos_listados -gt 1 ]; 
    then
        echo "Se encontraron archivos $archivos_listados .txt"
        echo "$archivos_txt" >> "$archivo_salida"
        echo "$fecha_hora" >> "$archivo_salida"
    else
        echo "No se encontraron archivos .txt se generan y listan 4 nuevos archivos"
        touch archivo1.txt
        touch archivo2.txt
        touch archivo3.txt
        touch archivo4.txt
        archivos_txt=$(BuscarArchivos . txt)
        echo "$archivos_txt" >> "$archivo_salida"
        echo "$fecha_hora" >> "$archivo_salida"
    fi

    #Incorporamos un aviso que vamos a mostrar el contenido del archivo y luego lo mostramos en pantalla
    echo "Contenido del archivo $archivo_salida:"
    cat "$archivo_salida"
}

if ValidarParametro $1;
then
    nombre_carpeta=$1
    CrearCarpeta
    cd $nombre_carpeta
    crearArchivo
else
    ErrorValidacionParametro
fi

exit