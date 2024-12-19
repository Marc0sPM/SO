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
#include <fcntl.h>

int nbytes = 0;
char *file = "-1";

void traverseDir(DIR * dir){

    struct dirent * de;
    
    while ((de = readdir(dir))){
        if(strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0 ){
            printf("-------%d, %s-------\n\n", de->d_type,de->d_name); //Debug, ver cual fichero se esta accediendo
            if(de->d_type == 8){ //fichero -- leer nbytes

                int fd = open (de->d_name, O_RDONLY, 0777);

                char buffer[nbytes];
                int i = 0;
                lseek(fd,0,SEEK_SET);
                
                read(fd,buffer,nbytes);
                strcat(buffer, "\n");

                if(strcmp(file, "-1")==0){ //escribir en terminal
                    printf("%s\n",buffer);
                }else{ //escribir en output.txt
                    int fd2 = open(file, O_CREAT | O_RDWR, 0777 );
                     lseek(fd2,0,SEEK_END);
                    write(fd2, buffer, nbytes);
                   
                }
            }else if(de->d_type == 4){ //directorio
                DIR *dirRecursive;
                if ((dirRecursive = opendir(de->d_name)) == NULL) {
                    printf("Error opening directory\n");
                    return 1;
                }
                traverseDir(dirRecursive);
            }
           
           
        }
    }
}


int main(int argc, char *argv[])
{
    int o;
    

	if(argc < 3){
        printf("Incorrect arguments!!!\n");
        return 1;
    }
    
    
    while((o = getopt(argc, argv, "n:o:")) != -1){
        switch (o)
        {
        case 'n':
            nbytes = atoi(optarg);
            break;
        case 'o':
            file = optarg;
        break;
        default:
            break;
        }
    }
    DIR *dir;
    if ((dir = opendir(".")) == NULL) {
        printf("Error opening directory\n");
        return 1;
    }
    
    traverseDir(dir);


    return 0;
}
