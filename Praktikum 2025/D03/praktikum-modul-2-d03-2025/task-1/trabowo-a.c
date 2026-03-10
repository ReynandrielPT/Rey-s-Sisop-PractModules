#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();

    if(pid == 0) {
        char *args[] = {"wget", "-O", "film.zip", "https://drive.google.com/uc?export=download&id=1nP5kjCi9ReDk5ILgnM7UCnrQwFH67Z9B", NULL};
        execv("/usr/bin/wget", args);
    }else{
        int status;
        wait(&status);
        if(WIFEXITED(status)) {
            char *argv[] = {"unzip", "-q", "film.zip", NULL};
            execv("/usr/bin/unzip", argv);
        }
    }

    return 0;
}
