#!/bin/bash

archivo_salida="resumenDelSistema.txt"

# Redirigir la salida al archivo
{
    echo "Resumen del sistema"
    echo "==================="
    echo "" 

    echo "Usuario:"
    whoami
    echo "" 

    echo "Fecha y hora:"
    date
    echo "" 

    echo "Kernel:"
    echo "$(uname -s) - $(uname -r)"
    echo "" 

    echo "Arquitectura:"
    uname -m
    echo "" 

    echo "CPU:"
    lscpu | grep 'Nombre del modelo:' | awk -F: '{print $2}' | sed 's/^ *//'
    echo ""

    echo "Primeras 10 interrupciones:"
    cat /proc/interrupts | head -n 11
    echo "" 

    echo "Memoria swap:"
    free -h | grep "Swap"
    echo "" 

    echo "Memoria principal:"
    free -h | grep "Mem:"
    echo "" 

    echo "Placa de video:"
    lspci | grep VGA

} > "$archivo_salida"

echo "La información del sistema se ha guardado en $archivo_salida"