#!/bin/bash

# Verificar que se hayan pasado dos argumentos
if [ $# -ne 2 ]; then
    echo "Uso: $0 <archivo_origen> <archivo_destino>"
    exit 1
fi

archivo_origen=$1
archivo_destino=$2

# Comando diff para comparar los archivos
echo "Comparando los archivos $archivo_origen y $archivo_destino con diff:"
diff $archivo_origen $archivo_destino
if [ $? -eq 0 ]; then
    echo "Los archivos son iguales."
else
    echo "Los archivos son diferentes."
fi

# Comando hexdump para ver el contenido en formato hexadecimal
echo -e "\nContenido hexadecimal del archivo origen ($archivo_origen):"
hexdump $archivo_origen

echo -e "\nContenido hexadecimal del archivo destino ($archivo_destino):"
hexdump $archivo_destino
