#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define QUEUE_KEY 1234
#define BOARD_SIZE 10
#define MSG_SIZE 200

typedef struct {
    long message_type; 
    char message_text[MSG_SIZE];
} Message;

char board[BOARD_SIZE];
int roles[2] = {0, 0};  
void reset_game() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = '1' + i;
    }
    roles[0] = 0;
    roles[1] = 0;
    
}


int main() {
    int msgid;
    Message msg;
    msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    int step=0;
    int count=0;
    int num;
    char * a;
    int flag=0;
    int temp;
    reset_game();
    printf("Game Started\n");
    while (1) {
        msgrcv(msgid, &msg, sizeof(msg), 1, 0);
        if(msg.message_text[5] == 'X' &&  flag==0){
            step=1;
        }
        flag+=1;
        if (strncmp(msg.message_text, "role", 4) == 0) {
            int player_index = (msg.message_text[5] == 'X' ? 0 : 1);
            msg.message_type= (msg.message_text[5] == 'X' ? 2 : 3);
            if (roles[player_index] == 1) {
                strcpy(msg.message_text, "False");
                msgsnd(msgid, &msg, sizeof(msg), 0);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                continue;
            } else {
                roles[player_index] = 1;
                if(flag==1){
                    strcpy(msg.message_text, "True");
                }else{
                    strcpy(msg.message_text, "True1");
                }
                
            }
            msgsnd(msgid, &msg, sizeof(msg), 0);
            if(roles[0]==1 && roles[1]==1){
                msg.message_type=3;
                     strcpy(msg.message_text, "Pe");
                     msgsnd(msgid, &msg, sizeof(msg), 0);
                     msg.message_type=2;
                      strcpy(msg.message_text, "Pe");
                    msgsnd(msgid, &msg, sizeof(msg), 0);
                    continue;
             
                   }
            continue;
            }
            

        count++;
        if(count%2!=0){
                num=msg.message_text[0]-'0';
               board[num] = (step == 1 ? 'X' : 'O'); 
                 printf("New Message\nSender: player %c\nMessage: %c\n",
                   (step == 1 ? 'X' : 'O'), msg.message_text[0]);
            msg.message_type=(step == 1 ? 3 : 2);
            temp = msg.message_type;
            }
        else{
            num=msg.message_text[0]-'0';
             board[num] = (step == 0 ? 'X' : 'O');
             printf("New Message\nSender: player %c\nMessage: %c\n",
                   (step == 0 ? 'X' : 'O'), msg.message_text[0]);
            msg.message_type=(step != 1 ? 3 : 2);
            temp = msg.message_type;
             
        }
            if ((board[1]==board[2])&&((board[2]==board[3])) || (board[1]==board[4])&&((board[4]==board[7])) || (board[1]==board[5])&&((board[5]==board[9])) || (board[2]==board[5])&&((board[5]==board[8])) || (board[3]==board[6])&&((board[6]==board[9])) || (board[3]==board[5])&&((board[5]==board[7])) || (board[4]==board[5])&&((board[5]==board[6])) || (board[7]==board[8])&&((board[8]==board[9]))) {
                msg.message_type = (temp == 2 ? 3 : 2);
                sprintf(msg.message_text, "You Win");
                msgsnd(msgid, &msg, sizeof(msg), 0); 
                sprintf(msg.message_text, "You Lose");
                msg.message_type = (msg.message_type  == 2 ? 3 : 2);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sprintf(msg.message_text, "%d", num);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                reset_game();  
                printf("Game Finished\n");
                break;
            }else if(count==9){
                
                sprintf(msg.message_text, "Draw");
                msg.message_type = 3;
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sprintf(msg.message_text, "%d", num);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                msg.message_type = 2;
                sprintf(msg.message_text, "Draw");
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sprintf(msg.message_text, "%d", num);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                reset_game();  
                printf("Game Finished\n");
                break;
            }
        msgsnd(msgid, &msg, sizeof(msg), 0);
        
    }
    return 0;
}

