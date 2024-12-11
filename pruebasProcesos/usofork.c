#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#define N 3
int intGlobal = 0;
int main() {
    pid_t pid;
    int intLocal = 0, i;
    int* ptrLocal = (int*)malloc(sizeof(int));
    *ptrLocal = 0;

    printf("PID INICIAL: %d\n", getpid());

    for (i=0;i<N;i++) {
        printf("PID: %d\n", pid);
        pid =fork();
        printf("PID FORK: %d\n", pid);

        if (pid==0) { // Este es el proceso hijo
        intGlobal+=2; intLocal+=2; (*ptrLocal) +=2;
        
        printf("[Hijo PID=%d, PPID=%d] Iteración %d: intGlobal=%d, intLocal=%d, *ptrLocal=%d\n", 
            getpid(), getppid(), i, intGlobal, intLocal, *ptrLocal);
        }
        else { // Este es el proceso padre
        intGlobal++; intLocal++; (*ptrLocal)++;
        printf("[Padre PID=%d] Iteración %d: intGlobal=%d, intLocal=%d, *ptrLocal=%d\n", 
            getpid(), i, intGlobal, intLocal, *ptrLocal);
        }
    }
    while (wait(NULL) != -1);

    if (errno != ECHILD) {
    printf("ERROR when waiting for childs to finish\n");
    exit(-1);
    }
    printf("Final [Padre]: intGlobal=%d, intLocal=%d, *ptrLocal=%d\n", intGlobal, intLocal, *ptrLocal);
    
    exit(0);
}
