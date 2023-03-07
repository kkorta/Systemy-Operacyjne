#include "common.h"

sem_t *semaphores[4];
pizzas_mem *smem;
int running = 1;

void open_resources(){
    semaphores[AVAILABLE_OVEN] = sem_open(SEM_AVAILABLE_OVEN, O_RDWR);
    semaphores[AVAILABLE_TABLE] = sem_open(SEM_AVAILABLE_TABLE, O_RDWR);
    semaphores[TO_DELIVER] = sem_open(SEM_TO_DELIVER, O_RDWR);
    semaphores[MEMORY_LOCK] = sem_open(SEM_MEMORY_LOCK, O_RDWR);
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
    open_resources();
    signal(SIGINT, close_semaphores);

    long ms;
    time_t s;
    int pizza_type;
    int p_index;
    while(running == 1){
        int uloc = 0;

        sem_wait(semaphores[TO_DELIVER]);
        sem_wait(semaphores[MEMORY_LOCK]);
        sem_post(semaphores[AVAILABLE_TABLE]);
        for (int i = smem->t_start_index; i < smem->t_start_index + smem->t_pizzas_n; ++i) {
            p_index = i % MAX_TABLE;
            if (smem->table[p_index] > 9 || smem->table[p_index] < 0) {
                printf("ERROR");
                continue;
            }

            pizza_type = smem->table[p_index];
            smem->table[p_index] = 10;
            smem->t_pizzas_n--;
            smem->t_start_index++;
            uloc = 1;
            curr_time_ms(&s, &ms);
            printf("(%d %d.%03ld) Taking pizza: %d. Number of pizzas on the table: %d\n",
                   getpid(), (int) s, ms, pizza_type,
                   smem->t_pizzas_n);
            sem_post(semaphores[MEMORY_LOCK]);

            sleep( 4);

            curr_time_ms(&s, &ms);
            printf("(%d %d.%03ld) Giving pizza: %d\n",
                   getpid(), (int) s, ms, pizza_type);

            sleep(4);
            printf("I'm back\n");
            break;
        }
        if (uloc == 0) {
            sem_post(semaphores[MEMORY_LOCK]);
        }
    }

    for (int i = 0; i < 4; ++i) {
        sem_close(semaphores[i]);
    }

    munmap(smem, sizeof (pizza_type));

    return 0;
}