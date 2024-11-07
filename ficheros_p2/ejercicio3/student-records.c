#include <stdio.h>
#include <unistd.h> /* for getopt() */
#include <stdlib.h> /* for EXIT_SUCCESS, EXIT_FAILURE */
#include <string.h>
#include "defs.h"


/* Assume lines in the text file are no larger than 100 chars */
#define MAXLEN_LINE_FILE 100

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

	/* To be completed  (part B) */
	return 0;
}


int write_binary_file(char *input_file, char *output_file)
{
	FILE* input = NULL;
	FILE* output = NULL;
	input = fopen(input_file, "r");
	output = fopen(output_file, "w");
	
	int num;

	fscanf(input, "%d", &num);
	student_t student;

	fwrite(&num, sizeof(int), 1, output);
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
	while ((opt = getopt(argc, argv, "hi:o:p")) != -1)
	{
		switch (opt)
		{
		case 'h':
			fprintf(stderr, "Usage: %s [ -h | -i <input_file> | -o <output_file>|-p]\n", argv[0]);
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
