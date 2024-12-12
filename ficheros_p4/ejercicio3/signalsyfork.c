#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

/*programa que temporiza la ejecución de un proceso hijo */

pid_t child_pid; 

// Manejador de la alarma, envia señal SIGKILL para reaunadar el proceso hijo, en este caso SIGKILL termina el proceso
void handle_alarm(int sig) {

	if(kill(child_pid, SIGKILL) == -1) {
		perror("Error al enviar SIGKILL al hijo");
		exit(EXIT_FAILURE);
	}
	printf("Señal SIGALRM recibida en el handler\n");
}



int main(int argc, char **argv)
{
	printf("NUm args: %u\n", argc);
	if(argc < 2) {
		fprintf(stderr, "Usage: ./%s <command>", argv[0]);
		return EXIT_FAILURE;
	}

	child_pid = fork();

 	if (child_pid == -1) {
        perror("Error en fork");
        return EXIT_FAILURE;
    }

	if(child_pid == 0) { // proceso del hijo
		if(execvp(argv[1], &argv[1]) == -1) {	
			perror("Error en execvp");
			return EXIT_FAILURE;
		}	

	} else{ // proceso del padre 
			struct sigaction sa;
			memset(&sa, 0, sizeof(sa));
			sa.sa_handler = handle_alarm;  // Establecemos el manejador
			sigaction(SIGALRM, &sa, NULL);


			//Ignoramos la señal SIGINT
			struct sigaction sa_sigint;
			memset(&sa_sigint, 0, sizeof(sa_sigint));
			sa_sigint.sa_handler = SIG_IGN;
			sigaction(SIGINT, &sa_sigint, NULL);

		// Establecemos la alarma para 5 segundos
			alarm(5); 


			int status;
			waitpid(child_pid, &status, 0);

			if (WIFEXITED(status)) {
            printf("El hijo terminó normalmente con código de salida %d.\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("El hijo terminó debido a una señal (señal %d).\n", WTERMSIG(status));
        } else {
            printf("El hijo terminó de manera inesperada.\n");
        }
	}
	return 0;
}
