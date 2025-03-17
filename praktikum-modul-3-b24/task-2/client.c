#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct
{
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

void clr_buf() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}


int main() {
    key_t key = 911;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    char temp[64];
    Message msg;
    int current_client_id = 0;

    while (1) {
        printf("Masukkan username: ");
        scanf("%s", temp);
        strcpy(msg.user, temp);
        msg.message_type = 6;
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msgrcv(msgid, &msg, sizeof(msg), 7, 0);

        if (msg.login == 1) {
            break;
        }

        printf("Udah ada!\n");
    }
    
    printf("Passed as client id %d\n", msg.client_id);
    current_client_id = msg.client_id;
    
    while(1){
        msg.login = 0;
        if (msg.client_id > 1) printf("Waiting for client %d...\n", msg.client_id - 1);
        msgrcv(msgid, &msg, sizeof(msg), msg.client_id, 0);
        printf("Infokan kontainer yang ingin kamu buat\n");
        printf("Masukkan nama kontainer yang akan kamu buat : ");
        scanf("%s",msg.nama);
        printf("Masukkan image yang digunakan : ");
        scanf("%s",msg.image);
        clr_buf();
        printf("Masukkan perintah saat kontainer dimulai : ");
        scanf("%[^\n]s",msg.cmd);
        printf("Masukkan volume dari container : ");
        scanf("%s",msg.volum);
        msg.message_type=6;
        msgsnd(msgid,&msg,sizeof(msg),0);
        printf("Menunggu konfirmasi %d\n", current_client_id);
        msgrcv(msgid,&msg,sizeof(msg),current_client_id,0);
        printf("Kontainer berhasil dibuat\nApakah anda ingin membuat kontainer lagi? (1/0) : ");
        scanf("%d",&msg.repeat);
        msg.message_type=6;
        msgsnd(msgid,&msg,sizeof(msg),0);
        if (msg.repeat == 0) {
            break;
        }
    }
    
    return 0;
}
