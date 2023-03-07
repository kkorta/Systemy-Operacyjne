#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "errno.h"

int sig1 = SIGUSR1;
int sig2 = SIGUSR2;

int received = 0;


int send = 0;
int finished = 0;

void sig1Handler(int signo, siginfo_t *siginfo, void *context){
    if(signo==sig1){
        printf("sig1! (sender)\n");
        received++;
        send = 0;
        return;
    }
}

void sig2Handler(int signo, siginfo_t *siginfo, void *context){
    if(signo==sig2){
        printf("sig2!(sender)\n");
        finished = 1;
        return;
    }
}

int main(int argc, char** argv) {
    int catcher_pid = atoi(argv[1]);
    int signals_no = atoi(argv[2]);
    char *mode = argv[3];

    if (!strcmp(mode, "sigrt")) {
        sig1 = SIGRTMIN + 1;
        sig2 = SIGRTMIN + 2;
    }

    struct sigaction sigaction1;
    sigaction1.sa_sigaction = sig1Handler();
    sigaction1.sa_flags = SA_SIGINFO;
    sigemptyset(&sigaction1.sa_mask);
    if (sigaction(sig1, &sigaction1, NULL) == -1) {
        return -1;
    }
    struct sigaction sigaction2;
    sigaction2.sa_sigaction = sig2Handler();
    sigaction2.sa_flags = SA_SIGINFO;
    sigemptyset(&sigaction2.sa_mask);
    if (sigaction(sig2, &sigaction2, NULL) == -1) {
        return -1;
    }

    if (!strcmp(mode, "kill") || !strcmp(mode, "sigrt")) {
        for (int i = 0; i < signals_no; i++) {
            kill(catcher_pid, sig1);
            send = 1;
            while (send == 1) {

            }
        }
        kill(catcher_pid, sig2);
    } else if (!strcmp(mode, "sigqueue")) {
        union sigval sigval;
        for (int i = 0; i < signals_no; i++) {
            sigval.sival_int = i;
            if (sigqueue(catcher_pid, sig1, sigval) == 0) {
                printf("Sending sig1 sender\n");
                send = 1;
                while (send == 1) {

                }
            }
            printf("Sending sig2 sender\n");
            sigqueue(catcher_pid, sig2, sigval);
        }

        while (!finished) {

        }

        printf("Caught signal2 in sender\n");


        printf("Sent: %d\n Got : %d\n", signals_no, received);

        return 0;
    }
}