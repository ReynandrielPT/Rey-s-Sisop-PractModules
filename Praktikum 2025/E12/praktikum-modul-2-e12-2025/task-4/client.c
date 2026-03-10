#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
// bruh
typedef struct {
    char isi[256];
    int jum;
    int stat; 
} shared;

int main() {
    char input[300];
    printf("Masukkan pesan dan jumlah\nformat: (pesan;jumlah): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    char *isi = strtok(input, ";");
    char *jum_str = strtok(NULL, ";");
    int jum = atoi(jum_str);
    int shmid = shmget(37, sizeof(shared), IPC_CREAT | 0666);
   
    shared *data = (shared *)shmat(shmid, NULL, 0);
    strncpy(data->isi, isi, 256);
    data->jum = jum;
    data->stat = 1;

    FILE *log = fopen("system.log", "a");
    if (log) {
        fprintf(log, "Message from client: %s\n", isi);
        fprintf(log, "Message count: %d\n", jum);
        fclose(log);
    }

    printf("DONE\n");
    return 0;
}
