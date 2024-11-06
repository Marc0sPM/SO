#include <stdio.h>
#include <stdlib.h>
#include <err.h>

/**
 * Es necesario definir un buffer con un tamaño determinado para ejecutar el fread()
 * Tambien usar size_t para no limitar el espacio que se necesita
 * 
 * La condicion para el while de lectura es que los bytes que se acaban de leer sean mayores que 0
 * sino, eso significa que ya no se puede leer nada mas.
 * 
 * 
 * @param size tamaño de cada unidad que se lee (bytes)
 * size_t fread(void *ptr, size_t size, size_t count, FILE *stream); 
 * 
 * 
 * size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream);
 */


//Tamaño del buffer para la lectura
#define BUFFER_SIZE  1024

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	size_t bytesRead, bytesWritten;
	unsigned char buffer[BUFFER_SIZE];

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte */
	while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
		/* Print byte to stdout */
		bytesWritten = fwrite(buffer, 1, bytesRead, stdout);

		if (bytesWritten != bytesRead){
			fclose(file);
			err(3,"fwrite()) failed!!");
		}
	}

	fclose(file);
	return 0;
}
