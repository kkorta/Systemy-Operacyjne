#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define SEM_PROJECT_ID 1
#define MEM_PROJECT_ID 2
#define PERMISSIONS 0666
#define MAX_TASKS 10
#define MAX_OVEN 5
#define MAX_TABLE 5


enum {
    AVAILABLE_OVEN = 0,
    AVAILABLE_TABLE,
    TO_DELIVER,
    MEMORY_LOCK
};

struct sembuf put_pizza_to_oven = {AVAILABLE_OVEN, -1, 0};
struct sembuf get_pizza_from_oven = {AVAILABLE_OVEN, 1m 0};

struct sembuf put_pizza_on_table = {AVAILABLE_TABLE, -1, 0};
struct sembuf get_pizza_from_table = {AVAILABLE_TABLE, 1, 0};

struct sembuf deliver_pizza = {TO_DELIVER, -1, 0};
struct sembuf new_pizza_to_deliver = {TO_DELIVER, 1, 0};

struct sembuf mem_lock = {MEMORY_LOCK, -1, 0};
struct sembuf mem_unlock = {MEMORY_LOCK, 1, IPC_NOWAIT};

typedef struct {
    int o_pizza_n;
    int o_start_index;
    int t_pizzas_n;
    int t_start_index;
    int oven[MAX_OVEN];
    int table[MAX_TABLE];
} pizzas_mem;

void curr_time_ms(time_t *s, long *ms){
    struct timespec timespec;
    clock_gettime(CLOCK_REALTIME, &timespec);
    *s = timespec.tv_sec;
    *ms = round(timespec.tv_nsec / 1.0e6);
    if (*ms > 999){
        (*s)++;
        *ms = 0;
    }
}