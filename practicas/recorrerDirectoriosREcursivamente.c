#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>

struct options {
	char *progname;
	int recurse;
};

struct options opt;

void usage(void)
{
	printf("%s [options] [dirname]\n\n", opt.progname);
	printf("lists the contents of dirname (default .)\n");
	printf("options:\n"
		"\t-h:\tshow this help\n"
		"\t-R:\trecursive\n"
	);
}

/* section b */
void list_dir(char *name)
{
	DIR *d;
	struct dirent *de;

	

	if (!(d = opendir(name))) {
		return 1;
	}

	while ((de = readdir(d))) {	
			printf("%s\n", de->d_name);
	}

	closedir(d);
}

/* section c */
void process_recurse(char *dirname, char *name)
{
	pid_t pid;
	char path[PATH_MAX];
	DIR *d;


	
	

	snprintf(path, sizeof(path), "%s/%s", dirname, name);
	if(strcmp(name, ".") != 0 && strcmp(name, "..") != 0 ){
		if ((pid = fork()) == 0) {
			
			if (opt.recurse) {
								
				if (d = opendir(path)) {
					printf("%s:\n", path);
					closedir(d);
				}
				list_dir(path);
				list_dir_recurse(path);
			}
			exit(0);
		} else if (pid < 0) {
			err(1, "fork");
		}
	}

	wait(NULL);
}

/* section c */
void list_dir_recurse(char *name)
{
	DIR *d;
	struct dirent *de;

	if (!(d = opendir(name))) {
		return 1;
	}

	while ((de = readdir(d))) {
			
			
			process_recurse(name, de->d_name);
	}

	closedir(d);


}

int main(int argc, char *argv[])
{
	char *dirname = ".";
	int o;
	opt.progname = argv[0];
	opt.recurse = 0;

	/* Section a: process options with getopt */
	while((o = getopt(argc, argv, "hR")) != -1){
		switch (o)
		{
		case 'h':
			usage();
			return 0;
			break;
		case 'R':
			opt.recurse =1;
			break;
		default:
			usage();
			break;
		}
	}
	//printf("%d OPIND, %d ARGC \n",optind, argc );
	if (optind < argc) {
		dirname = argv[optind];
	}




	/********************************************/

	if (opt.recurse)
		printf("%s:\n", dirname);

	list_dir(dirname);

	if (opt.recurse)
		list_dir_recurse(dirname);

	return 0;
}
