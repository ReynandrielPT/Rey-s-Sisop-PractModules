#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

int countHorror = 0, countAnimasi = 0, countDrama = 0;
pthread_mutex_t count = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t file = PTHREAD_MUTEX_INITIALIZER;

typedef struct{
    int isTrabowo;
    char *name, *loc;
} fileTask;

int number(const char *fileName){
    int num;
    sscanf(fileName, "%d", &num);
    return num;
}

void log(const char *path, const char *worker, const char *fileName, const char *genre){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time[20], log[200];
    strftime(time, sizeof(time), "%d-%m-%Y %H:%M:%S", t);
    sprintf(log, "%s/recap.txt", path);
    pthread_mutex_lock(&file);
    FILE *logFile = fopen(log, "a");
    if(logFile){
        fprintf(logFile, "[%s] %s: %s telah dipindahkan ke %s\n", time, worker, fileName, genre);
        fclose(logFile);
    }
    pthread_mutex_unlock(&file);
}

void genreCount(const char *genre){
    pthread_mutex_lock(&count);
    if(strcmp(genre, "FilmHorror") == 0) countHorror++;
    else if(strcmp(genre, "FilmAnimasi") == 0) countAnimasi++;
    else if(strcmp(genre, "FilmDrama") == 0) countDrama++;
    pthread_mutex_unlock(&count);
}

void* worker(void *arg){
    fileTask *task = (fileTask *)arg;
    char *genre = NULL;
    char old[200], newPath[200];

    if(strstr(task->name, "horror")) genre = "FilmHorror";
    else if(strstr(task->name, "animasi")) genre = "FilmAnimasi";
    else if(strstr(task->name, "drama")) genre = "FilmDrama";

    if(genre != NULL){
        sprintf(newPath, "%s/%s", task->loc, genre);
        mkdir(newPath, 0777);

        sprintf(old, "%s/%s", task->loc, task->name);
        sprintf(newPath, "%s/%s/%s", task->loc, genre,  task->name);
        rename(old, newPath);

        const char* worker;
        if(task->isTrabowo) worker = "Trabowo";
        else worker = "Peddy";
        log(task->loc, worker, task->name, genre);
        genreCount(genre);
    }
    return NULL;
}

int main(){
    char path[] = "/home/freddskii/film";
    char *name[100];
    struct dirent *entry;
    pthread_t threads[100];

    DIR *directory = opendir(path);
    int i = 0;
    while((entry = readdir(directory)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, "recap.txt") == 0 || strcmp(entry->d_name, "total.txt") == 0) continue;
        name[i] = strdup(entry->d_name);
        i++;
    }
    closedir(directory);

    char initLog[200];
    sprintf(initLog, "%s/recap.txt", path);
    FILE *logFile = fopen(initLog, "w");
    if(logFile) fclose(logFile);

    for(int j = 0 ; j < i ; j++){
        for(int k = 0 ; k < i-1 ; k++){
            if(number(name[k]) > number(name[k+1])){
                char *temp = name[k];
                name[k] = name[k+1];
                name[k+1] = temp;
            }
        }
    }

    int indexTrab = 0, indexPedd = i - 1;
    for(int j = 0; j < i/2; j++){
        fileTask *task1 = (fileTask *)malloc(sizeof(fileTask));
        task1->name = strdup(name[j]);
        task1->loc = path;
        task1->isTrabowo = 1;
        if(pthread_create(&threads[j], NULL, worker, task1) == 0) pthread_join(threads[j], NULL);

        fileTask *task2 = (fileTask *)malloc(sizeof(fileTask));
        task2->name = strdup(name[i-j-1]);
        task2->loc = path;
        task2->isTrabowo = 0;
        if(pthread_create(&threads[i - j - 1], NULL, worker, task2) == 0) pthread_join(threads[j], NULL);
    }

    char totalPath[200];
    sprintf(totalPath, "%s/total.txt", path);
    FILE *totalFile = fopen(totalPath, "w");

    const char *maxGenre;
    int maxCount = 0;

    if(countHorror > maxCount){maxCount = countHorror; maxGenre = "Horror";}
    if(countAnimasi > maxCount){maxCount = countAnimasi; maxGenre = "Animasi";}
    if(countDrama > maxCount){maxCount = countDrama; maxGenre = "Drama";}

    fprintf(totalFile, "Jumlah film animasi: %d\n", countAnimasi);
    fprintf(totalFile, "Jumlah film drama: %d\n", countDrama);
    fprintf(totalFile, "Jumlah film horror: %d\n", countHorror);
    fprintf(totalFile, "Genre dengan jumlah film terbanyak: %s\n", maxGenre);
    fclose(totalFile);
    return 0;
}
