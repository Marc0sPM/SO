#include <stdio.h>
#include <pthread.h>

#define NTH 10  // Define el número de hilos a crear

// Definición de la estructura que se pasará a cada hilo
struct tharg {
    int i;        // Un índice para identificar el hilo
    char *msg;    // Mensaje que el hilo imprimirá
};

// Función que se ejecutará en cada hilo
void* foo(void* arg) {
    // Se castea arg de tipo (void* )  a  (struct tharg* )  como hacer un int i --> (float) i
        struct tharg *tharg = (struct tharg *) arg;

    // El hilo imprime su índice y el mensaje       thread_id --> pthread_self()
    printf("Thread %d with pthread id %ld says %s\n", 
           tharg->i, (long)pthread_self(), tharg->msg);

    // Termina el hilo
    pthread_exit(0);
}

int main(void) {
    int j;
    pthread_attr_t attr;    // Variable para los atributos de los hilos
    struct tharg args[NTH];  // Array de estructuras para los argumentos de los hilos
    pthread_t thid[NTH];     // Array para almacenar los identificadores de los hilos

    // Inicializa los atributos del hilo (en este caso, los atributos por defecto)
    pthread_attr_init(&attr);

    // Crear NTH hilos
    for (j = 0; j < NTH; j++) {
        args[j].i = j;           // Asigna el índice del hilo
        args[j].msg = "Hello!";  // Asigna el mensaje que el hilo imprimirá

        // Crea un hilo y le pasa la estructura con los datos
        pthread_create(&thid[j], &attr, foo, &args[j]);
    }

    // Espera a que todos los hilos terminen
    for (j = 0; j < NTH; j++) {
        pthread_join(thid[j], NULL);  // Bloquea la ejecución hasta que el hilo termineps 
    }

    return 0;  // Finaliza el programa
}
