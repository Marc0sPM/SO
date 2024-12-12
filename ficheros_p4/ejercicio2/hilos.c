#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define MAX_THREADS 100 // numero maximo de hilos 
typedef struct {
	int t_num; 
	char priority;
} ThreadData;

// funcion que van a realizar los hilos 
void *thread_usuario(void *arg)
{
	ThreadData* data = (ThreadData* ) arg;
	if(data->priority == 'P') {
		printf("Thread #%ld with id: %ld is PRIORITY\n", data->t_num, pthread_self());
	}else if(data->priority == 'N') {
		printf("Thread #%ld with id: %ld is NOT PRIORITY\n", data->t_num, pthread_self());
	} else {
		printf("Error en la prioridad del hilo\n"); 
	}

	free(data);
	return NULL;
}

int main(int argc, char* argv[])
{
	if(argc != 2 ) {
		fprintf(stderr, "Usage: %s <number of threads>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int num_threads = atoi(argv[1]);

	if (num_threads <= 0 || num_threads > MAX_THREADS) {
        fprintf(stderr, "El número de hilos debe estar entre 1 y %d\n", MAX_THREADS);
        return EXIT_FAILURE;
    }


	pthread_t*  threads = malloc(num_threads* sizeof(pthread_t));
	if(threads == NULL) {
		perror("Error en creacion de lista de hilos");
		exit(EXIT_FAILURE);
	}
	for(int i = 0; i < num_threads; i++){
		ThreadData *  data = malloc(sizeof(ThreadData));
		if(data == NULL) {
			perror("data malloc failed!");
			exit(EXIT_FAILURE);
		}

		//asignamos la data del hilo
		data->t_num = i;
		data->priority = (i % 2 == 0) ? 'P' : 'N';

		if(pthread_create(&threads[i], NULL, thread_usuario, data ) != 0) {
			perror("Error al crear el hilo");
			//En caso de fallar al crear el hilo liberamos la data ya que no se va a usar
			free(data);
			exit(EXIT_FAILURE);
		}

	}

	// Ejecutamos todos los hilos de la lista 
	for(int i = 0; i < num_threads; i++){
		pthread_join(threads[i], NULL);
	}
	return 0;
}
