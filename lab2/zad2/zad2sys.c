#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


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
        int file = open(argv[2], O_RDONLY);


        long size = lseek(file, 0, SEEK_END);
        lseek(file, 0, SEEK_SET);


        char* buff = calloc(1 + size, sizeof(char));
        read(file, buff, sizeof(char)*size);

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
