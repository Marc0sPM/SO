#!/bin/bash

# Comprobamos si se ha pasado un argumento
if [ -z "$1" ]; then
    echo "Por favor, proporcione el nombre de un directorio."
    exit 1
fi

DIR="$1"

if [ -d "$DIR" ]; then 
    echo "El directorio '$DIR' ya existe. Eliminando todo su contenido..."
    rm -rf "$DIR"/* #Elimina todo el contenido del directorio
else
    echo "El directorio '$DIR' no exite. Creando el directorio..."
    mkdir "$DIR"
fi

cd "$DIR" || exit

#Creamos los ficheros necesarios
echo "Creando directio 'subdir'..."
mkdir subdir;

echo "Creando fichero regular 'fichero1'..."
touch fichero1

echo "Escribiendo en fichero2..."
echo "Hola me llamo Marcos" > fichero2

echo "Creando enlace simbolico enlaceS..."
ln -s fichero2 enlaceS

echo "Creando enlace duro enlaceH..."
ln fichero2 enlaceH

for file in *; do 
    if [ -e "$file" ]; then 
        echo "Atributos de '$file':"
        stat "$file"
        echo "========================================================="
    fi
done