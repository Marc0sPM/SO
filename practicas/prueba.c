#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int n = 0;
int main(int argc, char *argv[]){
    int status;
    int pid = fork();

    if (pid == 0) {
        printf("pid hijo: %d--------",pid);
        n++;
        printf("HIjo hhace esto n: %d----",n);
        sleep (3);
        exit(0);
    }
waitpid(pid,status,0);
printf("padre ha esperado hhace esto: %d----",n);
printf("pid padre: %d",pid);
    return 0;
}
