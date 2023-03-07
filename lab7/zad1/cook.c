#include "common.h"

int main(int argc, char **argv){
    srand(time(NULL) + getpid());
    key_t sem_key = ftok(getenv("HOME"), SEM_PROJECT_ID);
    int sem_id = semget(sem_key, 0, PERMISSIONS);
    sem_key = ftok(getenv("HOME"), MEM_PROJECT_ID);
    int sh_mem_id = shmget(sem_key, 0 ,PERMISSIONS);

    long ms;
    time_t s;
    int pizza_type;
    int p_index;
    while (1){
        pizzas_mem *smem = shmat(sh_mem_id, NULL, 0);
        pizza_type = rand() % 10;
        curr_time_ms(&s, &ms);
        printf("(%d %d.%03ld) Preparing pizza: %d\n", getpid(), (int) s, ms, pizza_type);
        sleep(1);
        semop(sem_id, (struct sembuf[2]) {mem_lock, put_pizza_to_oven}, 2);
        p_index = (smem->o_start_index + smem->o_pizza_n) % MAX_OVEN;
        smem->oven[p_index] = pizza_type;
        smem->o_pizza_n++;
        curr_time_ms(&s, &ms);
        printf("(%d %d.%03ld) added pizza: %d. number of pizzas in oven: %d\n",
               getpid(), (int) s, ms, pizza_type,
               smem->o_pizza_n);
        semop(sem_id, (struct sembuf[1]) {mem_unlock}, 1);
        sleep(4);
        semop(sem_id,
              (struct sembuf[4])
                      {get_pizza_from_oven, put_pizza_on_table, new_pizza_to_deliver, mem_lock},
              4);
        smem->oven[p_index] = 10;
        smem->o_start_index++;
        smem->o_pizza_n--;

        smem->table[(smem->t_start_index + smem->t_pizzas_n) % MAX_OVEN] = pizza_type;
        smem->t_pizzas_n++;
        curr_time_ms(&s, &ms);
        printf("(%d %d.%03ld) Taking pizza from oven: %d. Number of pizzas in oven: %d. Number of pizzas on the table: %d\n",
               getpid(), (int) s, ms, pizza_type,
               smem->o_pizza_n,
               smem->t_pizzas_n);

        semop(sem_id, (struct sembuf[1]) {mem_unlock}, 1);
        shmdt(smem);
    }
}
