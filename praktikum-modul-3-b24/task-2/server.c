#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    long message_type;
    char user[64];
    char nama[64];
    char image[64];
    char cmd[300];
    char volum[64];
    int login;
    int client_id;
    int repeat;
} Message;

int checkusername(Message msgc[5], Message msga, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(msgc[i].user, msga.user) == 0) {
            return 0;
        }
    }
    return 1;
}

int checkdockername(Message msgc[5], Message msga, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(msgc[i].nama, msga.nama) == 0) {
            return 0;
        }
    }
    return 1;
}

void create_docker_compose(Message msg) {
    FILE *file = fopen("./docker-compose.yml", "w");
    if (!file) {
        perror("Failed to open docker-compose.yml");
        return;
    }
    fprintf(file, "version: '3'\nservices:\n");
    fprintf(file, "  %s:\n    image: %s\n    container_name: %s\n    command: \"%s\"\n    volumes:\n      - %s\n",
            msg.nama, msg.image, msg.nama, msg.cmd, msg.volum);
    fclose(file);
}

int main() {
    key_t key = 911;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    Message msg[5], msgf;
    int count = 0, jumlah;
    char pesan[256];

    printf("Enter number of clients (1-5): ");
    scanf("%d", &jumlah);

    while (count < jumlah) {
        printf("Waiting for client %d...\n", count + 1);
        msgrcv(msgid, &msgf, sizeof(msgf), 6, 0);
        printf("Client with username %s connected\n", msgf.user);
        if (checkusername(msg, msgf, count)) {
            strcpy(msg[count].user, msgf.user);
            printf("Username %s accepted\n", msg[count].user);
            msgf.login = 1;
            msgf.client_id = count + 1;
            msgf.message_type = 7;
            msgsnd(msgid, &msgf, sizeof(msgf), 0);
            printf("Confirmation sent to client %d\n", count + 1);
            count++;
        } else {
            msgf.login = 0;
            printf("Username %s rejected\n", msgf.user);
            msgf.message_type = 7;
            msgsnd(msgid, &msgf, sizeof(msgf), 0);
        }
    }
    for (int i = 1; i <= jumlah; i++) {
        msgf.message_type = i;
        msgsnd(msgid, &msgf, sizeof(msgf), 0);
        printf("Client %d notified\n", i);
        msgrcv(msgid, &msgf, sizeof(msgf), 6, 0);
        printf("Client %d responded\n", i);
        strcpy(msg[i-1].nama, msgf.nama);
        strcpy(msg[i-1].image, msgf.image);
        strcpy(msg[i-1].cmd, msgf.cmd);
        strcpy(msg[i-1].volum, msgf.volum);
        printf("Info from client %d saved, creating docker compose.\n", i);

        create_docker_compose(msg[i-1]);
    
        int status;
        pid_t pid = fork();
        if (pid == 0) {
            execlp("docker", "docker", "compose", "down", NULL);
            exit(0);
        } else while (wait(&status) > 0);
        pid = fork();
        if (pid == 0) {
            execlp("docker", "docker", "compose", "build", NULL);
            exit(0);
        } else while (wait(&status) > 0);
        pid = fork();
        if (pid == 0) {
            execlp("docker", "docker", "compose", "up", "-d", NULL);
            exit(0);
        } else while (wait(&status) > 0);
        printf("Finished creating docker compose, sending confirmation to client %d\n", i);
        msgf.message_type = i;
        msgsnd(msgid, &msgf, sizeof(msgf), 0);
        printf("Waiting for client %d to respond\n", i);
        msgrcv(msgid, &msgf, sizeof(msgf), 6, 0);
        printf("Client %d responded with %d\n", i, msgf.repeat);
        if (msgf.repeat != 0) i--;
    }
    printf("All clients are finished. Closing\n");
    
    msgctl(msgid, IPC_RMID, NULL);  // Clean up the message queue
    return 0;
}
   
