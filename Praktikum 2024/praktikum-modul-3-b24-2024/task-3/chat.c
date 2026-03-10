#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int counter = 0; 

typedef struct {
    int index;
    char *word;
    FILE *file;
} ThreadArg;

void* write_word(void *arg) {
    ThreadArg *threadArg = (ThreadArg *)arg;
    pthread_mutex_lock(&mutex);
    while (threadArg->index != counter) {
        pthread_cond_wait(&cond, &mutex);
    }
    fprintf(threadArg->file, "%s ", threadArg->word);
    fflush(threadArg->file);
    counter++;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void write_message(char **words, int count, const char *container2) {
    if (strcmp(container2, getenv("NAME")) == 0) {
        printf("Pengirim dan penerima tidak bisa sama!\n");
        return;
    }
    pthread_t *threads = malloc((count + 2) * sizeof(pthread_t));
    ThreadArg *args = malloc((count + 2) * sizeof(ThreadArg));
    const char* container1 = getenv("NAME");
    char filename[256];

    snprintf(filename, sizeof(filename), "/app/data/connection/%s%s.txt", container2, container1);
    FILE *check_file = fopen(filename, "r");
    if (!check_file) {
        printf("Kedua kontainer tidak terkoneksi!\n");
        return;
    }
    fclose(check_file);

    snprintf(filename, sizeof(filename), "/app/data/%s/%s.txt", container2, container1);
    FILE *file = fopen(filename, "a");
    if(file){
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond, NULL);

        for (int i = 0; i < count; i++) {
            args[i].index = i;
            args[i].word = strdup(words[i]);
            args[i].file = file;
            pthread_create(&threads[i], NULL, write_word, &args[i]);
        }
        char *additionalWords[2] = {"from", container1};
        for (int i = count; i < count + 2; i++) {
            args[i].index = i;
            args[i].word = strdup(additionalWords[i - count]);
            args[i].file = file;
            pthread_create(&threads[i], NULL, write_word, &args[i]);
        }

        for (int i = 0; i < count+2; i++) {
            pthread_join(threads[i], NULL);
            free(args[i].word);
        }
        fprintf(file, "\n");

        fclose(file);
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&mutex);
    }
    free(threads);
    free(args);
    printf("Pesan telah berhasil terkirim\n");
}
    
void read_message(char filename[256]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("%s", filename);
        perror("Error opening file");
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }
    fclose(file);
}

int main(int argc, char *argv[]) {

    if ((strcmp(argv[1], "write") == 0)) {
        if(argc>3){
            write_message(&argv[2], argc - 3, argv[argc - 1]);
        }else{
            printf("Argument kurang!\n");
            return EXIT_FAILURE;
        }
        
    } else if(strcmp(argv[1], "read") == 0){
          printf("Pesan yang terkirim:\n");
           const char *container_name = getenv("NAME");
        char directory_path[1024];
        snprintf(directory_path, sizeof(directory_path), "/app/data/%s", container_name);
        DIR *dir = opendir(directory_path);
        if (dir == NULL) {
            perror("Failed to open directory");
            return EXIT_FAILURE;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            struct stat statbuf;
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", directory_path, entry->d_name);
            if (stat(filepath, &statbuf) == 0 && S_ISREG(statbuf.st_mode)) {
                const char *ext = strrchr(entry->d_name, '.');
                if (ext && strcmp(ext, ".txt") == 0) {
                    read_message(filepath);
                }
            }
        }

        closedir(dir);
    }else {
        fprintf(stderr, "Salah input.\n");
        return EXIT_FAILURE;
    }

    return 0;
}
