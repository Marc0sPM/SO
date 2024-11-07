#include <stdio.h>
#include <stdlib.h>
#include <err.h>


/**
 * @param buffer array donde se van a almacenar los datos leidos de forma temporal
 * @param num numero de bytes que se quiere leer cada iteracion
 * @param size tamaño del buffer 
 * @param stream file de la que se lee
 * fread(buffer, num, size, stream)
 * 
 * fwrite funciona igual pero asegurarse que el tamaño del buffer en la funcion sea la de los bytes de lectura
 */
#define BUFFER_SIZE 1024 // Tamaño de buffer para lectura de bytes

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	size_t readBytes, writeBytes;
	size_t buffer [BUFFER_SIZE];

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	/* Read file byte by byte */
	while ((readBytes = fread(buffer, 1, BUFFER_SIZE, file)) > 0 ) {
		/* Print byte to stdout */
		writeBytes = fwrite(buffer, 1, readBytes,  stdout);

		if (readBytes != writeBytes){
			fclose(file);
			err(3,"putc() failed!!");
		}
	}

	fclose(file);
	return 0;
}
