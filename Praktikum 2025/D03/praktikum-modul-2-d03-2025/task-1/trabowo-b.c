#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

int extract_number(const char *filename) {
    int number;
    sscanf(filename, "%d", &number);
    return number;
}

int main(){
    DIR * dir;
    char *name[101];
    struct dirent *entry;
    dir = opendir("/home/freddskii/film");
    
    int i = 0;
    while((entry = readdir(dir)) != NULL && i < 101 ){
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        name[i] = strdup(entry->d_name); i++;
    }
    
    for(int j = 0 ; j < i ; j++){
        for(int k = 0 ; k < i-1 ; k++){
            if(extract_number(name[k]) > extract_number(name[k+1])){
                char *temp = name[k];
                name[k] = name[k+1];
                name[k+1] = temp;
            }
        }
    }
    
    srand(time(NULL));
    int ran = rand() % i;
    printf("Film for Trabowo & Peddy: %s\n", name[ran]);
    closedir(dir);

    return 0;
}
