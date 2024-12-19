#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define CAPACITY 5
#define NUM_CUSTOMERS 1000

int quantityPizzas[2]={ 10, 10} ; //Margarita = 0, Peperoni = 1
pthread_mutex_t cerrojo;
pthread_cond_t esperarRepuesto;
pthread_cond_t camareroCocina;



void *client(void *arg)
{
    int tipo = *((int *)arg);
	pthread_mutex_lock(&cerrojo);
   
	while (quantityPizzas[tipo] == 0) {
		printf("-SEÑAL MANDADA AL CAMARERO-------\n");
		pthread_cond_signal(&camareroCocina);
        pthread_cond_wait(&esperarRepuesto, &cerrojo);
    }
	printf("[-]CLIENTE:Pizza tipo: %d , consumida por %d \n", tipo,pthread_self());
	quantityPizzas[tipo]--;
	//sleep (rand() % 4);
	pthread_mutex_unlock(&cerrojo);	
	return NULL;
}
void *camarero(){
	printf("CAMARERO CREADO!!!\n");
    while(1){
		pthread_mutex_lock(&cerrojo);
		pthread_cond_wait(&camareroCocina, &cerrojo);
		if(quantityPizzas[0]==0)
			quantityPizzas[0] = 10;
		if(quantityPizzas[1]==0)
			quantityPizzas[1] = 10;
		printf("[+]CAMARERO:Pizzas repuestas!!!\n");
		pthread_mutex_unlock(&cerrojo);
		pthread_cond_broadcast(&esperarRepuesto);
	}
	printf("CAMARERO TERMINADO!!!\n");
	return NULL;
}


int main(int argc, char *argv[])
{
	pthread_t thread[NUM_CUSTOMERS];
	pthread_t cam;
	pthread_create(&cam, NULL, camarero, NULL);
  	for (int i = 0; i < NUM_CUSTOMERS; i++) {
		int numeroAleatorio = rand() % 2;
		pthread_create(&thread[i], NULL, client, &numeroAleatorio);
		
		
  	}
	pthread_join(cam,NULL);
	for (int i = 0; i < NUM_CUSTOMERS; i++) {
    	pthread_join(thread[i], NULL);
  	}
	
	return 0;
}
