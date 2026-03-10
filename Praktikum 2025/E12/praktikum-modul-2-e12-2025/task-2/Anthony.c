#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>

#define MAX_LINE 2001
#define MAX_NEGARA 2001

typedef struct {
    char negara[1001];
    int sebelum;
    int sesudah;
} Nstat;

typedef struct {
    char negara[1001];
    char **data;
    int jumlah;
    Nstat *result;
} ThreadArg;

void log(const char *kategori, const char *judul);
void folder(const char *nama);
void proses_tahun();
void sorting();
void group();
void laporan();
void *hitung_statistik(void *arg);

// Fungsi untuk menghapus leading dan trailing whitespace dari string
void trim(char *str) {
    int start = 0;
    int end = strlen(str) - 1;
    while (isspace(str[start])) start++;
    while (end >= start && isspace(str[end])) end--;
    int i;
    for (i = start; i <= end; i++) str[i - start] = str[i];
    str[i - start] = '\0';
}

// Fungsi untuk membersihkan karakter non-printable dari string
void clean_string(char *str) {
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (isprint(str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

void one_click() {
    pid_t pid = fork();
    if (pid == 0) {
        char *url = "https://drive.google.com/uc?export=download&id=12GWsZbSH858h2HExP3x4DfWZB1jLdV-J";
        char *argv[] = {"wget", "-O", "netflixData.zip", url, NULL};
        execv("/usr/bin/wget", argv);
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
        pid = fork();
        if (pid == 0) {
            mkdir("/home/ubuntu/prak2", 0777);
            char *argv[] = {"unzip", "-o", "netflixData.zip", "-d", "/home/ubuntu/prak2", NULL};
            execv("/usr/bin/unzip", argv);
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
            pid = fork();
            if (pid == 0) {
                char *argv[] = {"rm", "-f", "netflixData.zip", NULL};
                execv("/bin/rm", argv);
                exit(EXIT_FAILURE);
            } else {
                wait(NULL);
                printf("Download dan ekstrak selesai\n");
            }
        }
    }
}

void sorting() {
    folder("judul");
    FILE *in = fopen("/home/ubuntu/prak2/netflixData.csv", "r");
    if (!in) return;
    char line[MAX_LINE];
    fgets(line, sizeof(line), in); // Skip header
    while (fgets(line, sizeof(line), in)) {
        char *judul = strtok(line, ",");
        char *sutradara = strtok(NULL, ",");
        char *negara = strtok(NULL, ",");
        char *tahun = strtok(NULL, ",\n");

        if (!judul || !tahun || !sutradara) continue;
        char first = toupper(judul[0]);
        char filename[100];
        if (isalnum(first)) sprintf(filename, "judul/%c.txt", first);
        else sprintf(filename, "judul/#.txt");

        FILE *out = fopen(filename, "a");
        if (out) {
            fprintf(out, "%s - %s - %s\n", judul, tahun, sutradara);
            fclose(out);
        }
        log("abjad", judul);
    }
    fclose(in);
}

void proses_tahun() {
    folder("tahun");
    FILE *in = fopen("/home/ubuntu/prak2/netflixData.csv", "r");
    if (!in) {
        perror("File tidak ditemukan");
        return;
    }
    char line[MAX_LINE];
    fgets(line, sizeof(line), in); // Skip header
    while (fgets(line, sizeof(line), in)) {
        char *judul = strtok(line, ",");
        char *sutradara = strtok(NULL, ",");
        char *negara = strtok(NULL, ",");
        char *tahun = strtok(NULL, ",\n");

        if (!judul || !tahun) continue;

        // Menghapus spasi di awal tahun
        while (*tahun == ' ') tahun++;

        // Validasi tahun
        if (strlen(tahun) == 0 || !isdigit(tahun[0])) continue;

        char filename[256];
        sprintf(filename, "tahun/%s.txt", tahun);

        FILE *out = fopen(filename, "a");
        if (out) {
            fprintf(out, "%s - %s - %s\n", judul, tahun, sutradara ? sutradara : "N/A");
            fclose(out);
        }
        log("tahun", judul);
    }
    fclose(in);
}

void group() {
    pid_t pid1 = fork();
    if (pid1 == 0) {
        sorting();
        exit(0);
    }
    pid_t pid2 = fork();
    if (pid2 == 0) {
        proses_tahun();
        exit(0);
    }
    wait(NULL);
    wait(NULL);
    printf("selesai.\n");
}

void log(const char *kategori, const char *judul) {
    FILE *log = fopen("log.txt", "a");
    if (!log) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(log, "[%02d:%02d:%02d] sort by %s: untuk %s\n",
            t->tm_hour, t->tm_min, t->tm_sec, kategori, judul);
    fclose(log);
}

void folder(const char *nama) {
    struct stat st = {0};
    if (stat(nama, &st) == -1) {
        mkdir(nama, 0777);
    }
}

void *hitung_statistik(void *arg) {
    ThreadArg *ta = (ThreadArg *)arg;
    int before = 0, after = 0;

    for (int i = 0; i < ta->jumlah; i++) {
        char line_copy[MAX_LINE];
        strcpy(line_copy, ta->data[i]);

        char *judul = strtok(line_copy, ",");
        char *sutradara = strtok(NULL, ",");
        char *negara = strtok(NULL, ",");
        char *tahun_str = strtok(NULL, ",\n");

        if (!tahun_str || !negara) continue;
        int tahun = atoi(tahun_str);
        if (tahun < 2000) before++;
        else after++;
    }

    strcpy(ta->result->negara, ta->negara);
    ta->result->sebelum = before;
    ta->result->sesudah = after;
    pthread_exit(NULL);
}

void laporan() {
    FILE *in = fopen("/home/ubuntu/prak2/netflixData.csv", "r");
    if (!in) {
        perror("not found!");
        return;
    }

    char *data[1000];
    int j_data = 0;
    char line[MAX_LINE];
    char n_unix[MAX_NEGARA][1001];
    int n_count = 0;

    fgets(line, sizeof(line), in); // Skip header
    while (fgets(line, sizeof(line), in)) {
        if (j_data < 1000) {
            data[j_data++] = strdup(line);
        }
    }
    fclose(in);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char filename[1001];
    sprintf(filename, "report_%02d%02d%04d.txt", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    FILE *out = fopen(filename, "w");

    for (int i = 0; i < j_data; i++) {
        char *copy = strdup(data[i]);
        char *judul = strtok(copy, ",");
        char *sutradara = strtok(NULL, ",");
        char *negara_ptr = strtok(NULL, ",");
        char *tahun_str = strtok(NULL, ",\n");

        if (negara_ptr && tahun_str) {
            char negara[1001];
            strcpy(negara, negara_ptr);
            trim(negara);
            clean_string(negara);
            int tahun = atoi(tahun_str);

            if (strlen(negara) > 0) {
                int found = -1;
                for (int j = 0; j < n_count; j++) {
                    if (strcmp(n_unix[j], negara) == 0) {
                        found = j;
                        break;
                    }
                }

                if (found == -1) {
                    strcpy(n_unix[n_count], negara);
                    found = n_count++;
                    fprintf(out, "%d. Negara: %s\n", found + 1, n_unix[found]);
                    fprintf(out, "Film sebelum 2000: 0\n");
                    fprintf(out, "Film setelah 2000: 0\n\n");
                }

                // Cari kembali dan update hitungan
                int sebelum = 0;
                int sesudah = 0;
                for (int k = 0; k < j_data; k++) {
                    if (data[k] != NULL) {
                        char *temp_copy = strdup(data[k]);
                        char *judul_temp = strtok(temp_copy, ",");
                        char *sutradara_temp = strtok(NULL, ",");
                        char *temp_negara_ptr = strtok(NULL, ",");
                        char *temp_tahun_str = strtok(NULL, ",\n");
                        if (temp_negara_ptr && temp_tahun_str) {
                            char temp_negara[1001];
                            strcpy(temp_negara, temp_negara_ptr);
                            trim(temp_negara);
                            clean_string(temp_negara);
                            int temp_tahun = atoi(temp_tahun_str);
                            if (strcmp(temp_negara, n_unix[found]) == 0) {
                                if (temp_tahun < 2000) {
                                    sebelum++;
                                } else {
                                    sesudah++;
                                }
                            }
                        }
                        free(temp_copy);
                    }
                }
                // Tulis ulang laporan yang sudah ada dengan hitungan yang benar
                fseek(out, 0, SEEK_SET);
                for (int l = 0; l < n_count; l++) {
                    int sblm = 0;
                    int ssdh = 0;
                    for (int k = 0; k < j_data; k++) {
                        if (data[k] != NULL) {
                            char *temp_copy2 = strdup(data[k]);
                            char *judul_temp2 = strtok(temp_copy2, ",");
                            char *sutradara_temp2 = strtok(NULL, ",");
                            char *temp_negara_ptr2 = strtok(NULL, ",");
                            char *temp_tahun_str2 = strtok(NULL, ",\n");
                            if (temp_negara_ptr2 && temp_tahun_str2) {
                                char temp_negara2[1001];
                                strcpy(temp_negara2, temp_negara_ptr2);
                                trim(temp_negara2);
                                clean_string(temp_negara2);
                                int temp_tahun2 = atoi(temp_tahun_str2);
                                if (strcmp(temp_negara2, n_unix[l]) == 0) {
                                    if (temp_tahun2 < 2000) sblm++;
                                    else ssdh++;
                                }
                            }
                            free(temp_copy2);
                        }
                    }
                    fprintf(out, "%d. Negara: %s\n", l + 1, n_unix[l]);
                    fprintf(out, "Film sebelum 2000: %d\n", sblm);
                    fprintf(out, "Film setelah 2000: %d\n\n", ssdh);
                }
            }
        }
        free(copy);
    }
    fclose(out);

    for (int i = 0; i < j_data; i++) free(data[i]);
    printf("done %s\n", filename);
}

int main() {
    int pilihan;
    while (1) {
        printf("\n-=+=- MENU -=+=-\n");
        printf("1. Download File \n");
        printf("2. Mengelompokkan Film \n");
        printf("3. Membuat Report\n");
        printf("4. Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1:
                one_click(); break;
            case 2:
                group(); break;
            case 3:
                laporan(); break;
            case 4:
                exit(0);
        }
    }
    return 0;
}
