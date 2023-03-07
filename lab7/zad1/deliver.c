//
// Created by Kacper on 19.05.2022.
//

#include "common.h"

int main(int argc, char ** argv){
    srand(time(NULL) + getpid());
    key_t sem_key = ftok(getenv("HOME"), SEM_PROJECT_ID);
    int sem_id = semget(sem_key, 0, PERMISSIONS);

    sem_key = ftok(getenv("HOME"), MEM_PROJECT_ID);
    int sh_mem_id = shmget(sem_key, 0, PERMISSIONS);

    long ms;
    time_t s;
    int pizza_type;
    int p_index;
    while(1 == 1){
        int uloc = 0;
        pizzas_mem *smem = shmat(sh_mem_id, NULL, 0);

        semop(sem_id, (struct sembuf[3]) {deliver_pizza, get_pizza_from_table ,mem_lock},
              3);
        for (int i = smem->t_start_index; i < smem->t_start_index + smem->t_pizzas_n; ++i) {
            p_index = i % MAX_TABLE;
            if (smem->table[p_index] > 9 || smem->table[p_index] < 0) {
                printf("Error!!\n");
                continue;
            }

            pizza_type = smem->table[p_index];
            smem->table[p_index] = 10;
            smem->t_pizzas_n--;
            smem->t_start_index++;
            uloc = 1;
            curr_time_ms(&s, &ms);
            printf("(%d %d.%03ld) Receiving pizza: %d. Number of pizzas on the table: %d\n",
                   getpid(), (int) s, ms, pizza_type,
                   smem->t_pizzas_n);
            semop(sem_id, (struct sembuf[1]) {mem_unlock}, 1);

            sleep( 4);

            curr_time_ms(&s, &ms);
            printf("(%d %d.%03ld) Giving pizza: %d\n",
                   getpid(), (int) s, ms, pizza_type);

            sleep(4);
            printf("I'm back\n");
            break;
        }
        if (uloc == 0) {
            semop(sem_id, (struct sembuf[1]) {mem_unlock}, 1);
        }
        shmdt(smem);
    }
}