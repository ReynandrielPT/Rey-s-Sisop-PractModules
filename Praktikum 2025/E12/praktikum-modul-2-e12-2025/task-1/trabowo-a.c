#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void bjer(const char* zipFi) {
    pid_t pid = fork();
    if (pid == 0) {
        char *bruh[] = {"unzip", "-o", (char *)zipFi, "-d", ".", NULL};
        execvp(bruh[0], bruh); 
    } 
    else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0); 
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0){
            printf("DONE\n");
        } else {
            printf("GAGAL\n");
        }
    } 
}

int main() {
     char* zipFi = "film.zip";  
    bjer(zipFi);
    return 0;
}
