#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int *n ;


int main(void){
	n = (int *)malloc(sizeof(int));
	*n = 5;
	int pid=fork();
	if (pid==0){
		(*n)++;
		printf("Hijo: %d\n", *n);
		exit(0);
	}
	int status;
	wait(&status);
	printf("Padre: %d\n", *n);

	return 0;
}