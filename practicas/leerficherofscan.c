#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define BUFFER_SIZE 512

typedef struct node {
  int name;
  int numPredecessors;
  int *predecessors;
} Node;

int main(int argc, char* arg2[]){
    if(argc > 2 ){
        printf("[!] Arguments exceded, please only enter 1 args!!!\n");
        return 1;
    }
    int fd = open(arg2[1], O_CREAT | O_RDWR, 0777);
    FILE *file = fdopen(fd, "r");
    
    int numEntries;
    
    fscanf(file, "%d", &numEntries);
      if (file == NULL) {
        perror("Error creating FILE stream\n");
        close(fd);
        return 1;
    }
    
    Node *entries = (Node *)malloc(numEntries * sizeof(Node));

    for (int i = 0; i < numEntries; ++i) {
        fscanf(file, "%d,%d", &entries[i].name, &entries[i].numPredecessors);

        entries[i].predecessors = (int *)malloc(entries[i].numPredecessors * sizeof(int));

        for (int j = 0; j < entries[i].numPredecessors; ++j) {
            fscanf(file, ",%d", &entries[i].predecessors[j]);
        }
    }
    printf("Entrada1: %d, %d\n", entries[2].name, entries[2].numPredecessors);
    
     // Free allocated memory
    for (int i = 0; i < numEntries; ++i) {
        free(entries[i].predecessors);
    }
    
    free(entries);
    fclose(file);
    close(fd);
    return 0;
}