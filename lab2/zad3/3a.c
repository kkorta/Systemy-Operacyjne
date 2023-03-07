#include <time.h>
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "dirent.h"
#include "sys/stat.h"


int directories = 0, files = 0, char_dev = 0, block_dev = 0, fifos = 0, slinks = 0, socks = 0;


char* determineType(char file){
    if (DT_DIR == file){
        directories++;
        return "dir";
    }
    if (DT_REG = file){
        files++;
        return "file";
    }
    if (DT_CHR == file){
        char_dev++;
        return "char_dev";
    }
    if (DT_BLK == file){
        block_dev++;
        return "block_dev";
    }
    if (DT_FIFO == file){
        fifos++;
        return "fifo";
    }
    if (DT_LNK = file){
        slinks++;
        return "slink";
    }
    if (DT_SOCK == file){
        socks++;
        return "sock";
    }
    return "zero";

}
char* timer(__time_t* time){
    char* arr = calloc(25, 1);
    strftime(arr, 25, "%Y-%m-%d %H:%M:%S", localtime(time));
    return arr;
}
void print(char  *path, struct dirent* dir){
    char* result = realpath(fpath, NULL);
    struct stat stat1;
    stat(dir -> d_name, &stat1);
    printf("Sciezka bezwzgledna: %s\n", result);
    printf("Liczba dowiazan: %hd\n", stat1.st_nlink);

    printf("Rodzaj pliku: %s\n", determineType(dir->d_type));
    printf("Rozmiar w bajtach: %ld\n", stat1.st_size);
    printf("Data ostatniego dostepu: %s\n", timer((__time_t*)&stat1.st_atime));
    printf("Data ostatniej modyfikacji: %s\n", timer(__time_t*)&stat1.st_mtime);
}

void recursionFile(char *path){
    DIR* dir = opendir(path);
    struct dirent* dirent;
    if (!dir)
        return;

    char path1[500];


    while ((dirent = readdir(dir)) != NULL){

        if ((strcmp(dirent -> d_name, "..") != 0) && (strcmp(dirent -> d_name, ".") != 0))
        {
            strcpy(path1, path);
            strcat(path1, "/");
            strcat(path1, dirent -> d_name);

            print(path1, dirent);
            recursionFile(path1);
        }
    }
    closedir(dir);
}

int main(int argc, char** argv){
    if (argc == 2){
        recursionFile(argv[1]);
        printf("zwykłe pliki %d,",files);
        printf("katalogi %d,",directories );
        printf("pliki specjalne znakowe %d,", char_dev );
        printf("pliki specjalne blokowe %d,", block_dev);
        printf("potoki FIFO %d,",fifos );
        printf("linki symboliczne %d", slinks);
        printf("sokety %d\n", socks);

    }
    else if (argc < 2){
        printf("More arguments needed");
    }
    else{
        printf("Less argument needed");
    }
}

