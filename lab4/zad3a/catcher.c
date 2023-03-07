#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "errno.h"


int sig1 = SIGUSR1;
int sig2 = SIGUSR2;

int received = 0;
pid_t sender_id;

void sig1Handler(int signo, siginfo_t *siginfo, void *context){
    if (signo == sig1 && siginfo->si_code==SI_USER){
        received++;
        printf("Signal1 received (catcher)\n");
        sender_id = siginfo->si_pid;
        return;
    }

}

void sig2Handler(int signo, siginfo_t *siginfo, void *context){
    if (signo == sig2 && siginfo->si_code==SI_USER){
        printf("Signal2 received (catcher)\n");
        sender_id = siginfo->si_pid;
        return;
    }
}

int main(int argc, char** argv){


    char* mode = argv[1];

    if (strcmp(mode, "kill") == 0 || strcmp(mode, "sigrt")){
        for (int i = 0; i < received; i++){
            kill(sender_id, sig1);
        }
        kill(sender_id, sig2);
    }
    else if(strcmp(mode, "sigqueue") == 0){
        union sigval sigval;

        for (int i = 0;i < received; i++){
            sigval.sival_int = i;
            if (sigqueue(sender_id, sig1, sigval) == 0){
                printf("Signal sent\n");

            }

        }
        sigqueue(sender_id, sig2, sigval);


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

    printf("received %d\n", received);

    return 0;




}