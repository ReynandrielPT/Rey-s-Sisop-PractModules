#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>

typedef struct {
    char isi[256];
    int jum;
    int stat;
} shared;

typedef struct {
    long bruh;
    char isi[256];
} mes;

int main() {
    int jum_worker;
    printf("jumlah worker: ");
    scanf("%d", &jum_worker);

    int que[jum_worker];
    for (int i = 0; i < jum_worker; i++) {
        key_t key = 73 + i;
        que[i] = msgget(key, IPC_CREAT | 0666);
        
    }
    int shmid = shmget(37, sizeof(shared), 0666);
    shared *data = (shared *)shmat(shmid, NULL, 0);
    FILE *log = fopen("system.log", "a");

    int giliran = 0;
    while (1) {
        if (data->stat == 1) {
            for (int i = 0; i < data->jum; i++) {
                if (log) {
                    fprintf(log, "Received at lb: %s (#message %d)\n", data->isi, i + 1);
                    fflush(log);
                }

                mes msg;
                msg.bruh = 1;
                snprintf(msg.isi, sizeof(msg.isi), "%s", data->isi);
                msgsnd(que[giliran], &msg, sizeof(msg.isi), 0);
                giliran = (giliran + 1) % jum_worker;
                usleep(10000); 
            }
            data->stat = 0;
            if (log) fclose(log);
            break;
        }
        usleep(50000);
    }

    return 0;
}
