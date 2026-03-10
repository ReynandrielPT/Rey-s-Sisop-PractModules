#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>

int main(){
    DIR * dir;
    char *name[101];
    struct dirent *entry;
    dir = opendir("/home/freddskii/film");
    
    int i = 0;
    while((entry = readdir(dir)) != NULL && i < 101 ){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strstr(entry->d_name, ".txt") != NULL) continue;
        name[i] = strdup(entry->d_name); i++;
    }
    
    char path[] = "/home/freddskii/film";
    for(int k = 0; k < i; k++){
        pid_t pid = fork();
    
        if(pid == 0){
            char zipPath[512];
            snprintf(zipPath, sizeof(zipPath), "%s/%s.zip", path, name[k]);
    
            char *args[] = {"zip", "-r", zipPath, name[k], NULL};
            chdir(path);
            execvp("zip", args);
        }else{
            int status;
            waitpid(pid, &status, 0);
            if(WIFEXITED(status) && WEXITSTATUS(status) == 0){
                pid_t rm_pid = fork();
                if(rm_pid == 0){
                    chdir(path);
                    char *rm_args[] = { "rm", "-r", name[k], NULL };
                    execvp("rm", rm_args);
                } 
            }
        }
    }
    closedir(dir);
    return 0;
}
