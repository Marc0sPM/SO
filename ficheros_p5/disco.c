	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <pthread.h>

	#define CAPACITY 2
	#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

	typedef struct {
		int isVip;
		int id;
	} client_info;

	pthread_mutex_t mtx;
	pthread_cond_t vip_cond, normal_cond;

	int aforo = 0;
	int waiting_vip = 0;
	int waiting_normal = 0;

	void enter_normal_client(int id)
	{

		pthread_mutex_lock(&mtx);

		/*	Seccion de introduccion de clientes a la discoteca es la seccion critica	*/

		while(aforo >= CAPACITY || waiting_vip > 0) {
			waiting_normal++;
			pthread_cond_wait(&normal_cond, &mtx);
			waiting_normal--;
		}

		aforo++;


		printf("NORMAL client #%2d entered. Current occupacy: %d\n", id, aforo);

		pthread_mutex_unlock(&mtx);

	}

	void enter_vip_client(int id)
	{
		pthread_mutex_lock(&mtx);


	/*	Seccion de introduccion de clientes a la discoteca es la seccion critica	*/


		while(aforo >= CAPACITY) {
			waiting_vip++;
			// Esperando a la señal vip_cond
			pthread_cond_wait(&vip_cond, &mtx);
			waiting_vip--;
		}

		aforo ++;
		printf("VIP client #%2d entered. Current occupacy: %d\n", id, aforo);

		pthread_mutex_unlock(&mtx);
	}

	void dance(int id, int isvip)
	{
		printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
		sleep((rand() % 3) + 1);
	}

	void disco_exit(int id, int isvip)
	{
		/*	Acceso al aforo de la discotca es seccion critica -> mutex 	*/

		pthread_mutex_lock(&mtx);

		aforo--;
		printf("Client #%2d (%s) exited. Current occupacy: %d\n", id, VIPSTR(isvip), aforo);

		// Si hay vips esperando se manda la señal vip_cond
		if(waiting_vip > 0) {
			pthread_cond_signal(&vip_cond);
		} else if(waiting_normal > 0) {
			// Si no hay vips pero si normales 
			pthread_cond_signal(&normal_cond);
		} 

		pthread_mutex_unlock(&mtx);

	}

	/*Funcion que ejecutan los hilos */
	void *client(void *arg)
	{
		client_info* info = (client_info* )arg;

		if(info->isVip) {
			enter_vip_client(info->id);
		} else {
			enter_normal_client(info->id);
		}

		dance(info->id, info->isVip);

		disco_exit(info->id, info->isVip);

		return NULL;

	}

	/*Carga de clientes a partir de un fichero*/
	void load_clients_from_file(const char* filename, client_info** clients, int *num_clients) {
		FILE* file = fopen(filename, "r");
		if(!file) {
			perror("Error opening file");
			exit(EXIT_FAILURE);
		}

		fscanf(file, "%d", num_clients);
		*clients = malloc(sizeof(client_info) * (*num_clients));

		for(int i = 0; i < *num_clients; i++){
			
			fscanf(file, "%d", &(*clients)[i].isVip);
			(*clients)[i].id = i + 1;
		}
		fclose(file);
	}

	int main(int argc, char *argv[])
	{
		if(argc != 2) {
			fprintf(stderr, "Usage: %s <file_name>", argv[0]);
			return EXIT_FAILURE;
		}


		client_info* clients = NULL;
		int num_clients = 0;
		load_clients_from_file(argv[1], &clients, &num_clients);

		pthread_t threads [num_clients];

		//Inicializacion de mutex y condiciones 
		pthread_mutex_init(&mtx, NULL);
		pthread_cond_init(&vip_cond, NULL);
		pthread_cond_init(&normal_cond, NULL);


		//Creacion de los hilos
		for(int i = 0; i < num_clients; i++){
			pthread_create(&threads[i], NULL, client,  &clients[i] );
		}

		for( int i = 0; i < num_clients; i++){
			pthread_join(threads[i], NULL);
		}
		// Limpieza de memoria, mutex y condiciones 
		free(clients);
		pthread_mutex_destroy(&mtx);
		pthread_cond_destroy(&vip_cond);
		pthread_cond_destroy(&normal_cond);

		return 0;
	}
