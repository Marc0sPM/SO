#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 2 // Capacidad máxima de la discoteca
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip") // Macro para convertir booleano a cadena de texto

// Estructura para almacenar la información de los clientes
typedef struct {
    int isVip; // Indica si el cliente es VIP (1 para VIP, 0 para normal)
    int id;    // Identificador único del cliente
} client_info;

// Variables globales para sincronización
pthread_mutex_t mtx; // Mutex para proteger las secciones críticas
pthread_cond_t vip_cond, normal_cond; // Variables de condición para clientes VIP y normales

// Variables para el estado de la discoteca
int aforo = 0; // Número actual de clientes dentro de la discoteca
int waiting_vip = 0; // Número de clientes VIP esperando para entrar
int waiting_normal = 0; // Número de clientes normales esperando para entrar

// Función para manejar la entrada de un cliente normal
void enter_normal_client(int id) {
    pthread_mutex_lock(&mtx); // Bloquea el mutex para la sección crítica

    // Espera mientras la discoteca está llena o hay clientes VIP esperando
    while (aforo >= CAPACITY || waiting_vip > 0) {
        waiting_normal++; // Incrementa la cantidad de normales en espera
        pthread_cond_wait(&normal_cond, &mtx); // Espera a ser notificado
        waiting_normal--; // Decrementa la cantidad de normales en espera tras ser notificado
    }

    aforo++; // Incrementa el aforo al entrar
    printf("NORMAL client #%2d entered. Current occupacy: %d\n", id, aforo);

    pthread_mutex_unlock(&mtx); // Libera el mutex
}

// Función para manejar la entrada de un cliente VIP
void enter_vip_client(int id) {
    pthread_mutex_lock(&mtx); // Bloquea el mutex para la sección crítica

    // Espera mientras la discoteca está llena
    while (aforo >= CAPACITY) {
        waiting_vip++; // Incrementa la cantidad de VIPs en espera
        pthread_cond_wait(&vip_cond, &mtx); // Espera a ser notificado
        waiting_vip--; // Decrementa la cantidad de VIPs en espera tras ser notificado
    }

    aforo++; // Incrementa el aforo al entrar
    printf("VIP client #%2d entered. Current occupacy: %d\n", id, aforo);

    pthread_mutex_unlock(&mtx); // Libera el mutex
}

// Función para simular el baile de un cliente dentro de la discoteca
void dance(int id, int isvip) {
    printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip)); // Mensaje indicando que el cliente está bailando
    sleep((rand() % 3) + 1); // Tiempo aleatorio de baile (entre 1 y 3 segundos)
}

// Función para manejar la salida de un cliente de la discoteca
void disco_exit(int id, int isvip) {
    pthread_mutex_lock(&mtx); // Bloquea el mutex para la sección crítica

    aforo--; // Decrementa el aforo al salir
    printf("Client #%2d (%s) exited. Current occupacy: %d\n", id, VIPSTR(isvip), aforo);

    // Notifica a los clientes que están esperando según la prioridad
    if (waiting_vip > 0) {
        pthread_cond_signal(&vip_cond); // Prioriza a los clientes VIP
    } else if (waiting_normal > 0) {
        pthread_cond_signal(&normal_cond); // Si no hay VIPs, notifica a un cliente normal
    }

    pthread_mutex_unlock(&mtx); // Libera el mutex
}

// Función ejecutada por cada hilo (cliente)
void *client(void *arg) {
    client_info* info = (client_info*) arg; // Recupera la información del cliente

    // Decide si el cliente entra como VIP o normal
    if (info->isVip) {
        enter_vip_client(info->id); // Entrada para cliente VIP
    } else {
        enter_normal_client(info->id); // Entrada para cliente normal
    }

    dance(info->id, info->isVip); // Cliente baila dentro de la discoteca

    disco_exit(info->id, info->isVip); // Cliente sale de la discoteca

    return NULL; // Termina el hilo
}

// Carga los datos de los clientes desde un archivo
void load_clients_from_file(const char* filename, client_info** clients, int *num_clients) {
    FILE* file = fopen(filename, "r"); // Abre el archivo en modo lectura
    if (!file) {
        perror("Error opening file"); // Imprime error si no se pudo abrir el archivo
        exit(EXIT_FAILURE); // Sale del programa
    }

    // Lee el número de clientes del archivo
    fscanf(file, "%d", num_clients);
    *clients = malloc(sizeof(client_info) * (*num_clients)); // Reserva memoria para los clientes

    // Lee la información de cada cliente (VIP o normal)
    for (int i = 0; i < *num_clients; i++) {
        fscanf(file, "%d", &(*clients)[i].isVip); // Lee si el cliente es VIP
        (*clients)[i].id = i + 1; // Asigna un ID único al cliente
    }
    fclose(file); // Cierra el archivo
}

int main(int argc, char *argv[]) {
    if (argc != 2) { // Verifica que se pase el nombre del archivo como argumento
        fprintf(stderr, "Usage: %s <file_name>", argv[0]); // Muestra el formato correcto de uso
        return EXIT_FAILURE; // Termina con error
    }

    client_info* clients = NULL; // Puntero para almacenar la información de los clientes
    int num_clients = 0; // Número total de clientes
    load_clients_from_file(argv[1], &clients, &num_clients); // Carga los clientes desde el archivo

    pthread_t threads[num_clients]; // Array de identificadores de hilos

    // Inicialización de mutex y variables de condición
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&vip_cond, NULL);
    pthread_cond_init(&normal_cond, NULL);

    // Creación de hilos para cada cliente
    for (int i = 0; i < num_clients; i++) {
        pthread_create(&threads[i], NULL, client, &clients[i]);
    }

    // Espera a que todos los hilos terminen
    for (int i = 0; i < num_clients; i++) {
        pthread_join(threads[i], NULL);
    }

    // Limpieza de memoria y recursos
    free(clients); // Libera la memoria de los clientes
    pthread_mutex_destroy(&mtx); // Destruye el mutex
    pthread_cond_destroy(&vip_cond); // Destruye la condición VIP
    pthread_cond_destroy(&normal_cond); // Destruye la condición normal

    return 0; // Termina el programa exitosamente
}

