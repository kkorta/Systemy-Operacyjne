#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

    char symbol;
    if (argc > 3)
    {
        printf("Too many arguments\n");
        return -1;
    }
    else if (argc < 3){
        printf("More arguments needed");
        return -1;
    }
    else{
        symbol = *(argv[1]);
        FILE* file = fopen(argv[2], "r");
        if (ferror(file)){
            fprintf(stderr, "Error with file opening");
            return -1;
        }
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        rewind(file);

        char* buff = calloc(1 + size, sizeof(char));
        fread(buff, 1, size, file);

        int symbol_counter = 0;
        int lines_counter = 0;
        int exist_line = 0;
        for (int i = 0; i < size; i++){
            if (buff[i] == symbol){symbol_counter++; exist_line = 1;}
            if(buff[i] == '\n'){
                if (exist_line){ lines_counter++;}
                exist_line = 0;
            }
        }
        printf("Number of char \"%c\" is %d\n", symbol, symbol_counter);
        printf("Number of lines where char exists is %d", symbol_counter);
        free(buff);
    }
    return 0;
}

