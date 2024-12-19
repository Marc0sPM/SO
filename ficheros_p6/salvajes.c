#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define NUMITER 4 // Número de iteraciones (cuántas veces un salvaje comerá)
#define SHM_NAME "/CALDERO" // Nombre del segmento de memoria compartida
#define SEM_EMPTY "/EMPTY" // Nombre del semáforo que indica que el caldero está vacío
#define SEM_FULL "/FULL" // Nombre del semáforo que indica que el caldero está lleno
#define SEM_MUTEX "/MUTEX" // No usado aquí, pero podría ser usado para exclusión mutua

// Variables globales
int *pot = NULL; // Puntero al caldero en la memoria compartida
sem_t *full; // Semáforo para sincronizar si el caldero está lleno
sem_t *empty; // Semáforo para sincronizar si el caldero está vacío

// Función para tomar una ración del caldero
int getServingsFromPot(void)
{
    // Si el caldero está vacío
    if (*pot <= 0) {
        sem_post(empty); // Notifica al cocinero que el caldero está vacío
        sem_wait(full); // Espera a que el cocinero lo llene
    }
    // Simula un retraso para asegurarse de que la memoria compartida se actualiza correctamente
    sleep(1);
    (*pot)--; // Toma una ración del caldero
    printf("Savage took a serving. Remaining: %d\n", *pot); // Muestra cuántas raciones quedan
}

// Función para simular que un salvaje come
void eat(void)
{
    unsigned long id = (unsigned long)getpid(); // Obtiene el ID del proceso
    printf("Savage %lu eating\n", id); // Imprime que el salvaje está comiendo
    sleep(rand() % 5); // Simula el tiempo que tarda en comer
}

// Función principal de los salvajes
void savages(void)
{
    for (int i = 0; i < NUMITER; ++i) { // Itera NUMITER veces
        getServingsFromPot(); // Toma una ración del caldero
        eat(); // Come la ración
    }
}

int main(int argc, char *argv[])
{
    // Abre el semáforo "full" (caldero lleno)
    full = sem_open(SEM_FULL, 0);
    // Abre el semáforo "empty" (caldero vacío)
    empty = sem_open(SEM_EMPTY, 0);

    // Abre la memoria compartida para acceder al caldero
    int shd = shm_open("/CALDERO", O_RDWR, 0644);
    ftruncate(shd, sizeof(int)); // Establece el tamaño del segmento de memoria compartida
    pot = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shd, 0); // Mapea la memoria compartida

    savages(); // Llama a la función principal de los salvajes

    // Libera la memoria compartida
    munmap(pot, sizeof(int));

    return 0; // Finaliza el programa
}

