#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <linux/limits.h>

/* Forward declaration */
int get_size_dir(char *fname, size_t *blocks);

/* Gets in the blocks buffer the size of file fname using lstat. If fname is a
 * directory get_size_dir is called to add the size of its contents.
 */
int get_size(char *fname, size_t *blocks)
{
	struct stat st;
	if(lstat(fname, &st) == -1) {
		perror("Error en lstat de fichero");
		return -1;
	}

	*blocks += st.st_blocks;

	if(S_ISDIR(st.st_mode)) {
		return get_size_dir(fname, blocks);
	}
	return 0;
}


/* Gets the total number of blocks occupied by all the files in a directory. If
 * a contained file is a directory a recursive call to get_size_dir is
 * performed. Entries . and .. are conveniently ignored.
 */
int get_size_dir(char *dname, size_t *blocks)
{
	DIR *dir = opendir(dname);
    if (!dir) {
        perror("Error al abrir el directorio");
        return -1;
    }

    struct dirent *entry;
	//Leemos las entradas del directorio hasta que no haya mas
    while ((entry = readdir(dir)) != NULL) {
        // Ignorar las entradas `.` y `..`
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // crear la ruta completa del archivo
        char path[PATH_MAX];
		//sobreescribimos path con el formato "dir/file|dir"
        snprintf(path, sizeof(path), "%s/%s", dname, entry->d_name);

        struct stat st;
        if (lstat(path, &st) == -1) {
            perror("Error en lstat de entrada");
            closedir(dir);
            return -1;
        }

		//Sumamos lo que ocupa el archivo / directorio 
		*blocks += st.st_blocks;

		// Si es un subdirectorio calculamos el espacio 
        if (S_ISDIR(st.st_mode)) {
            // Llamada recursiva para el subdirectorio
            if (get_size_dir(path, blocks) == -1) {
                closedir(dir);
                return -1;
			}
		}
    }

    closedir(dir);
	return 0;
}

/* Processes all the files in the command line calling get_size on them to
 * obtain the number of 512 B blocks they occupy and prints the total size in
 * kilobytes on the standard output
 */
int main(int argc, char *argv[])
{
	if(argc < 2) {
		fprintf(stderr, "Uso: %s <fichero1> <fichero2> ... <ficheron>", argv[0]);
		return EXIT_FAILURE;
	}

	for(int i = 1 ; i < argc; i++) {
		size_t blocks = 0;
		if((get_size(argv[i], &blocks)) == -1) {
			continue;
		};

		size_t kilobytes = blocks / 2;
		printf("%luK %s\n", kilobytes, argv[i]);
	}
	return 0;
}