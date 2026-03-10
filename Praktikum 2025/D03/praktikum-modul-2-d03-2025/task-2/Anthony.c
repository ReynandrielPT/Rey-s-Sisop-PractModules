#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>

typedef struct report{
    int before, after;
    char country[100];
}rep;

void download(){
    pid_t pid = fork();
    if(pid == 0){
        char *args[] = {"wget", "-O", "netflixData.zip", "https://drive.google.com/uc?export=download&id=12GWsZbSH858h2HExP3x4DfWZB1jLdV-J", NULL};
        execv("/usr/bin/wget", args);
    } else {
        int status;
        wait(&status);
        if(WIFEXITED(status)){
            pid_t unzip_pid = fork();

            if(unzip_pid == 0){
                char *argv[] = {"unzip", "-q", "netflixData.zip", NULL};
                execv("/usr/bin/unzip", argv);
            }else{
                wait(&status);
                if(WIFEXITED(status)) remove("netflixData.zip");
            }
        }
    }
}

void awk(char *line, char *collum[]){
    int flag = 0, col = 0, index = 0;
    char temp[1000], *ptr = line;

    while(*ptr != '\0'){
        if(*ptr == '"'){
            flag = !flag;
            temp[index++] = *ptr;
        } else if(*ptr == ',' && flag == 0){
            temp[index] = '\0';
            collum[col++] = strdup(temp);
            index = 0;
        } else {
            temp[index] = *ptr;
            index++;
        }
        ptr++;
    }

    temp[index] = '\0';
    collum[col++] = strdup(temp);
}

void log(char *category, char *title){
    FILE *log = fopen("log.txt", "a");
    char hour[9];

    time_t seconds;
    struct tm *times;
    time(&seconds);
    times = localtime(&seconds);

    strftime(hour, sizeof(hour), "%H:%M:%S", times);
    fprintf(log, "[%s] Proses mengelompokkan berdasarkan %s: sedang mengelompokkan untuk film %s\n", hour, category, title);
    fclose(log);
}

void category(char *file, char *type){
    int status;
    pid_t pid = fork();
    if(pid == 0){
        char *argv[] = {"mkdir", "-p", type, NULL};
        execvp("mkdir", argv);
    } else wait(&status);

    FILE *data = fopen(file, "r");
    char dataLine[1000];
    fgets(dataLine, 1000, data);

    while(fgets(dataLine, 1000, data)){
        dataLine[strcspn(dataLine, "\n")] = '\0';

        char *collum[4];
        awk(dataLine, collum);

        char *title = collum[0], *director = collum[1], *year = collum[3], fileName[50];

        if(strcmp(type, "judul") == 0){
            log("Abjad", title);

            char mainTitle = title[0];
            if(isalnum(mainTitle))
                snprintf(fileName, sizeof(fileName), "%s/%c.txt", type, mainTitle);
            else
                snprintf(fileName, sizeof(fileName), "%s/#.txt", type);
        } else if(strcmp(type, "tahun") == 0){
            log("Tahun", title);
            snprintf(fileName, sizeof(fileName), "%s/%s.txt", type, year);
        }

        FILE *txt = fopen(fileName, "a");
        if(txt){
            fprintf(txt, "%s - %s - %s\n", title, year, director);
            fclose(txt);
        }
    }
    fclose(data);
}

rep stat[100];
int countryCount = 0;

void *country(void *arg){
    char *data = (char*)arg;
    FILE *file = fopen(data, "r");

    char line[1000];
    fgets(line, 1000, file);
    while(fgets(line, 1000, file)){
        line[strcspn(line, "\n")] = '\0';

        char *collum[4];
        awk(line, collum);

        char *countri = collum[2];
        int year = atoi(collum[3]);

        if(strlen(countri) == 0) continue;

        int found = 0;
        for(int i = 0; i < countryCount; i++){
            if(strcmp(stat[i].country, countri) == 0){
                if(year < 2000) stat[i].before++;
                else stat[i].after++;
                found = 1;
                break;
            }
        }

        if(!found){
            strcpy(stat[countryCount].country, countri);
            if(year < 2000){
                stat[countryCount].before = 1;
                stat[countryCount].after = 0;
            } else {
                stat[countryCount].before = 0;
                stat[countryCount].after = 1;
            }
            countryCount++;
        }
    }
    fclose(file);
    return NULL;
}

void report(char *file_csv){
    pthread_t thread;
    pthread_create(&thread, NULL, country, file_csv);
    pthread_join(thread, NULL);

    char fileName[100], date[10];

    time_t seconds;
    struct tm *timeDetail;
    time(&seconds);
    timeDetail = localtime(&seconds);

    strftime(date, sizeof(date), "%d%m%Y", timeDetail);
    sprintf(fileName, "report_%s.txt", date);
    FILE *report = fopen(fileName, "w");

    for(int i = 0; i < countryCount; i++){
        fprintf(report, "%d. Negara: %s\n", (i+1), stat[i].country);
        fprintf(report, "Film sebelum 2000: %d\n", stat[i].before);
        fprintf(report, "Film setelah 2000: %d\n\n", stat[i].after);
    }
    fclose(report);
}

int main(){
    int a;
    char *path = "/home/freddskii/netflixData.csv";

    while(a != 4){
        printf("1. Download file\n");
        printf("2. Sorting film\n");
        printf("3. Ultimate report\n");
        printf("4. Exit\n");
        printf("Input : ");
        scanf("%d", &a);

        switch(a){
            case 1 :
                download();
                break;
            case 2 :
                int stat;
                pid_t pid1, pid2;

                pid1 = fork();
                if(pid1 == 0){category(path, "judul"); exit(0);}
                pid2 = fork();
                if(pid2 == 0){category(path, "tahun"); exit(0);}

                wait(&stat);
                wait(&stat);
                break;
            case 3 :
                report(path);
                break;
            case 4 :
                return 0;
        }
    }
}
