#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <bits/getopt_posix.h>

pid_t launch_command(char** argv){
    /* To be completed */
    pid_t pid = fork();

    if(pid == 0) { // hijo 
        execvp(argv[0], argv);
        perror("execvp failed ! ");
        exit(EXIT_FAILURE);
    }
    if(pid > 0) {    // proceso padre
        int status;
        waitpid(pid,&status, 0);// esperamos a que el proceso hijo termine 
        return pid; 

    }
    return -1; // si el fork falla 
}


char **parse_command(const char *cmd, int* argc) {
    // Allocate space for the argv array (initially with space for 10 args)
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

    while (*start && isspace(*start)) start++; // Skip leading spaces

    while (*start) {
        // Reallocate more space if needed
        if (arg_count >= argv_size - 1) {  // Reserve space for the NULL at the end
            argv_size *= 2;
            argv = realloc(argv, argv_size * sizeof(char *));
            if (argv == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }

        // Find the start of the next argument
        end = start;
        while (*end && !isspace(*end)) end++;

        // Allocate space and copy the argument
        arg_len = end - start;
        argv[arg_count] = malloc(arg_len + 1);

        if (argv[arg_count] == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(argv[arg_count], start, arg_len);
        argv[arg_count][arg_len] = '\0';  // Null-terminate the argument
        arg_count++;

        // Move to the next argument, skipping spaces
        start = end;
        while (*start && isspace(*start)) start++;
    }

    argv[arg_count] = NULL; // Null-terminate the array

    (*argc)=arg_count; // Return argc

    return argv;
}


int main(int argc, char *argv[]) {
    char **cmd_argv;
    int cmd_argc, opt;
    int i;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s \"command\"\n", argv[0]);
        return EXIT_FAILURE;
    }
    cmd_argv=parse_command(argv[2], &cmd_argc);

    while((opt = getopt(argc, argv, "x:s:")) != -1) {
        switch(opt) {
            case 'x':

                launch_command(cmd_argv);

                break;
            case 's':
                FILE* file = fopen(optarg , "r");
                if(file == NULL) {
                    perror("ERROR al abrir file");
                    exit(EXIT_FAILURE);
                }
                char buff [1024];
                while(fgets(buff, sizeof(buff), file) != NULL) {
                    size_t len = strlen(buff);
                    if(len > 0 && buff[len - 1] == '\n') {
                        buff[len - 1] = '\0';
                    }

                    cmd_argv=parse_command(buff, &cmd_argc);

                    if(cmd_argv[0] != NULL) {
                        launch_command(cmd_argv);
                    }
                }

                for(int i = 0; cmd_argv[i] != NULL; i++) {
                    free(cmd_argv[i]);
                }
                free(cmd_argv); 
                fclose(file);
                // for(commands in fichero  [con fgets() ]) -> launch_command
                break;
        }


    }


    // Print parsed arguments
    printf("argc: %d\n", cmd_argc);
    for (i = 0; cmd_argv[i] != NULL; i++) {
        printf("argv[%d]: %s\n", i, cmd_argv[i]);
        free(cmd_argv[i]);  // Free individual argument
    }

    free(cmd_argv);  // Free the cmd_argv array

    return EXIT_SUCCESS;
}