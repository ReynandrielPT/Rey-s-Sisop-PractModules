#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>

void solve(char* dirBruh) {
    DIR* dir = opendir(dirBruh);
    struct dirent* bruh;
    char* files[1000];
    int JumFil = 0;
    while ((bruh = readdir(dir)) != NULL) {
        if (strcmp(bruh->d_name, ".") != 0 ) {
            files[JumFil++] = bruh->d_name;  
        }
    }
    closedir(dir);
    if (JumFil > 0) {
        srand(time(NULL));
        int ran = rand() % JumFil;  
        printf("Film for Trabowo & Peddy: ‘<%s>’\n", files[ran]);
    } else {
        printf("lah kok ga ada?\n");
    }
}

int main() {
    solve("film"); 
    return 0;
}
