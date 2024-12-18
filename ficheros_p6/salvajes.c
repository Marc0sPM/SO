#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define NUMITER 4
#define SHM_NAME "/CALDERO"
#define SEM_EMPTY "/EMPTY"
#define SEM_FULL "/FULL"
#define SEM_MUTEX "/MUTEX"

int *pot = NULL;
sem_t *full;
sem_t *empty;

int getServingsFromPot(void)
{
	if(*pot <= 0) {
			sem_post(empty);
			sem_wait(full);
	}
	// esperameos a que se actualice la memoria compartida pot
	sleep(1);
	(*pot)--;
    printf("Savage took a serving. Remaining: %d\n", *pot);

}
void eat(void)
{
	unsigned long id = (unsigned long) getpid();
	printf("Savage %lu eating\n", id);
	sleep(rand() % 5);

void savages(void)
{
	for(int i = 0; i < NUMITER; ++i) {
		getServingsFromPot();
		eat();
	}
}

int main(int argc, char *argv[])
{
	full = sem_open(SEM_FULL, 0);
	empty = sem_open(SEM_EMPTY, 0);

	int shd = shm_open("/CALDERO", O_RDWR, 0644);
    ftruncate(shd, sizeof(int));  // Set the size of the shared memory
    pot = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shd, 0);
	savages();

	munmap(pot, sizeof(int));
	return 0;
}
