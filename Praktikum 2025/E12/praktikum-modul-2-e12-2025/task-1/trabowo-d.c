#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char *zipFi = "filmdone.zip";
    char *bruh[] = {"zip", "-r", zipFi, "film", NULL};  

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        execvp("zip", bruh);
        perror("gagal");
        return 1;
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Zip DONE: %s\n", zipFi);
        } else {
            printf("GAGAL\n");
        }
    }

    return 0;
}
