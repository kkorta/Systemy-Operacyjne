#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "unistd.h"

int size;
int max_len;
int cmd_len_max;

void commandsHandlers(char* cmd[], int len){
    int pipes[len][2];
    for(int i=0;i<len;i++){
        if(pipe(pipes[i])<0){
            printf("error with pipe!\n");
            exit(11);
        }
    }
    printf("======\n");
    for(int i=0;i<len;i++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("cant fork\n");
            exit(12);
        }
        if (pid == 0) {
            if (i != 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i != len - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            for (int j = 0; j < len - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            char *args[cmd_len_max];
            args[0] = strtok(cmd[i], " ");
            for (int r = 1; r < cmd_len_max; r++) {
                args[r] = strtok(NULL, " ");
            }

            execvp(args[0], args);
            exit(EXIT_SUCCESS);
        }
    }
}

void parse(int all,int num, int orders[num-all][10], char*** cmd){
    for(int i=num-all-1;i>=0;i--) {
        int j = 0;
        int ctr=0;
        char** parsed = calloc(all*cmd_len_max,1);
        while(orders[i][j]>-1){
            int k = 0;
            while(k<cmd_len_max && strcmp(cmd[orders[i][j]][k]," ")){
                parsed[ctr] = calloc(max_len,1);
                strcpy(parsed[ctr],cmd[orders[i][j]][k]);
                ctr++;
                k++;
            }
            j++;
        }

        commandsHandlers(parsed, ctr);
        free(parsed);
    }
}

int main(int argc, char** argv){
    if(argc > 2){
        printf("Too many args!\n");
        return -1;
    }
    char* path = argv[1];
    FILE* f= fopen(path, "r");
    if(f == NULL){
        printf("error!\n");
        return -1;
    }
    char* buf = calloc(1,2048);

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(buf, size, 1, f);
    fclose(f);

    printf("\n%s\n---\n",buf);

    int num = 0;
    char *c = buf;

    int curr = 0, all = 0, cmd_len = 1;
    max_len = 0;
    cmd_len_max = 0;
    while(*c != '\0'){
        if(*c == '|'){
            cmd_len += 1;
        }
        if(*c == '\n'){
            if(cmd_len_max < cmd_len)
                cmd_len_max = cmd_len;
            cmd_len = 1;
            num += 1;
            if(curr > max_len){
                max_len = curr;
            }
            if(curr == 0)
                all = num-1;
            curr = 0;
        }
        else
            curr += 1;
        c++;
    }

    char eq = '=';
    char stp = '|';
    char end = '\n';

    char *** commands = calloc(num+1,1);
    int orders[num - all][10];
    for(int i = 0;i < num - all;i++)
        for(int j = 0; j < 10; j++)
            orders[i][j] = -1;
    for(int i = 0; i < num; i++) {
        commands[i] = calloc(cmd_len_max + 1, 1);
        for (int j = 0; j < cmd_len_max; j++) {
            commands[i][j] = calloc(max_len + 1, 1);
            commands[i][j] = " ";
        }
    }
    char* cmd=strtok(buf,&stp);
    int row = -1;
    int col;
    while(cmd!=NULL){
        if(cmd[0] != ' '){
            row += 1;
            col = 0;
            if(row >= all){
                if(cmd[0] == ' '){
                    cmd += 9;
                }
                else{
                    cmd += 8;
                }
                orders[row - all][col] = atoi(cmd) - 1;
                col += 1;
            }
        }
        else{
            if(row >= all){
                if(cmd[0] == ' '){
                    cmd += 9;
                }
                else{
                    cmd += 8;
                }
                orders[row-all][col] = atoi(cmd)-1;
            }
            else {
                commands[row][col] = cmd;
                printf("|%d|%d|:%s\n", row, col, commands[row][col]);
            }
            col+=1;
        }
        cmd = strtok(NULL,&stp);
    }
    parse(all,num, orders, commands);
    free(commands);
    free(buf);
    return 0;
}