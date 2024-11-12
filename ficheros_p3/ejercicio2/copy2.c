#include <stdio.h>
#include <stdlib.h>
//libreria para las flags
#include <fcntl.h>
#include <unistd.h>
//libreria para permisos de sistema
#include <sys/stat.h>
#include <string.h>

const size_t BLOCK_SIZE = 512;

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

void copy_regular(char *orig, char *dest)
{
	int fdo = open(orig, O_RDONLY);
	if(fdo < 0) {
		perror("Error al abrir el archivo origen"); 
		exit(EXIT_FAILURE);
	}

	int fdd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (fdd < 0 ) {
		perror("Error al abrir el archivo destino");
		exit(EXIT_FAILURE);
	}

	copy(fdo, fdd);

	close(fdo);
	close(fdd);
}

void copy_link(char *orig, char *dest)
{
	struct stat sb;
	//Se sacan stat del link que queremos copiar
	if(lstat(orig, &sb) == -1) {
		perror("Error en lstat");
		exit(EXIT_FAILURE);
	}

	//Calculamos el espacion que ocupa el link y reservamos la memoria
	ssize_t link_size = sb.st_size + 1; // + 1 para añadir '\0'
	char * buffer = malloc(link_size);
	if(buffer == NULL) {
		perror("Error al asignar memoria");
		exit(EXIT_FAILURE);
	}

	//leemos el link que queremos copiar y lo ponemos en buffer
	ssize_t len = readlink(orig,  buffer, link_size);
	if(len == -1 ) {
		perror("Error en readlink");
		free(buffer);
		exit(EXIT_FAILURE);
	}
	//ponemos caracter de separacion
	buffer[len] = '\0';

	//creamos nuevo enlace simbolico a partir del buffer
	if(symlink(buffer, dest) == -1) {
		perror("Error en symlink");
		free(buffer);
		exit(EXIT_FAILURE);
	}
	//limpiamos memoria del buffer
	free(buffer);
}

int main(int argc, char *argv[])
{
	if(argc != 3) {
		fprintf(stderr, "Usage: %s <origen> <destino>", argv[0]);
		return EXIT_FAILURE;
	}

	struct stat sb;
	if((lstat(argv[1], &sb)) == -1) {
		perror("Error en lstat del archivo origen");
		return EXIT_FAILURE;
	}

	if(S_ISREG(sb.st_mode)) {
		copy_regular(argv[1], argv[2]);
	} else if(S_ISLNK(sb.st_mode)) {
		copy_link(argv[1], argv[2]);
	} else {
		fprintf(stderr, "Error: tipo de archivo no soportado\n");
	}

	return EXIT_SUCCESS;
}
