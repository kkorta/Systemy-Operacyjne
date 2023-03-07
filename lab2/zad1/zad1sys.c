#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>
#include <fcntl.h>


int main(int argc, char** argv)
{

    char *filename1 = NULL;
    char *filename2 = NULL;
    if (argc > 3)
    {
        printf("Too many arguments\n");
        return -1;
    }
    else if (argc == 3)
    {
        filename1 = argv[1];
        filename2 = argv[2];
    }
    else{
        filename1 = calloc(100, 1);
        filename2 = calloc(100, 1);
        scanf("%s", filename1);
        scanf("%s", filename2);
    }

    int editFile = open(filename2, O_RDONLY);
    int file = open(filename1, O_WRONLY);


    long size = lseek(file, 0, SEEK_END);
    lseek(file, 0, SEEK_SET);


    char* buff = calloc(size + 1, sizeof(char));
    read(file, buff, sizeof(char)*size);

    bool check = true;
    int start = 0;

    for (int i = 0; i < size; ++i) {

        if (!isspace(buff[i])){
            check = false;
        }
        else if( buff[i] == '\n'){
            if (!check){
                write(editFile, buff + start, sizeof(char)*(i + 1 - start));

            }
            check = true;
            start = 1 + i;
        }

    }
    close(file);
    close(editFile);
    free(buff);
    return 0;
}