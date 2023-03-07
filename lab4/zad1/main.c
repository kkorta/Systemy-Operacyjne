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

void handler(int sign){
    printf("handler received\n");
}

int main(int argc, char** argv){
    int mode = determineMode(argv[2]);
    if(argc!=3){
        printf("3 arguments needed\n");
        return 1;
    }
    switch(mode){
        case 0:
            signal(SIGUSR1, SIG_IGN);
            raise(SIGUSR1);
            printf("ignored \n");
            break;
        case 1:
            signal(SIGUSR1, handler);
            raise(SIGUSR1);
            break;
        case 2:
        case 3:
            ;sigset_t mask;
            sigemptyset(&mask);
            sigaddset(&mask, SIGUSR1);
            sigprocmask(SIG_BLOCK, &mask, NULL);
            raise(SIGUSR1);
            if(mode==3){
                sigset_t currSig;
                sigpending(&currSig);
                printf("pending\n", sigismember(&currSig, SIGUSR1)?"yes":"no");
            }
            break;
    }
    if(strcmp(argv[1],"exec") == 0){
        execl("./testy", "./testy", argv[2], NULL);
    }
    else if(strcmp(argv[1],"fork") == 0){
        if(fork()==0){
            switch(mode){
                case 0:
                    raise(SIGUSR1);
                    printf("ignored\n");
                    break;
                case 1:
                    raise(SIGUSR1);
                    break;
                case 2:
                    raise(SIGUSR1);
                    printf("masked \n");
                    break;
                case 3:
                    ;sigset_t currsigs;
                    sigpending(&currsigs);
                    printf("pending (child %s)\n", sigismember(&currsigs, SIGUSR1)?"yes":"no");
                    break;
            }
        }
    }
    else{
        return 1;
    }
    return 0;
}