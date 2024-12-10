#include <stdio.h>
#include <unistd.h>

int main() {
    char *args[] = { "./otro", NULL }; // Argumentos para ejecutar 'ls -l'
    execvp("./otro", args); // Buscar y ejecutar 'ls' en el PATH
    perror("execvp failed"); // Si falla, imprime un error
    return 1;
}
