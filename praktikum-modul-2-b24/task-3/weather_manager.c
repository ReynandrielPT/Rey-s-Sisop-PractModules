#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX 1024

void logging(const char* act_msg) {
    char msg[1024];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(msg, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n", 
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, act_msg);
    
    FILE *logbook = fopen("logbook.txt", "a");
    fprintf(logbook, "%s", msg);
    fclose(logbook);
}

void log_drop(const char* suhu, int value) {
    char msg[100];
    sprintf(msg, "Successfully drop row with %s %d°C", suhu, value);
    logging(msg);
}

void ensure(const char *name) {
    struct stat st = {0};
    if (stat(name, &st) == -1) {
        mkdir(name, 0700);
    }
}

void downloadunzip(const char *url, const char *o) {
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        execlp("curl", "curl", url, "-Lo", "data.zip", NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
    if (WIFEXITED(status)) {
        logging("Successfully download file.");
    } 

    pid = fork();
    if (pid == 0) {
        execlp("unzip", "unzip", "-o", "data.zip", "-d", o, NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
    if (WIFEXITED(status)) {
        logging("Successfully unzip file.");
    }

    pid = fork();
    if (pid == 0) {
        execlp("rm", "rm", "data.zip", NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
}

void preprocess(const char *fp, const char *o) {
    char temp_fp[256];
    sprintf(temp_fp, "%s/temp.csv", o);

    FILE *fp_in = fopen(fp, "r");
    FILE *fp_out = fopen(temp_fp, "w");
    if (fp_in == NULL || fp_out == NULL) {
        perror("Failed to open file");
        return;
    }

    char line[MAX];
    char curr_line[MAX];
    fgets(line, sizeof(line), fp_in);
    while (fgets(line, sizeof(line), fp_in) != NULL) {
        strcpy(curr_line, line);
        char *tokens = strtok(line, ",");
        int ind = 0;
        int at;
        int keep = 1;

        while (tokens) {
            if (ind == 3) { 
                at = atoi(tokens);
                if (at > 35) {
                    keep = 0;
                    log_drop("Suhu Udara", at);
                    break;
                }
            }
            tokens = strtok(NULL, ",");
            ind++;
        }

        if (keep) {
            fprintf(fp_out, "%s", curr_line);
        }
        strcpy(line, "");
        strcpy(curr_line, "");
    }

    fclose(fp_in);
    fclose(fp_out);

    remove(fp);
    rename(temp_fp, fp);
}

void grouping(const char *input_file, const char *dir) {
    FILE *fp_in = fopen(input_file, "r");
    if (!fp_in) {
        perror("Failed to open input file");
        return;
    }

    ensure(dir);

    char line[MAX], last_id[50] = {0}, curr_line[MAX];
    FILE *fp_out = NULL;

    while (fgets(line, sizeof(line), fp_in)) {
        strcpy(curr_line, line);
        char *token = strtok(line, ",");
        char *location_id = token;

        if (fp_out && strcmp(last_id, location_id) != 0) {
            fclose(fp_out);
            fp_out = NULL;
        }

        if (!fp_out) {
            char path[256];
            snprintf(path, sizeof(path), "%s/city_%s.csv", dir, location_id);
            fp_out = fopen(path, "w");
            if (!fp_out) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            strcpy(last_id, location_id);
        }

        fprintf(fp_out, "%s", curr_line);
    }

    if (fp_out) {
        fclose(fp_out);
    }
    fclose(fp_in);
    logging("Successfully data grouping..");
}

int main() {
    const char *url = "https://drive.usercontent.google.com/u/0/uc?id=1U1-9v9pJeX5MoofwoBHCcD9nA7YWy_np&export=download";
    const char *o = "weather";
    const char *fp = "weather/kecamatanforecast.csv";

    downloadunzip(url, o);
    preprocess(fp, o);
    grouping(fp, "weather/city_group");

    return 0;
}