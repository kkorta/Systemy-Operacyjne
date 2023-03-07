#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "sys/times.h"
#include "unistd.h"
#include "time.h"

#include "signal.h"

int determineMode(char* arg){
    if(strcmp(arg,"ignore") == 0){
        return 0;
    }
    if(strcmp(arg,"handler") == 0){
        return 1;
    }
    if(strcmp(arg,"mask") == 0){
        return 2;
    }
    if(strcmp(arg,"pending") == 0){
        return 3;
    }
    return -1;
}

int main(int argc, char** argv){
    int mode = determineMode(argv[1]);
    switch(mode){
        case 0:
            raise(SIGUSR1);
            printf("ignored\n");
            break;
        case 2:
            raise(SIGUSR1);
            printf("masked \n");
            break;
        case 3:
            ;sigset_t curr_sigs;
            sigpending(&curr_sigs);
            printf("pending (parent %s)\n", sigismember(&curr_sigs, SIGUSR1)?"yes":"no");
            break;
        default:
            return 1;
    }
    return 0;
}
