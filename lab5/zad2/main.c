#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv){
    if (argc != 2 && argc != 4){
        printf("Wrong number of arguments");
        return -1;
    }
    else if (argc == 2){
        char com[15] = "sort -k ";
        if (strcmp(argv[1], "data") == 0){
            strcat(com, "3");
        } else if (strcmp(argv[1], "nadawca") == 0){
            strcat(com, "2");
        }
        FILE *sort = popen(com, "w");
        FILE *mail = popen("mailq", "r");
        char buf[1024];

        while (fgets(buf, sizeof (buf), mail) != NULL){
            fputs(buf, sort);
        }
        pclose(mail);
        pclose(sort);
    }
    else {
        char *email = argv[1];
        char *title = argv[2];
        char *text = argv[3];

        char com[1024] = "mail -s ";
        strcat(strcat(strcat(com, "'"), title), "' ");
        strcat(strcat(strcat(com, "'"), email), "' ");

        FILE *mail = popen(com, "w");
        fputc('"', mail);
        fputs(text, mail);
        fputc('"', mail);
        pclose(mail);
    }
    return 0;
}