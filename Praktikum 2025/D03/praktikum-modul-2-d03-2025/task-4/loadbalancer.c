#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <errno.h>

typedef struct msg_t {
    sem_t empty;
    sem_t full;
    int repeat;
    char message[1000];
} Msg;

typedef struct msg_buffer {
    long mesg_type;
    char mesg_text[1000];
    int order;
    bool last;
} Mesg_buffer;

void write_to_log(const char *message, int count){
    FILE *log = fopen("/home/riyan/sisop_ws/sistem.log", "a");
    if(log) {
        fprintf(log, "Received at lb: %s (#message %d)\n", message, count);
        fclose(log);
    }else{
        perror("fopen log file");
    }
}

int main(int argc, char **argv){
    if (argc != 2) {
        printf("usage: %s <number of workers>\n", argv[0]);
        return 1;
    }

    int worker = atoi(argv[1]);
    if(worker<=0) {
        printf("invalid number of workers.\n");
        return 1;
    }

    // shared memory
    key_t shm_key = 1234;
    int shmid = shmget(shm_key, sizeof(Msg), 0666);

    Msg *send = (Msg *)shmat(shmid, NULL, 0);
  

    // message queue
    key_t msg_key = 4567;
    int msg_queue[worker];

    for(int i=0; i<worker; i++){
        msg_queue[i] = msgget(msg_key + i, 0666 | IPC_CREAT);
    }
    for(int i=0; i<send->repeat; i++) {
        sem_wait(&send->full);     
        write_to_log(send->message, i + 1);

        Mesg_buffer msg_send;
        msg_send.mesg_type = 1;
        msg_send.order = i+1;
        if(i==send->repeat-1) msg_send.last = true;
        else msg_send.last = false;
        strncpy(msg_send.mesg_text, send->message, strlen(send->message));

        msgsnd(msg_queue[(i)%worker], &msg_send, sizeof(msg_send.mesg_text) + sizeof(msg_send.order) + sizeof(msg_send.last), 0);

        sem_post(&send->empty);
     
    }

  
    for(int i = 0; i <worker; i++) {
        Mesg_buffer term_msg;
        term_msg.mesg_type = 1;

        strcpy(term_msg.mesg_text, "TERMINATE");
        term_msg.order = send->repeat + i;
        term_msg.last = false;

        msgsnd(msg_queue[i], &term_msg, sizeof(term_msg.mesg_text) + sizeof(term_msg.order) + sizeof(term_msg.last), 0);
        
    }


    shmdt(send);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
