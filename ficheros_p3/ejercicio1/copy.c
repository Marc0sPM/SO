#include <stdio.h>
#include <stdlib.h>
//libreria para las flags
#include <fcntl.h>
#include <unistd.h>
//libreria para permisos de sistema
#include <sys/stat.h>

const size_t BLOCK_SIZE = 512; // Tamaño de bloque intermedio para copia

void copy(int fdo, int fdd)
{
	char buffer[BLOCK_SIZE];
	// uso de ssize_t para enteros con valores negativos en sistemas POSIX
	ssize_t bytes_read, bytes_written;

	//metodo read(valor de apertura, buffer, tamaño buffer) returns ssize_t
	// Si devuelve 0 --> final de archivo (EOF)
	// Si devuelve -1 --> error de lectura
	while((bytes_read = read(fdo, buffer, BLOCK_SIZE)) > 0) {
		//Se escribe en dst_file los datos leidos en org_file
		bytes_written = write(fdd, buffer, bytes_read);
	
		if(bytes_read != bytes_written) {
			perror("Error escribiendo en el archivo destino");
			exit(EXIT_FAILURE);
		}
	}
	if(bytes_read == -1) {
		perror("Error al leer el archivo origen");
		exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[])
{
	if(argc != 3) {
		fprintf(stderr, "Usage: %s <fichero_origen> <fichero_destino>", argv[0]);
		return EXIT_FAILURE;
	}
	int fdo = open(argv[1], O_RDONLY);
	if (fdo < 0) {
		perror("Error al abrir el archivo origen");
		return EXIT_FAILURE;
	}


	//se abre archivo destino, si no hay se crea y si hay se trunca 
	// y se le dan permisos de lectura y escritura
	int fdd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if(fdd < 0) {
		perror("Error al abrir archivo destino");
		return EXIT_FAILURE;
	}

	copy(fdo, fdd);
	close(fdo);
	close(fdd);
	return 0;
}
