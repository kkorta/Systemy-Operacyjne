#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>

#define SEM_AVAILABLE_OVEN "/oven"
#define SEM_AVAILABLE_TABLE "/table"
#define SEM_TO_DELIVER "/deliveries"
#define SEM_MEMORY_LOCK "/memory"
#define SHARED_MEMORY_NAME "/sh_mem"
#define MAX_TASKS 10
#define PERMISSIONS 0660
#define MAX_OVEN 5
#define MAX_TABLE 5

enum {
    AVAILABLE_OVEN = 0,
    AVAILABLE_TABLE,
    TO_DELIVER,
    MEMORY_LOCK
};


typedef struct {
    int o_pizzas_n;
    int o_start_index;
    int t_pizzas_n;
    int t_start_index;
    int oven[MAX_OVEN];
    int table[MAX_TABLE];
} pizzas_mem;


void curr_time_ms(time_t *s, long *ms)
{
    struct timespec timespec;

    clock_gettime(CLOCK_REALTIME, &timespec);

    *s  = timespec.tv_sec;
    *ms = round(timespec.tv_nsec / 1.0e6);
    if (*ms > 999) {
        (*s)++;
        *ms = 0;
    }
}