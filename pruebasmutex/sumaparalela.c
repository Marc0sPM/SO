#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define M 4

struct param {
unsigned long long ni,nf;
};
pthread_mutex_t mtx;

unsigned long long total = 0;
void* suma(void* arg)   {
    struct param *p = (struct param*) arg;
    unsigned long long i;
    for (i = p->ni; i <= p->nf; i++) {
        // se bloquea el aceso al codigo que accede a la variable global total para que en la ejecucion, 
        // varios hilos no accedan simultaneamente y asi calcular bien los valores
        pthread_mutex_lock(&mtx);
        total += i;
        pthread_mutex_unlock(&mtx);

    }
    return NULL;
}

void reparto(struct param [M], unsigned long long N);

int main(int argc, char* argv[]){
    int i;
    unsigned long long N;
    char *endptr;

    pthread_t hilos[M];
    struct param P[M] = {0};

    if ((argc != 2) || !(N = strtoll(argv[1], &endptr, 10)) ||
    (*endptr != '\0')){
        printf("Error, uso:\n\t%s unsigned_long_long\n", argv[0]);
        return(-1);
    }

    printf("Sumando de 1 a %llu (Gauss: %llu)... \n",N, (N * (N+1))/2);

    reparto(P, N);

    for (i=0; i<M; i++) pthread_create(&hilos[i], NULL, suma, &P[i]);
    for (i=0; i<M; i++) pthread_join(hilos[i], NULL);

    printf("Resultado = %llu\n", total);

    return 0;
}

void reparto(struct param P[M], unsigned long long int N)
{
    unsigned long long i,j,n,resto;
    n = N / M;
    resto = N % M;
    // numero del caracter por el que va el for del numero 
    j = 0;
    for (i=0; (i < M) && (j < N); i++){
        P[i].ni = j+1;
        P[i].nf = j+n;
        if (resto){
            P[i].nf++;
            resto--;
        }
        j = P[i].nf;
    }
}

