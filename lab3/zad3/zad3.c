#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "stdlib.h"
#include "sys/wait.h"
#include "string.h"
#include "dirent.h"
#include "sys/stat.h"


int checkExistence(char* path, char* search){
    FILE* file = fopen(path, "r+");
    if (ferror(file)){
        fprintf(stderr, "Error with file opening");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    rewind(file);

    char buffer[file_size];
    fread(buffer, 1, file_size, file);

    int result = 0;
    if (strstr(buffer, search)){
        result = 1;
    }
    free(buffer);
    fclose(file);
    return  result;
}


void traverse(char* path, int steps, int index, char* search){
    struct dirent *dir;
    char path2[500];
    DIR* dirStart = opendir(path);
    int i = 0;
    pid_t child;
    if(!dirStart){
        return;
    }

    while ((dir == readdir(dirStart)) != NULL){
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
            strcpy(path2, path);
            strcat(path2, "/");
            strcat(path2, dir->d_name);

            int sg = dir->d_type;
            if (DT_DIR == sg && steps > 1){
                child = fork();
                if (child != 0){
                    i++;
                } else{
                    traverse(path2, steps - 1, 1, search);
                }

            } else{
                int len = strlen(dir->d_name);
                if(dir->d_name[len - 1] == 't' && dir->d_name[len - 2] == 'x' && dir->d_name[len - 3] == 't' && dir->d_name =='.'){
                    if (checkExistence(path2, search)){
                        printf("proccess %d, path %s\n", getpid(), path2);
                    }
                }
            }


        }
    }
    int j = 0;
    for (j; j < i; j++){
        wait(NULL);
    }

    if (index != -1)
        exit(0);
}

int main(int argc, char** argv){
    if (argc <= 3){
        return -1;
    }
    traverse(argv[1], atoi(argv[3]), -1, argv[2]);
    return 0;
}