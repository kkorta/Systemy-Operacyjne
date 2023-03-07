#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "errno.h"


int sig1 = SIGUSR1;
int sig2 = SIGUSR2;

int received = 0;

void sig1Handler(int signo, siginfo_t *siginfo, void *context){
    if (signo == sig1 && siginfo->si_code==SI_USER){
        received++;
        printf("Signal1 received (parent)\n");
        return;
    }

}

void sig2Handler(int signo, siginfo_t *siginfo, void *context){
    if (signo == sig2 && siginfo->si_code==SI_USER){
        printf("Signal2 received (parent)\n");
        return;
    }
}

int main(int argc, char** argv){
    if (argc != 4){
        printf("4 arguments needed\n");
        return 1;
    }

    char* mode = argv[3];
    int catcher_pid = atoi(argv[1]);
    int signals_no = atoi(argv[2]);

    if (strcmp(mode, "kill") == 0 || strcmp(mode, "sigrt")){
        for (int i = 0; i < signals_no; i++){
            kill(catcher_pid, sig1);
        }
        kill(catcher_pid, sig2);
    }
    else if(strcmp(mode, "sigqueue") == 0){
        union sigval sigval;

        for (int i = 0;i < signals_no; i++){
            union sigval sigval;
            sigval.sival_int = i;
            if (sigqueue(catcher_pid, sig1, sigval) == 0){
                printf("Signal sent\n");

            }

        }
        sigqueue(catcher_pid, sig2, sigval);


    }


    if (strcmp(mode, "sigrt") == 0){
        sig1 = SIGRTMIN + 1;
        sig2 = SIGRTMIN + 2;
    }

    struct sigaction signal1;
    signal1.sa_sigaction = sig1Handler;
    signal1.sa_flags = SA_SIGINFO;
    sigemptyset(&signal1.sa_mask);
    if (sigaction(sig1, &signal1, NULL) == -1){
        return 1;
    }

    struct sigaction signal2;
    signal2.sa_sigaction = sig2Handler;
    signal2.sa_flags = SA_SIGINFO;
    sigemptyset(&signal2.sa_mask);
    if (sigaction(sig2, &signal2, NULL) == -1){
        return 1;
    }

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, sig2);
    sigsuspend(&mask);


    printf("Caught signal2 in sender\n");


    printf("Sent: %d\n Got : %d\n",
           signals_no, received);

    return 0;




}