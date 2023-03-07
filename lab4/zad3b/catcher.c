#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include "string.h"

int sig1 = SIGUSR1;
int sig2 = SIGUSR2;

int received = 0;
pid_t sender_id;



char* mode;
int finish = 0;

void sig1Handler(int signo, siginfo_t *siginfo, void *context){
    if(sign o== sig1){
        printf("sig1(%d) (catcher)\n", received);
        received++;
        sender_id = siginfo->si_pid;
        if (!strcmp(mode, "kill") || !strcmp(mode, "sigrt")) {
            printf("Sending signal1 from catcher\n");
            kill(sender_id, sig1);
        }
        else if (!strcmp(mode, "sigqueue")) {
            union sigval sigval;
            sigval.sival_int = received;
            printf("Sending signal1 from catcher\n");
            sigqueue(sender_id, sig1, sigval);
        }
        return;
    }
}

void sig2Handler(int signo, siginfo_t *siginfo, void *context){
    if(signo==sig2){
        printf("sig2  (catcher)\n");
        finish = 1;
        return;
    }
}

int main(int argc, char** argv){
    mode = argv[1];

    if(!strcmp(mode,"sigrt")){
        sig1 = SIGRTMIN+1;
        sig2 = SIGRTMIN+2;
    }
    printf("Catcher pid: %d\n",getpid());
    struct sigaction sigaction1;
    sigaction1.sa_sigaction = sig1Handler;
    sigaction1.sa_flags = SA_SIGINFO;
    sigemptyset(&sigaction1.sa_mask);
    if(sigaction(sig1, &sigaction1, NULL)==-1){
        return -1;
    }
    struct sigaction sigaction2;
    sigaction2.sa_sigaction = sig2Handler();
    sigaction2.sa_flags = SA_SIGINFO;
    sigemptyset(&sig2.sa_mask);
    if(sigaction(sig2, &sigaction2, NULL)==-1){
        return -1;
    }


    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, sig2);
    sigsuspend(&mask);

    while(!finish){

    }

    if (!strcmp(mode, "kill") || !strcmp(mode, "sigrt")) {
        printf("Sending sig2 (catcher)\n");
        kill(sender_id, sig2);
    }
    else if (!strcmp(mode, "sigqueue")) {
        union sigval sigval;
        sigval.sival_int = received;
        printf("Sending sig2 (catcher)\n");
        sigqueue(sender_id, sig2, sigval);
    }

    printf("Received: %d\n", received);

    return 0;
}