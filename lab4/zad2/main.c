#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "signal.h"
#include "wait.h"


void sig1Handler(int signo, siginfo_t *siginfo, void *context){
    if (signo == SIGUSR1 && siginfo->si_code == SI_USER){
        printf("sig1 try killing (real user id %d)\n", siginfo->si_uid);
    }
}

void sigChilHandler(int signo, siginfo_t *siginfo, void *context){
    if (signo == SIGCHLD){
        printf("Sigchild %d\n", siginfo->so_status);
    }
}

void sigfpeHandler(int signo, siginfo_t *siginfo, void *context){
    if (signo = SIGFPE && siginfo->si_code == FPE_INTDIV){
        printf("Sigfpe instruction %p\n", siginfo->si_addr);
    }
}

void case1(void){
    struct sigaction sigaction1;
    sigaction1.sa_sigaction = sig1Handler;
    sigaction1.sa_flags = SA_SIGINFO;
    sigemptyset(&sigaction1.sa_mask);

    if (sigaction(SIGUSR1, &sigaction1, NULL) == -1){
        printf("cant reach sig1\n");
        return;
    }
    kill(getpid(), SIGUSR1);
}

int case2(void){
    struct sigaction sigaction1;
    sigaction1.sa_sigaction = sig1Handler;
    sigaction1.sa_flags = SA_SIGINFO;
    sigemptyset(&sigaction1.sa_mask);

    if (sigaction(SIGCHLD, &sigaction1, NULL) == -1){
        printf("cant reach SIGCHLD\n");
        return 0;
    }

    pid_t pid = fork();
    if (pid < 0){
        printf("fork error\n");
        return 0;
    }

    wait(NULL);
    return 0;

}

void case3(void){
    struct sigaction sigaction1;
    sigaction1.sa_sigaction = sig1Handler;
    sigaction1.sa_flags = SA_SIGINFO;
    sigemptyset(&sigaction1.sa_mask);

    if (sigaction(SIGFPE, &sigaction1, NULL) == -1){
        printf("cant reach sigfpe\n");
        return;
    }

    int x = 0;
    printf("%d", 9 / x);

}

int main(int argc, char **argv){
    case1();
    case2();
    case3();
    return 0;
}