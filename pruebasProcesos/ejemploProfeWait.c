#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid1, pid2;
    int status;

    pid1 = fork(); // Crea el primer proceso hijo

    if (pid1 == -1) {
        perror("fork failed");
        return 1;
    }

    if (pid1 == 0) { // Proceso hijo 1
        execlp("ls", "ls", NULL); // Ejecución de "ls"
        perror("execlp failed");
        return -1;
    } else {
        pid2 = fork(); // Crea el segundo proceso hijo

        if (pid2 == -1) {
            perror("fork failed");
            return 1;
        }

        if (pid2 == 0) { // Proceso hijo 2
            execlp("ls", "ls", "-l", NULL); // Ejecución de "ls -l"
            perror("execlp failed");
            return -1;
        } else {
            // El padre espera que terminen ambos hijos
            while (wait(&status) > 0);
            printf("Todos los hijos han terminado.\n");
        }
    }

    return 0;
}
