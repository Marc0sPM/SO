#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <bits/getopt_posix.h>

int main(int argc, char *argv[]) {
    int mode = 0;
    int has_read = 0;
    int has_write = 0;
    char *filename = NULL;
    char *text = NULL;
    int opt;

    while((opt = getopt(argc, argv, "f:w:r")) != -1 ) {
        switch (opt)
        {
        case 'f':
            filename = optarg;
            break;
        case 'r' : 
            has_read = 1;
            break;
        case 'w':
            has_write = 1;
            text = optarg;
            break;
        default:
            fprintf(stderr, "Uso: %s -f <filename> [-r] [-w]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }


    if(!filename || (!has_read && !has_write)) {
        // printf("FLAG: %u", flags);
        fprintf(stderr, "Debe especificar un fichero con -f y al menos una opcion de apertura -r o -w.\n");
        return EXIT_FAILURE;
    }

    if(has_read && has_write) {
        mode |= O_RDWR | O_CREAT | O_TRUNC;
    } else if(has_read) {
        mode = O_RDONLY;
    } else if (has_write) {
        mode |= O_WRONLY | O_CREAT | O_TRUNC;
    }

    int fd = open(filename, mode, 0644);
    if(fd == -1 ) {
        perror("Error al abrir el fichero");
        return EXIT_FAILURE;
    }
    if(has_write && text) {
        if(write(fd, text, strlen(text)) == -1) {
            perror("Error al escribir en el fichero");
        } else {
            printf("Escritura realizada con exito\n");
        }
    }

    if(lseek(fd, 0, SEEK_SET) == -1) {
        perror("Error al mover el puntero del archivo al inicio");
        close(fd);
        return EXIT_FAILURE;
    }

    char buffer[100];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if(bytes_read == -1) {
        perror("Error al leer el fichero");
    } else {
        buffer[bytes_read] = '\0';
        printf("Lectura realizada con exito: %s\n", buffer);
    }

    if(close(fd) == -1) {
        perror("Error al cerrar el fichero");
    }
    return 0;
}