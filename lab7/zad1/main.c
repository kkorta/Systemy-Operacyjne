//
// Created by Kacper on 19.05.2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>
#include <errno.h>

#include "common.h"

int sem_id;
int shared_mem_id;
int workers_n;
pid_t *children;

void create_semaphore(){
    key_t sem_key = ftok(getenv("HOME"), SEM_PROJECT_ID);
    sem_id = semget(sem_key, 4, IPC_CREAT | PERMISSIONS);

    semctl(sem_id, AVAILABLE_OVEN, SETVAL, MAX_OVEN);
    semctl(sem_id, AVAILABLE_TABLE, SETVAL, MAX_TABLE);
    semctl(sem_id, TO_DELIVER, SETVAL, 0);
    semctl(sem_id, MEMORY_LOCK, SETVAL, 1);
}

void create_shared_memory(){
    key_t sm_key = ftok(getenv("HOME"), MEM_PROJECT_ID);
    shared_mem_id = shmget(sm_key, sizeof (pizzas_mem), PERMISSIONS | IPC_CREAT);
    pizzas_mem *mem = shmat(shared_mem_id, NULL, 0);
    memset(mem, 0, sizeof (pizzas_mem));
    shmdt(mem);
}

void cleanup(){
    if(shared_mem_id != -1){
        shmctl(shared_mem_id, IPC_RMID, 0);
    }

    if(sem_id != -1){
        semctl(sem_id, 0, IPC_RMID);
    }

    free(children);
    shared_mem_id = -1;
    sem_id = -1;
    children = NULL;
}

void kill_children(){
    for(int i = 0; i < workers_n; i++){
        kill(children[i], SIGINT);
    }
    cleanup();
    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        puts("Wrong number of arguments");
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, kill_children);
    signal(SIGTERM, kill_children);

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    workers_n = N + M;

    create_semaphore();
    create_shared_memory();
    children = calloc(workers_n, sizeof(pid_t));
    int children_indexer = 0;
    pid_t pid;

    for (int i = 0; i < N; ++i) {
        if ((pid = fork()) == 0){
            execl("./cook", "./cook", NULL);
            exit(0);
        }
        children[children_indexer++] = pid;
    }
    for (int i = 0; i < M; ++i) {
        if ((pid = fork()) == 0){
            execl("./deliver", "./deliver", NULL);
            exit(0);
        }
        children[children_indexer++] = pid;
    }

    for (int i = 0; i < workers_n; ++i) {
        wait(0);
    }

    cleanup();
    return 0;
}