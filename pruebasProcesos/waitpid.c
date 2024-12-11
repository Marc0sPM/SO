#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    pid_t pid = fork(); // Crea un nuevo proceso

    if (pid == -1) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // Este bloque es ejecutado por el proceso hijo
        printf("Soy el proceso hijo, me voy a detener...\n");
        pause(); // El hijo se queda detenido aquí hasta recibir una señal
        printf("El hijo continúa después de ser reanudado\n"); // Esto no se ejecuta hasta que se reciba SIGCONT
        exit(0);
        
        return 0; // El hijo finaliza normalmente
    } else {
        // Este bloque es ejecutado por el proceso padre
        int status;
        sleep(1); // Espera un momento para asegurarse de que el hijo esté en estado detenido
        kill(pid, SIGSTOP); // Envía la señal SIGSTOP al hijo (detiene el proceso hijo)
        printf("El hijo ha sido detenido\n");

        // El padre puede hacer otras tareas aquí si es necesario

        // Ahora reanuda el proceso hijo
        sleep(2);
        kill(pid, SIGCONT); // Reanuda el hijo
        printf("El hijo ha sido reanudado\n");

        // se debe de enviar algun tipo de señal para que el hijo salga del pause(), 
        // por ello, no puede terminal normalmente

        // Ahora esperamos que el hijo termine
        pid_t child_pid = waitpid(pid, &status, 0); // Espera por el hijo

        if (child_pid == -1) {
            perror("waitpid failed");
            return 1;
        }

        // Verifica cómo terminó el hijo
        if (WIFEXITED(status)) {
            printf("El hijo terminó normalmente con código de salida %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("El hijo terminó debido a una señal %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("El hijo fue detenido por la señal %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("El hijo fue reanudado\n");
        }
    }

    return 0;
}
