#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

typedef struct {
    char filename[256];
    char genre[20];
} Movie;

void mkbjer(char *dir_name) {
    struct stat st = {0};
    if (stat(dir_name, &st) == -1) { if (mkdir(dir_name, 0700) != 0) { perror("bruh");} }}


void recap(char *worker, char *filename, char *genre) {
    FILE *logFile = fopen("recap.txt", "a");
    if (logFile == NULL) {
        perror("Fbruh");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(logFile, "[%02d-%02d-%04d %02d:%02d:%02d] %s: %s telah dipindahkan ke %s\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, worker, filename, genre);
    fclose(logFile);
}
void pindah(char *file, char *desti) {
    char pathhh[512];
    char dir_path[512];
    char desti_path[512];

    snprintf(pathhh, sizeof(pathhh), "./film/%s", file);
    snprintf(desti_path, sizeof(desti_path), "./film/%s/%s", desti, file);
    snprintf(dir_path, sizeof(dir_path), "./film/%s", desti);
    mkbjer(dir_path); 
    if (rename(pathhh, desti_path) != 0) {
        perror("bruh");
    }
}
void saring(char *dir_path, Movie *muvi, int *count) {
    DIR *dir = opendir(dir_path);
    struct dirent *entry;
    if (dir == NULL) {
        perror("dir bruh");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  
            strcpy(muvi[*count].filename, entry->d_name);
            if (strstr(entry->d_name, "horror") != NULL) {
                strcpy(muvi[*count].genre, "FilmHorror");
            } else if (strstr(entry->d_name, "animasi") != NULL) {
                strcpy(muvi[*count].genre, "FilmAnimasi");
            } else if (strstr(entry->d_name, "drama") != NULL) {
                strcpy(muvi[*count].genre, "FilmDrama");
            } else {
                continue;  
            }
            (*count)++;
        }
    }
    closedir(dir);
}

int main() {
    Movie muvi[1000];
    int muviCou = 0;
    char *dir_path = "./film";
    saring(dir_path, muvi, &muviCou);
    int i = 0, j = muviCou - 1;
    int horrorCou = 0, animasiCou = 0, dramaCou = 0;
    while (i <= j) {
        if (strstr(muvi[i].genre, "FilmHorror") != NULL) {
            horrorCou++;
        } else if (strstr(muvi[i].genre, "FilmAnimasi") != NULL) {
            animasiCou++;
        } else if (strstr(muvi[i].genre, "FilmDrama") != NULL) {
            dramaCou++;
        }
        pindah(muvi[i].filename, muvi[i].genre);
        recap("Trabowo", muvi[i].filename, muvi[i].genre);
        i++;
        if (i <= j) {
            if (strstr(muvi[j].genre, "FilmHorror") != NULL) {
                horrorCou++;
            } else if (strstr(muvi[j].genre, "FilmAnimasi") != NULL) {
                animasiCou++;
            } else if (strstr(muvi[j].genre, "FilmDrama") != NULL) {
                dramaCou++;
            }
            pindah(muvi[j].filename, muvi[j].genre);
            recap("Peddy", muvi[j].filename, muvi[j].genre);
            j--;
        }
    }
    FILE *fileTOT = fopen("total.txt", "w");
    if (fileTOT == NULL) {
        perror("bruh");
        return 1;
    }
    fprintf(fileTOT, "Jumlah film horror: %d\n", horrorCou);
    fprintf(fileTOT, "Jumlah film animasi: %d\n", animasiCou);
    fprintf(fileTOT, "Jumlah film drama: %d\n", dramaCou);
    if (horrorCou >= animasiCou && horrorCou >= dramaCou) {
        fprintf(fileTOT, "Film terbanyak: FilmHorror\n");
    } else if (animasiCou >= horrorCou && animasiCou >= dramaCou) {
        fprintf(fileTOT, "Film terbanyak: FilmAnimasi\n");
    } else {
        fprintf(fileTOT, "Film terbanyak: FilmDrama\n");
    }
    fclose(fileTOT);
    return 0;
}
/*   /\_/\
*    (= ._.)
*   / >  \>
*/
// sorry gabut mas hehehehehhee
