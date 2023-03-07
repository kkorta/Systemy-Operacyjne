#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/wait.h>

clock_t timer_begin, timer_end;
struct tms timer_start_tms, timer_end_tms;

void start_timer(){
    timer_begin = times(&timer_start_tms);
}

void stop_timer(){
    timer_end = times(&timer_end_tms);
}

double calc_time(clock_t s, clock_t e){
    return ((double ) (e - s) / (double ) sysconf(_SC_CLK_TCK))
}

double function(long double x){
    return 4/(x * x + 1);
}

void calculate_area(int n, double width){
    int new_n = 1/width;
    int num_of_iter = n/new_n;
    int index = 0;
    int i = 1;
    int p_id = 0;
    double result;
    char tab[25];

    while (i <= n){
        p_id = fork();
        if(p_id == 0){
            sprintf(tab, "w%d.txt", i);
            FILE *file = fopen(tab, "w");

            for (int j = index; j < new_n && j < num_of_iter + index; ++j){
                result = function((1/2) * width + j * width) * width;
                fprintf(file, "%f", result);

            }
            fclose(file);
            exit(0);
        }
        index += num_of_iter;
        ++i;
    }
}

int main(int argc, char** argv){
    if (argc < 3){
        fprintf(stderr, "More arguments needed");
    }
    else if (argc > 3){
        fprintf(stderr, "Less arguments needed");
    }
    else{
        start_timer();
        double width = atof(argv[1]);
        int n = atoi(argv[2]);

        calculate_area(n, width);

        char tab[25];
        int i = 1;
        double tmp;
        double result = 0;
        FILE *file;
        for (i; i < n; ++i){
            sprintf(tab, "w%d.txt", i);
            file = fopen(tab, "r");
            while (fscanf(file, "&lf\n", &tmp) != EOF){
                result += tmp;
            }
            fclose(file);
        }
        stop_timer();
        printf("result: %f\n width: %f\n", result, width);
    }
    return 0;
}