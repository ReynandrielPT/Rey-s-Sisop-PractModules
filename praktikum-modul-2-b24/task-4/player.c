#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>


#define QUEUE_KEY 1234
#define MSG_SIZE 200

typedef struct {
    long message_type;  
    char message_text[MSG_SIZE];
} Message;
char board[10];
void format_board() {
    printf("\n %c | %c | %c \n---+---+---\n %c | %c | %c \n---+---+---\n %c | %c | %c \n",
            board[1], board[2], board[3],
            board[4], board[5], board[6],
            board[7], board[8], board[9]);
}

int main() {
    int msgid;
    Message msg;
    char choice;
    int temp;
    int namber;
    int first=0;
    int a = 9;
    while(a>0){
        board[a]=' ';
        a--;
    }
    char *temp2 = malloc(256);
    msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("Error accessing message queue");
        exit(1);
    }
    
    while (1) {
        printf("Available player type:\n1. X\n2. O\nEnter your choice (1 or 2): ");
        scanf(" %c", &choice);

        if (choice != '1' && choice != '2') {
            printf("Invalid choice. Please enter 1 or 2.\n");
            continue;
        }

        
        sprintf(msg.message_text, "role %c", (choice == '1' ? 'X' : 'O'));
        msg.message_type = 1;
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msgrcv(msgid, &msg, sizeof(msg), (choice == '1' ? 2 : 3), 0);
        if ( strcmp(msg.message_text, "True") == 0) {
            printf("\nWaiting for another player to join the game...\n\n");
            first=1;
            break;
        } if ( strcmp(msg.message_text, "True1") == 0) {
            break;
        }else {
            printf("'%c' has already been taken, choose other player types\n\n", choice == '1' ? 'X' : 'O');
            continue;
        }
    }

    while (1) {
        msgrcv(msgid, &msg, sizeof(msg), msg.message_type, 0);
        if(strcmp (msg.message_text, "False") == 0){
            continue;
        }
        if(strcmp(msg.message_text, "You Win") == 0 || strcmp(msg.message_text, "You Lose") == 0  || strcmp(msg.message_text, "Draw") == 0 ){
            sprintf(temp2, "%s", msg.message_text);
            if(strcmp(msg.message_text, "You Lose") == 0 || strcmp(msg.message_text, "Draw") == 0 ){
                msgrcv(msgid, &msg, sizeof(msg), msg.message_type, 0);
                int pos = msg.message_text[0]-'0';
                board[pos] = (msg.message_type == 2 ? 'O' : 'X');
            }
            printf("\n--- Last Board ---\n");
            format_board();
            printf("\n---Game Finished!---\n" );
            sprintf(msg.message_text, "%s", temp2);
            printf("%s\n", msg.message_text);
            break;}
        else if(strcmp(msg.message_text, "Pe")!=0){
                int pos = msg.message_text[0]-'0';
                board[pos] = (msg.message_type == 2 ? 'O' : 'X');
        }if(strcmp(msg.message_text, "Pe")==0){
            printf("\n----Game Started!----\nYou play as '%c'\n\n 1 | 2 | 3\n--- --- ---\n 4 | 5 | 6\n--- --- ---\n 7 | 8 | 9\n\nSelect the corresponding cell number to choose a cell.\n ", msg.message_type == 2 ? 'X' : 'O');
            format_board();
            if(first==0){
                printf("\nWaiting for the other player to make the first move...\n");
                continue;
            }
        }else{ 
            printf("\nPlayer %c's turn", msg.message_type == 2 ? 'X' : 'O');
            format_board();
        }
        int f = 0;
        printf("\nYour turn\n");
        while(!f){
            printf("Enter a cell number: ");
            scanf("%d", &namber);
            if(namber>9 || namber<1){
                printf("Wrong input, choose cell number 1-9\n");
                continue;
            }
            if(board[namber] == 'X'){
                printf("Choose other cell, %c is filled by 'X'\n", msg.message_text[0]);
            }
            else if(board[namber] == 'O'){
                printf("Choose other cell, %c is filled by 'O'\n", msg.message_text[0]);
            }else{
                f=1;
                board[namber] = (msg.message_type == 2 ? 'X' : 'O');
                sprintf(msg.message_text, "%d", namber);
            }
        }
        temp = msg.message_type;
        msg.message_type=1;
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msg.message_type = temp;
    }

    
    
    return 0;
}