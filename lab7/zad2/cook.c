#include "common.h"

sem_t *semaphores[4];
pizzas_mem *smem;
int running = 1;

void open_resources(){
    semaphores[AVAILABLE_OVEN] = sem_open(SEM_AVAILABLE_OVEN, O_RDWR);
    semaphores[AVAILABLE_TABLE] = sem_open(SEM_AVAILABLE_TABLE, O_RDWR);
    semaphores[TO_DELIVER] = sem_open(SEM_TO_DELIVER, O_RDWR);
    semaphores[MEMORY_LOCK] = sem_open(SEM_MEMORY_LOCK, O_RDWR);

    for (int i = 0; i < 4; ++i) {
        if (semaphores[i] == SEM_FAILED) {
            perror("sem_open");
        }
    }
    int sh_mem_id = shm_open(SHARED_MEMORY_NAME, O_RDWR, PERMISSIONS);
    smem = mmap(
            NULL,
            sizeof (pizzas_mem),
            PROT_READ|PROT_WRITE,
            MAP_SHARED,
            sh_mem_id,
            0);
}

void close_semaphores(){
    running = 0;
}

int main(int argc, char ** argv){
    srand(time(NULL) + getpid());
    signal(SIGINT, close_semaphores);
    open_resources();

    long ms;
    time_t s;
    int pizza_type;
    int p_index;
    while(running == 1){
        pizza_type = rand() % 10;
        curr_time_ms(&s, &ms);
        printf("(%d %d.%03ld) Preparing pizza: %d\n",
               getpid(), (int) s, ms, pizza_type);
        sleep(1);

        sem_wait(semaphores[AVAILABLE_OVEN]);
        sem_wait(semaphores[MEMORY_LOCK]);
        p_index = (smem->o_start_index + smem->o_pizzas_n) % MAX_OVEN;

        smem->oven[p_index] = pizza_type;
        smem->o_pizzas_n++;
        curr_time_ms(&s, &ms);
        printf("(%d %d.%03ld) Added pizza: %d. Number of pizzas in oven: %d\n",
               getpid(), (int) s, ms, pizza_type,
               smem->o_pizzas_n);
        sem_post(semaphores[MEMORY_LOCK]);

        sleep(4);

        sem_wait(semaphores[AVAILABLE_TABLE]);
        sem_wait(semaphores[MEMORY_LOCK]);
        sem_post(semaphores[AVAILABLE_OVEN]);
        sem_post(semaphores[TO_DELIVER]);

        smem->oven[p_index] = 10;
        smem->o_start_index++;
        smem->o_pizzas_n--;
        smem->table[(smem->t_start_index + smem->t_pizzas_n) % MAX_OVEN] = pizza_type;
        smem->t_pizzas_n++;
        curr_time_ms(&s, &ms);
        printf("(%d %d.%03ld) Taking pizza: %d. Number of pizzas in oven: %d. Number of pizzas on Table: %d\n",
               getpid(), (int) s, ms, pizza_type,
               smem->o_pizzas_n,
               smem->t_pizzas_n);

        sem_post(semaphores[MEMORY_LOCK]);
    }

    for (int i = 0; i < 4; ++i) {
        sem_close(semaphores[i]);
    }

    munmap(smem, sizeof (pizza_type));

    return 0;
}