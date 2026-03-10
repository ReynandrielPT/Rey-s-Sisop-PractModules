#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

typedef struct msg_buffer {
    long mesg_type;       
    char mesg_text[1000];
    int order;
    bool last;
} Mesg_buffer;


void write_to_log(int curr_worker, int order) {
    FILE *log = fopen("/home/riyan/sisop_ws/sistem.log", "a");
    if (log) {
        fprintf(log, "Worker %d: message received (message #%d)\n", curr_worker, order);
        fclose(log);
    }
    
   
}

void write_total_received(int n, int curr_worker) {
    FILE *log = fopen("/home/riyan/sisop_ws/sistem.log", "a");
    if (log) {
        fprintf(log, "Worker %d: %d messages\n", curr_worker, n);
        fclose(log);
    }
   
}

int main(int argc, char **argv) {
 
    if (argc != 2) {
        printf("usage: %s <number of workers>\n", argv[0]);
        return 1;
    }

    int worker = atoi(argv[1]);
    if (worker <= 0) {
        printf("invalid number of workers: %s\n", argv[1]);
        return 1;
    }

    for (int i = 0; i < worker; i++) {
        char sem_name[40];
        char sem_terminate[40];

        sprintf(sem_name, "/log_sem_%d", i);
        sprintf(sem_terminate, "/terminate_sem_%d", i);

        sem_unlink(sem_name);
        sem_unlink(sem_terminate); 

        sem_t *sem = sem_open(sem_name, O_CREAT | O_EXCL, 0666, (i == 0) ? 1 : 0);
        sem_t *semt = sem_open(sem_terminate, O_CREAT | O_EXCL, 0666, 0);
    
        sem_close(sem);
        sem_close(semt);
    }

    key_t key = 4567;
    int msg_queue[worker];
    for (int i = 0; i < worker; i++) {
        msg_queue[i] = msgget(key + i, 0666 | IPC_CREAT);
    }

    for (int i = 0; i < worker; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            Mesg_buffer msg;
            int worker_id = i;
            int count = 0;

            char my_sem_name[20], next_sem_name[20];
            char my_semt_name[40], next_semt_name[40];
            char first_semt_name[40];

            sprintf(my_sem_name, "/log_sem_%d", worker_id);
            sem_t *my_sem = sem_open(my_sem_name, 0);

            sprintf(my_semt_name, "/terminate_sem_%d", worker_id);
            sem_t *my_semt = sem_open(my_semt_name, 0);

            int next_worker = (worker_id + 1) % worker;

            sprintf(next_sem_name, "/log_sem_%d", next_worker);
            sem_t *next_sem = sem_open(next_sem_name, 0);

            sprintf(next_semt_name, "/terminate_sem_%d", next_worker);
            sem_t *next_semt = sem_open(next_semt_name, 0);

            sprintf(first_semt_name, "/terminate_sem_%d", 0);
            sem_t *first_semt = sem_open(first_semt_name, 0);
            

            while (1) {
                msgrcv(msg_queue[i], &msg, sizeof(msg.mesg_text) + sizeof(msg.order) + sizeof(msg.last), 1, 0);
             
                if (strcmp(msg.mesg_text, "TERMINATE") == 0) {
                    sem_post(next_sem);
                    break;
                }
                sem_wait(my_sem);
                write_to_log(worker_id + 1, msg.order);
                sem_post(next_sem);

                if(msg.last == true){
                    sem_post(first_semt);
                }
                

                count++;
            }

            sem_wait(my_semt);
            write_total_received(count, worker_id + 1);
            sem_post(next_semt);

            msgctl(msg_queue[i], IPC_RMID, NULL);
            _exit(0);
        }
    }
    
    for (int i = 0; i < worker; i++) {
        wait(NULL);
    }


    

    return 0;
}
