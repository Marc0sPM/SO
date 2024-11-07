#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

int main(int argc, char* argv[])
{
	FILE* file = NULL;

	//Que haya al menos un archivo destino y algunas cadenas de caracteres para escribir
	if(argc < 3) {
		fprintf(stderr, "Necesario introducir archivo destino y al menos una cadena para escribir");
		return 1;
	}
	//Se abre el stream de escritura
	if((file = fopen(argv[1], "w")) == NULL) {
		perror("Error al abrir el archivo");
		return 2;
	}

	//Se escribe uno a uno los caracteres
	for(int i = 2; i < argc; i++) {
		//strlen + 1 sirve para saber el tamaño del string y tener en cuenta el separador nulo '\0'
		size_t len = strlen(argv[i]) + 1;
		size_t written = fwrite(argv[i], 1, len, file);
		if(written != len){
			perror("Error al escribir en el archivo");
			fclose(file);
			return 3;
		}
	}
	fclose(file);
	return 0;
}
