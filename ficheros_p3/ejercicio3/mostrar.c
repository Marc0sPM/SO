#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <bits/getopt_core.h>


int main(int argc, char *argv[])
{
	int n = 0; 		// Numero de bytes a saltar
	int e_flag = 0; // Flag para leer los ultimos N bytes en caso
	int opt;

	while((opt = getopt(argc, argv, "n:e")) > 0) {
		switch (opt)
		{
		case 'n':
			n = atoi(optarg);
			break;
		case 'e':
			e_flag = 1;
			break;
		default:
		fprintf(stderr, "Usage: %s [-n N] [-e] <archivo>\n", argv[0]);
			return EXIT_FAILURE;
		}
	}

	//Si hay argumentos de mas
	if(optind >= argc) {
		fprintf(stderr, "Usage: %s [-n N] [-e] <archivo>\n", argv[0]);
			return EXIT_FAILURE;
	}

	const char *file_name = argv[optind];
	int fdo = open(file_name, O_RDONLY);
	if(fdo < 0) {
		perror("Error al leer el archivo");
		return EXIT_FAILURE;
	}

	off_t offset;
	if(e_flag) {
		offset = lseek(fdo, -n, SEEK_END);
	} else {
		offset = lseek(fdo, n, SEEK_SET);
	}
	if(offset == -1 ) {
		perror("Error en lseek");
		close(fdo);
		return EXIT_FAILURE;
	}

	char buffer;
	ssize_t bytes_read;
	//Leemos byte a byte y al mismo tiempo escribimos en la salida estandar
	while((bytes_read = read(fdo, &buffer, 1)) > 0) {
		if(write(STDOUT_FILENO, &buffer, 1) == -1) {
			perror("Error al escribir en la salida estandar");
			close(fdo);
			return EXIT_FAILURE;
		}
	}

	if(bytes_read == -1) {
		perror("Error al leer el archivo");
	}
	close(fdo);
	printf("\n");

	return EXIT_SUCCESS;
}
