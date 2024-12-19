#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 8192

// Function to copy a file from source path to destination path
void copy_file(const char *src_path, const char *dst_path) {
    // a completar utilizando las funciones read y write 
    // para leer del fichero fuente y escribir en el destino
    // la copia debe ser en bloques de BUFFER_SIZE, en ningún
    // caso con un buffer del tamaño total del archivo
    int src_fd, dst_fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    // Open source file
    src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) {
        perror("Error opening source file");
        return;
    }

    // Open destination file
    dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dst_fd < 0) {
        perror("Error opening destination file");
        close(src_fd);
        return;
    }

    // Copy file contents in chunks of BUFFER_SIZE
    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dst_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to destination file");
            close(src_fd);
            close(dst_fd);
            return;
        }
    }

    if (bytes_read < 0) {
        perror("Error reading from source file");
    }

    // Close file descriptors
    close(src_fd);
    close(dst_fd);
}



#define MAX_PATH_LENGTH 512 //podemos asumir que la ruta no supera esta longitud

// Function to recursively copy directories and files
void copy_directory(const char *src_dir, const char *dst_dir) {
    DIR* dir;
    struct dirent *entry; // Para iterar sobre las entradas del directorio
    char src_path[MAX_PATH_LENGTH]; // Buffer para construir las rutas completas
    char dst_path[MAX_PATH_LENGTH]; // Buffer para construir las rutas completas

    // crear el directorio objetivo con mkdir
    if (mkdir(dst_dir, 0755) < 0) {
        if (errno != EEXIST) {
            perror("Error creating destination directory");
            return;
        }
    }

    // luego, recorrer el directorio (ignorando las entradas . y ..)
    // Open source directory
    dir = opendir(src_dir);
    if (!dir) {
        perror("Error opening source directory");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // Construct full paths for source and destination
        // se puede utilizar, por ejemplo, snprintf para generar las rutas 
    // a subarchivos y subdirectorios.
        snprintf(src_path, MAX_PATH_LENGTH, "%s/%s", src_dir, entry->d_name);
        snprintf(dst_path, MAX_PATH_LENGTH, "%s/%s", dst_dir, entry->d_name);
        struct stat statbuf; // Declarar la variable statbuf

        if (stat(src_path, &statbuf) == -1) {
            perror("stat"); // Manejo de error si stat falla
            return;
        }
        // y copiar recursivamente todos los ficheros y directorios internos
        if (S_ISREG(statbuf.st_mode)) {
            // Regular file: copy it
            copy_file(src_path, dst_path);
        } else if (S_ISDIR(statbuf.st_mode)) {
            // Directory: copy it recursively
            copy_directory(src_path, dst_path);
        }
    }
    // Close directory
    if (closedir(dir) == -1) {
        perror("closedir");
        return -1;
    }
    // NOTA: no hay que copiar enlaces, dispositivos de caracteres, ni
    // ningún otro fichero especial, solo archivos y directorios
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_directory> <destination_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *src_dir = argv[1];
    const char *dst_dir = argv[2];

    copy_directory(src_dir, dst_dir);

    return EXIT_SUCCESS;
}
