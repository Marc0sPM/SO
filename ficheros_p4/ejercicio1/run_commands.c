#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/wait.h> // Para waitpid
#include <unistd.h>   // Para fork, execvp
#include <getopt.h>

#define MAX_CHILDREN 100 // Numero maximo de procesos hijos que puede ejecutar el programa

typedef struct{
    pid_t pid; 
    int cmd_number;

} ChildProcess;


pid_t launch_command(char** argv, int cmd_number) {
    printf("@@ Running command #%d:", cmd_number);
    for (int i = 0; argv[i] != NULL; i++) {
        printf(" %s", argv[i]);
    }
    printf("\n");

    pid_t pid = fork();

    if (pid == 0) { // Proceso hijo
        execvp(argv[0], argv);
        perror("execvp failed!");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) { // Proceso padre
        // En caso normal esperar al hijo desde aqui
        return pid;
    }

    perror("Fork failed");
    return -1; // Si fork falla
}

char **parse_command(const char *cmd, int* argc) {
    size_t argv_size = 10;
    const char *end;
    size_t arg_len;
    int arg_count = 0;
    const char *start = cmd;
    char **argv = malloc(argv_size * sizeof(char *));

    if (argv == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    while (*start && isspace(*start)) start++; // Saltar espacios iniciales

    while (*start) {
        if (arg_count >= argv_size - 1) {
            argv_size *= 2;
            argv = realloc(argv, argv_size * sizeof(char *));
            if (argv == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        end = start;
        while (*end && !isspace(*end)) end++;

        arg_len = end - start;
        argv[arg_count] = malloc(arg_len + 1);

        if (argv[arg_count] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(argv[arg_count], start, arg_len);
        argv[arg_count][arg_len] = '\0'; // Null-terminate
        arg_count++;

        start = end;
        while (*start && isspace(*start)) start++;
    }

    argv[arg_count] = NULL; // Null-terminate el array
    *argc = arg_count;
    return argv;
}

int main(int argc, char *argv[]) {
    int opt;
    ChildProcess children [MAX_CHILDREN];
    int bach = 0;
    int num_children = 0;

    if (argc < 3) {
            fprintf(stderr, "Usage: %s -x \"command\" OR %s -s \"file\"\n", argv[0], argv[0]);
            return EXIT_FAILURE;
    }

    while ((opt = getopt(argc, argv, "x:s:b")) != -1) {
        switch (opt) {
            case 'x': {
                int cmd_argc;
                char **cmd_argv = parse_command(optarg, &cmd_argc);
                launch_command(cmd_argv, 0);

                for (int i = 0; cmd_argv[i] != NULL; i++) {
                    free(cmd_argv[i]);
                }
                free(cmd_argv);
                break;
            }
            case 's': {
                FILE *file = fopen(optarg, "r");
                if (file == NULL) {
                    perror("Error al abrir el archivo");
                    return EXIT_FAILURE;
                }

                char buff[1024];
                int cmd_number = 0;
                while (fgets(buff, sizeof(buff), file) != NULL) {
                    size_t len = strlen(buff);
                    if (len > 0 && buff[len - 1] == '\n') {
                        buff[len - 1] = '\0'; // Eliminar el salto de línea
                    }

                    int cmd_argc;
                    char **cmd_argv = parse_command(buff, &cmd_argc);

                    if (cmd_argv[0] != NULL) {
                        // Vamos guardando los procesos hijos en nuestra lista children y llevando el contador de comandos cmd_number
                        if(bach) {
                            //Ejecutamos el comando pero sin esperar a que cada hijo termine
                            pid_t pid = launch_command(cmd_argv, cmd_number);
                            if(num_children < MAX_CHILDREN) {
                                children[num_children].pid = pid;
                                children[num_children].cmd_number = cmd_number;
                                num_children ++; 
                            }else{
                                fprintf(stderr, "Numero de procesos hijos excede el limite");
                                exit(EXIT_FAILURE);
                            }
                        }else {
                            launch_command(cmd_argv, cmd_number);
                        }
                        
                        cmd_number++;
                    }

                    for (int i = 0; cmd_argv[i] != NULL; i++) {
                        free(cmd_argv[i]);
                    }
                    free(cmd_argv);
                }

                fclose(file);
                break;
            }
            case 'b':
                bach = 1;
                break;
            default:
                fprintf(stderr, "Opción no válida\n");
                return EXIT_FAILURE;
        }
    }

    //Si tenemos que ejecutar todos los comandos en paralelo, simplemente le decimos al padre que espere a cada uno de los procesos hijos para terminar
    if (bach) {
        int status;
        for (int i = 0; i < num_children; i++) {
            pid_t pid = waitpid(children[i].pid, &status, 0);
            if (pid > 0) {
                printf("@@ Command #%d terminated (pid: %d, status: %d)\n",
                       children[i].cmd_number, pid, WEXITSTATUS(status));
            }
        }
    }

    return EXIT_SUCCESS;
}
