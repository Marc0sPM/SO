#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"
#include <bits/getopt_core.h>


/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100

char* loadstr (FILE *file) {
	//se usa int para identifica EOF u otros carecteres que no caben en un byte
	int ind = 'a';
	int size = 0;
	while(ind != '\0') {
		ind = fgetc(file);
		size++;
	} 

	fseek(file, -size, SEEK_CUR);
	char* buff = malloc(size);
	fread(buff, sizeof(char), size, file);
	return buff;
}

int print_text_file(char *path)
{
	//Lectura 
	FILE* file = NULL;
	file = fopen(path, "r");
	int num;

	fscanf(file, "%d", &num);

	student_t student;

	char* aux = malloc(MAXLEN_LINE_FILE);
	for(int i = 0; i < num; i++) {
		fscanf(file, "%i:%s", &student.student_id, aux);
		//Necesario hacer copia al ser un array estatico (NIF)
		strcpy(student.NIF, strsep(&aux, ":"));
		//No es necesario hacer copia al ser un array dinamico
		student.first_name = strsep(&aux, ":");
		student.last_name = aux;
		printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n", i, student.student_id, student.NIF, student.first_name, student.last_name);
	}	
	fclose(file);
	return 0;
}

int print_binary_file(char *path)
{
	FILE* file = fopen(path, "rb");
	int num;

	fread(&num, sizeof(int), 1, file);

	student_t student;

	for(int i = 0; i < num; i++){
		fread(&student.student_id, sizeof(int), 1, file);
		fread(&student.NIF, sizeof(char), MAX_CHARS_NIF + 1, file);
		student.first_name = loadstr(file);
		student.last_name = loadstr(file);
		printf("[Entry #%d]\n\tstudent_id=%d\n\tNIF=%s\n\tfirst_name=%s\n\tlast_name=%s\n", i, student.student_id, student.NIF, student.first_name, student.last_name);
	}
	return 0;
}


int write_binary_file(char *input_file, char *output_file)
{
	//abrimos entrada y salida
	FILE* i_file = fopen(input_file,"r");
	FILE* o_file = fopen(output_file,"w");

	int num;

	fscanf(i_file,"%d",&num);
	student_t studen;

	//escribimos cantidad de registros
	fwrite(&num,sizeof(int),1,o_file);

	char* aux=malloc(MAXLEN_LINE_FILE);

	for(int i=0;i<num;i++){
		//leemos regsitros de entrada
		fscanf(i_file,"%i:%s",&studen.student_id,aux);
		strcpy(studen.NIF,strsep(&aux,":"));
		studen.first_name = strsep(&aux,":");
		studen.last_name = aux;

		//escribimos registros en salida (binaria)
		fwrite(&studen.student_id,sizeof(int),1,o_file);
		fwrite(&studen.NIF,sizeof(char),MAX_CHARS_NIF + 1,o_file);
		fwrite(studen.first_name,sizeof(char),strlen(studen.first_name)+1 ,o_file);
		fwrite(studen.last_name,sizeof(char),strlen(studen.last_name)+1 ,o_file);

	}

	return 0;
}

int main(int argc, char *argv[])
{
	int ret_code, opt;
	struct options options;

	/* Initialize default values for options */
	options.input_file = NULL;
	options.output_file = NULL;
	options.action = NONE_ACT;
	ret_code = 0;

	/* Parse command-line options (incomplete code!) */
	while ((opt = getopt(argc, argv, "hi:o:pb")) != -1)
	{
		switch (opt)
		{
		case 'h':
			fprintf(stderr, "Usage: %s [ -h | -p | -i file | -o <output_file> | -b ]\n", argv[0]);
			exit(EXIT_SUCCESS);
		case 'i':
			options.input_file = optarg;
			break;
		case 'p':
			options.action = PRINT_TEXT_ACT;
			break;
		case 'o':
			options.output_file = optarg;
			options.action = WRITE_BINARY_ACT;
			break;
		case 'b':
			options.action = PRINT_BINARY_ACT;
			break;
		default:
			exit(EXIT_FAILURE);
		}
	}

	if (options.input_file == NULL)
	{
		fprintf(stderr, "Must specify one record file as an argument of -i\n");
		exit(EXIT_FAILURE);
	}

	switch (options.action)
	{
	case NONE_ACT:
		fprintf(stderr, "Must indicate one of the following options: -p, -o, -b \n");
		ret_code = EXIT_FAILURE;
		break;
	case PRINT_TEXT_ACT:
		/* Part A */
		ret_code = print_text_file(options.input_file);
		break;
	case WRITE_BINARY_ACT:
		/* Part B */
		ret_code = write_binary_file(options.input_file, options.output_file);
		break;
	case PRINT_BINARY_ACT:
		/* Part C */
		ret_code = print_binary_file(options.input_file);
		break;
	default:
		break;
	}
	exit(ret_code);
}
