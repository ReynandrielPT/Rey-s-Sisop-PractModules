#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

typedef struct {
    long jenis;
    char isi[256];
} Message;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Format: ./worker <nomor_worker>\n");
        return 1;
    }

    int id_worker = atoi(argv[1]);
    key_t key = 73 + (id_worker - 1);
    int msgid = msgget(key, 0666);

    FILE *log = fopen("system.log", "a");

    Message msg;
    int counter = 0;

    while (1) {
        ssize_t result = msgrcv(msgid, &msg, sizeof(msg.isi), 0, IPC_NOWAIT);
        if (result > 0) {
            fprintf(log, "Worker%d: message received\n", id_worker);
            fflush(log);
            counter++;
        } else {
            if (counter > 0) break; 
            usleep(100000); 
        }
    }

    fprintf(log, "Worker %d: %d messages\n", id_worker, counter);
    fclose(log);
    return 0;
}
