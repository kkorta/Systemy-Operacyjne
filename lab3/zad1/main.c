#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
    if (argc != 2){
        fprintf(stderr,   "wrong number of arguments\n" );
        exit(1);
    }

    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++){
        int pid = fork();
        if (pid == 0){
            printf("I (process %d) come from process number %d", i, (int)getpid());
        }

    }
    return 0;
}