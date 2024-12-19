#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

#define N 10

int *contador;
sem_t *mutex;

void productor() {
    int i;
    for (i = 0; i < N; i++) {
        sem_wait(mutex);
        (*contador)++;
        printf("Productor: Incrementa contador a %d\n", *contador);
        sem_post(mutex);
    }
}

void consumidor() {
    int i;
    for (i = 0; i < N; i++) {
        sem_wait(mutex);
        (*contador)--;
        printf("Consumidor: Decrementa contador a %d\n", *contador);
        sem_post(mutex);
    }
}

int main() {
    // Crear memoria compartida para el contador
    contador = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *contador = 0;

    // Crear semáforo para el mutex
    mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(mutex, 1, 1); // Inicializar el semáforo como mutex

    // Crear un proceso hijo
    pid_t pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Proceso hijo (productor)
        productor();
    } else {
        // Proceso padre (consumidor)
        consumidor();

        // Esperar a que el proceso hijo termine
        wait(NULL);

        // Destruir el semáforo
        sem_destroy(mutex);

        // Liberar la memoria compartida
        munmap(contador, sizeof(int));
        munmap(mutex, sizeof(sem_t));
    }

    return 0;
}
