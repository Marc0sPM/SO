#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#define M 10 // Cantidad de raciones que el cocinero pone en el caldero
#define SHM_NAME "/CALDERO" // Nombre del segmento de memoria compartida
#define SEM_EMPTY "/EMPTY" // Nombre del semáforo para indicar si el caldero está vacío
#define SEM_FULL "/FULL" // Nombre del semáforo para indicar si el caldero está lleno
#define SEM_MUTEX "/MUTEX" // (No usado aquí) Mutex para proteger acceso concurrente

// Declaración de semáforos y memoria compartida
sem_t *full; // Semáforo para indicar que el caldero está lleno
sem_t *empty; // Semáforo para indicar que el caldero está vacío
int *pot = NULL; // Puntero al caldero en memoria compartida
int shd; // Descriptor de la memoria compartida

int finish = 0; // Bandera para terminar el proceso

// Función para colocar raciones en el caldero
void putServingsInPot(int servings)
{
    *pot = servings; // Se coloca la cantidad de raciones en el caldero
    sem_post(full); // Se libera el semáforo "full" indicando que el caldero está lleno
    printf("[+]Cook (%d): Putting servings in the pot\n", getpid());
}

// Función principal del cocinero
void cook(void)
{
    while (!finish) {
        // Espera hasta que el caldero esté vacío
        sem_wait(empty);
        putServingsInPot(M); // Llena el caldero con M raciones
        sleep(2); // Simula el tiempo que tarda en llenar el caldero
    }
}

// Función para liberar recursos y limpiar
void cleanup() {
    munmap(pot, sizeof(int)); // Desasigna la memoria compartida
    sem_close(full); // Cierra el semáforo "full"
    sem_close(empty); // Cierra el semáforo "empty"
    sem_unlink(SEM_EMPTY); // Elimina el semáforo "empty"
    sem_unlink(SEM_FULL); // Elimina el semáforo "full"
    shm_unlink(SHM_NAME); // Elimina el segmento de memoria compartida
}

// Manejador de señales (SIGINT y SIGTERM)
void handler(int signo)
{
    if (signo == SIGTERM || signo == SIGINT) {
        finish = 1; // Establece la bandera de terminación
        cleanup(); // Limpia los recursos
        printf("Cook (%d): Received signal, terminating\n", getpid());
        exit(EXIT_SUCCESS); // Termina el proceso exitosamente
    }
}

int main(int argc, char *argv[])
{
    // Configura manejadores para señales SIGTERM y SIGINT
    signal(SIGTERM, handler);
    signal(SIGINT, handler);

    // Establece y configura el segmento de memoria compartida
    shd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666); // Crea o abre la memoria compartida
    ftruncate(shd, sizeof(int)); // Establece el tamaño de la memoria compartida
    pot = (int*) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shd, 0); // Mapea la memoria compartida

    *pot = 0; // Inicializa el caldero como vacío (0 raciones)

    // Inicializa los semáforos
    empty = sem_open(SEM_EMPTY, O_CREAT, 0666, 0); // Inicialmente 0, ya que el caldero empieza vacío
    full = sem_open(SEM_FULL, O_CREAT, 0666, 0); // Inicialmente 0, ya que no hay raciones en el caldero

    printf("Cook %d: Started\n", getpid());
    cook(); // Inicia la función principal del cocinero

    // Limpieza de recursos (nunca se alcanza debido al bucle infinito en `cook()`)
    return 0;
}

