#include <stdio.h>
#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>



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

    FILE* editFile = fopen(filename2, "w+");
    FILE* file = fopen(filename1, "r+");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0 , SEEK_SET);
    rewind(file);

    char* buff = calloc(size + 1, sizeof(char));
    fread(buff, sizeof(char), size, file);

    bool check = true;
    int start = 0;

    for (int i = 0; i < size; ++i) {

        if (!isspace(buff[i])){
            check = false;
        }
        else if( buff[i] == '\n'){
            if (!check){
                fwrite(start + buff, sizeof(char), i + 1 - start, editFile);

            }
            check = true;
            start = 1 + i;
        }

    }
    fclose(file);
    fclose(editFile);
    free(buff);
    return 0;
}