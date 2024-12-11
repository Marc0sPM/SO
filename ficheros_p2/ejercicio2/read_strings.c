#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor
 *
 * The loadstr() function must allocate memory from the heap to store
 * the contents of the string read from the FILE.
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc())
 *
 * Returns: !=NULL if success, NULL if error
 */
char *loadstr(FILE *file)
{	
	//Se ve cuantos bytes ocupa la cadena
	int ind = 'a';
	int size = 0;
	while(ind != '\0'){
		ind =fgetc(file);
		size++;
	}
	//Ponemos el puntero en el archivo de vuelta al inicio de la cadena para leerla
	fseek(file,-size,SEEK_CUR);
	char* buff= malloc(size);
	fread(buff,sizeof(char),size,file);
	return buff;	
}

int main(int argc, char *argv[])
{
	FILE* file = NULL;
	if((file = fopen(argv[1],"r")) == NULL){
		perror("Error al abrir file");
		return 1;
	};

	while(getc(file)!=EOF){
		//Se retrocede uno porque fgetc avanza un byte directamente en vez de leer el byte actual, por lo que se debe retroceder al \0 de la cadena anterior
		fseek(file,-1,SEEK_CUR);
		char* word = loadstr(file);
		printf("%s\n",word);
		free(word);
		
	}
	/* To be completed */
	fclose(file);
	return 0;
}