#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#define M 10
#define SHM_NAME "/CALDERO"
#define SEM_EMPTY "/EMPTY"
#define SEM_FULL "/FULL"
#define SEM_MUTEX "/MUTEX"

sem_t	*full;
sem_t *empty;
int *pot = NULL; // Puntero a la memoria compartida
int shd;

int finish = 0;

void putServingsInPot(int servings)
{
	*pot = servings;
	sem_post(full);
	printf("[+]Cook (%d): Putting servings in the pot\n", getpid());
		
		
}

void cook(void)
{
	while(!finish) {

		//Esperamos a que el caldero este vacio
		sem_wait(empty);
		putServingsInPot(M);
		sleep(2);
	}
}

void cleanup() {
	munmap(pot, sizeof(int));
	sem_close(full);
	sem_close(empty);
	sem_unlink(SEM_EMPTY);
	sem_unlink(SEM_FULL);
	// sem_unlink(SEM_MUTEX);
	shm_unlink(SHM_NAME);
}

/* handler para liberacion de memoria cuando acaba el proceso*/
void handler(int signo)
{
	if(signo == SIGTERM || signo == SIGINT) {
	 	finish = 1;
		cleanup();
		printf("Cook (%d): Received signal, terminating\n", getpid());
		exit(EXIT_SUCCESS);
	}
}

int main(int argc, char *argv[])
{
	signal(SIGTERM, handler);
	signal(SIGINT, handler);

	// establecemos espacion de la memoria compartida
	shd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	 ftruncate(shd, sizeof(int));
	pot = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shd, 0);

	*pot = 0;

	empty = sem_open(SEM_EMPTY, O_CREAT, 0666, 0); // 1 porque al principio esta vacio el caldero
	full = sem_open(SEM_FULL, O_CREAT, 0666, 0); // a 0 porque no hay servings en el pot
	// sem_open(SEM_MUTEX, O_CREAT, 0666, 1);

	printf("Cook %d: Sarted\n", getpid());
	cook();

	// cleanup
	
	return 0;
}
