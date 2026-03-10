#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>

typedef struct msg_t{
    sem_t empty;
    sem_t full;
    int repeat;
    char message[1000];
}Msg;


void write_to_log(char message[1000], int count){
    pid_t child_id;
    int status;
    char cmd[1200];
    child_id = fork();

    if (child_id < 0) {
        perror("fork");
        return;
    }

    if (child_id == 0) {
        char *argv[] = {"touch", "/home/riyan/sisop_ws/sistem.log",NULL};
        execv("/bin/touch", argv);
        
    }

    else {
        while (wait(&status) > 0);
        sprintf(cmd, "echo Message from client: %s >> /home/riyan/sisop_ws/sistem.log; " "echo Message count: %d >> /home/riyan/sisop_ws/sistem.log", message, count);

        char *argv2[] = {"sh", "-c", cmd,NULL};
        execv("/bin/sh", argv2);

    }
}

int main(int argc, char** argv)
{
    // ekstrak input dari command line
    int count = 0;
    int semicolon = 0;
    if(argc != 2){
        printf("usage: ./client '<your message>; <count>'' ");
        return 1;
    }
    int len = strlen(argv[1]);

    //cari indeks ;
    for(int i=0; i<len; i++){
        if(argv[1][i] == ';'){
            semicolon = i;
            break;
        }
    }

    //copy dari indeks 0 -> ;
    char msg[1000] = "";
    strncat(msg, argv[1], semicolon);
    
    //copy angka
    char num[5] = ""; 
    int j=0;
    for(int i=semicolon+1; i<len; i++){
        num[j] = argv[1][i];
        j++;
    }

    //convert string ke integer
    count = atoi(num);
    pid_t child_id;
    child_id = fork();

    if(child_id == 0){
        write_to_log(msg, count);
    }
    else{
    key_t key = 1234;
    int shmid = shmget(key, sizeof(Msg), IPC_CREAT | 0666);
    Msg* send = shmat(shmid, NULL, 0);

    send->repeat = count;
    sem_init(&send->empty, 1, 1);
    sem_init(&send->full, 1, 0);

    for(int i=0; i<count; i++){
        sem_wait(&send->empty);
        strncpy(send->message, msg, strlen(msg));
        sem_post(&send->full);
    }
    shmdt(send);
    shmctl(shmid, IPC_RMID, NULL);
    }

}
