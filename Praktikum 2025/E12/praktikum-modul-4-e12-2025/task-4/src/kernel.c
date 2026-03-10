#include "std_lib.h"
#include "kernel.h"

void echo(char* input) {
    printString(input);
}

void grep(char* pattern, char* input) {
    if (strcmp(pattern, input) == 0) {
        printString(input);
    }
}

void wc(char* input) {
    int lines = 0, words = 0, chars = 0;
    bool inWord = false;

    while (*input) {
        chars++;

        if (*input == '\n') {
            lines++;
        }

        if (*input == ' ' || *input == '\n' || *input == '\t') {
            if (inWord) {
                words++;
                inWord = false;
            }
        } else {
            inWord = true;
        }

        input++;
    }

    if (inWord) {
        words++;
    }

    printString("Lines: ");
    printString(itoa(lines));
    printString(", Words: ");
    printString(itoa(words));
    printString(", Characters: ");
    printString(itoa(chars));
}

int main() {
    char buf[128];

    clearScreen();
    printString("LilHabOS - E12\n");

    while (true) {
        printString("$> ");
        readString(buf);
        printString("\n");

        if (strlen(buf) > 0) {
            if (strncmp(buf, "echo ", 5) == 0) {
                echo(buf + 5);
            }
            else if (strncmp(buf, "echo ", 5) == 0 && strchr(buf, '|') != NULL) {
                char* arg1 = buf + 5;
                char* arg2 = strchr(buf, '|') + 2;
                grep(arg2, arg1);
            }
            else if (strncmp(buf, "echo ", 5) == 0 && strchr(buf, '|') != NULL && strstr(buf, "wc") != NULL) {
                char* arg1 = buf + 5;
                wc(arg1);
            }
        }
    }
}


    //                        ╱|、
    //                       (˚ˎ 。7  
    //                        |、˜〵          
    //                       じしˍ,)ノ
