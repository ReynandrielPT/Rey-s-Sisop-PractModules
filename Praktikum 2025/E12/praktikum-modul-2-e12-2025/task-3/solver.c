#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>
#include <cjson/cJSON.h>
#include <pthread.h>
typedef struct {
    char *short_name;
    int release_month;
    char *manhwa;
    char *url;
} Heroine;
void file_name(char *str){
    int k=0;
    for(int i = 0;i < strlen(str);i++){
        if (str[i] == ' ') {
            str[k++] = '_';
        }
        else if((str[i] >= 'A' && str[i] <= 'Z') ||
                (str[i] >= 'a' && str[i] <= 'z') ||
                (str[i] >= '0' && str[i] <= '9')
                ) {
            str[k++] = str[i];
        }
        }
        str[k] = '\0';
    }


size_t write_data(void *buffer, size_t size, size_t nmemb, char *data){
    size_t realsize = size * nmemb;
    strncat(data, buffer, realsize);
    return realsize;
}
void create_directory(char *name){

    pid_t pid = fork();
    if(pid == 0){
        char *args[] = {"mkdir","-p",name,NULL};
        execvp("mkdir",args);
    }
    else if(pid > 0){
        waitpid(pid,NULL,0);
    }
}

/*
Format : 
Title: The Villain's Daughter-in-Law
Status: Publishing
Release: 2024-10-16
Genre: Fantasy, Romance
Theme: Time Travel
Author: Na, Reuyan, Kim, Dael
*/
void parse_json(char *id, int *release_month){
    CURL *curl;
    CURLcode res;
    char url[100];
    char data[10000] = {0};
    snprintf(url, sizeof(url),"https://api.jikan.moe/v4/manga/%s", id);
    curl = curl_easy_init();
    if(curl){

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    
    // fetch data
    cJSON *root = cJSON_Parse(data);
    if(!root) return;
    cJSON *data_obj = cJSON_GetObjectItemCaseSensitive(root, "data");
    if(!data_obj) return;
    cJSON *title = cJSON_GetObjectItemCaseSensitive(data_obj, "title");
    cJSON *status = cJSON_GetObjectItemCaseSensitive(data_obj, "status");
    cJSON *published = cJSON_GetObjectItemCaseSensitive(data_obj, "published");
    cJSON *from = cJSON_GetObjectItemCaseSensitive(published, "from");
    cJSON *genre = cJSON_GetObjectItemCaseSensitive(data_obj, "genres");
    cJSON *theme = cJSON_GetObjectItemCaseSensitive(data_obj, "themes");
    cJSON *author = cJSON_GetObjectItemCaseSensitive(data_obj, "authors");
    
    sscanf(from->valuestring, "%*d-%d", release_month);
    char clean_title[100] = {0};
    strncpy(clean_title, title->valuestring,sizeof(clean_title));
    clean_title[sizeof(clean_title)-1] = '\0';
    file_name(clean_title);
    char file_path[200] = {0};
    snprintf(file_path,sizeof(file_path), "Manhwa/%s.txt", clean_title);
    FILE *file = fopen(file_path,"w");
    if(file){
        fprintf(file, "Title: %s\n", title->valuestring);
        fprintf(file, "Status: %s\n", status->valuestring);
        char date[11];
        strncpy(date,from->valuestring,10);
        date[10] = '\0';
        fprintf(file, "Release: %s\n", date);
        fprintf(file, "Genre: ");
        for(int i = 0;i < cJSON_GetArraySize(genre);i++){
            cJSON *genre_item = cJSON_GetArrayItem(genre, i);
            cJSON *genre_name = cJSON_GetObjectItemCaseSensitive(genre_item, "name");
            fprintf(file, "%s", genre_name->valuestring);
            if(i != cJSON_GetArraySize(genre)-1){
                fprintf(file, ", ");
            }
        }
        }
        fprintf(file, "\nTheme: ");
        for(int i = 0;i < cJSON_GetArraySize(theme);i++){
            cJSON *temp = cJSON_GetArrayItem(theme, i);
            cJSON *theme_name = cJSON_GetObjectItemCaseSensitive(temp, "name");
            fprintf(file, "%s", theme_name->valuestring);
            if(i != cJSON_GetArraySize(theme)-1){
                fprintf(file, ", ");
            }
        }
        fprintf(file, "\nAuthor: ");
        for(int i = 0;i < cJSON_GetArraySize(author);i++){
            cJSON *temp = cJSON_GetArrayItem(author, i);
            cJSON *author_name = cJSON_GetObjectItemCaseSensitive(temp, "name");
            fprintf(file, "%s", author_name->valuestring);
            if(i != cJSON_GetArraySize(author)-1){
                fprintf(file, ", ");
            }
        }
        fclose(file);
        cJSON_Delete(root);
    }

void uppercase(const char *str, char *out) {
    int k = 0;
    for (int i = 0; i < strlen(str); i++) {
        if(str[i] >= 'A' && str[i] <= 'Z'){
            out[k++] = str[i];
        }
    }
    out[k] = '\0';
}

void create_zip(const char *file_name) {
    char uc_file_name[100] = {0};
    uppercase(file_name, uc_file_name);
    char destination[200] = {0};
    char source[200] = {0};
    snprintf(destination, sizeof(destination), "Archive/%s.zip", uc_file_name);
    snprintf(source, sizeof(source), "Manhwa/%s.txt", file_name);
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"zip", "-j", destination, source, NULL};
        execvp("zip", args);
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);
    }
}
void get_image(const char *url, const char *file_name) {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    FILE *file = fopen(file_name, "wb");
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(file);
    } 

}
pthread_mutex_t lock;
void *download_image(void *args){
    pthread_mutex_lock(&lock);
    Heroine *data = (Heroine *)args;
    char *url = data->url;
    char folder_name[200] = {0};
    snprintf(folder_name, sizeof(folder_name), "Heroines/%s", data->short_name);
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"mkdir", "-p", folder_name, NULL};
        execvp("mkdir", args);
        printf("Berhasil create folder %s\n",folder_name);
        exit(1);
    }
    else if(pid > 0){
        waitpid(pid, NULL, 0);
        for(int i = 0;i < data->release_month;i++){
            char file_name[200] = {0};
            snprintf(file_name, sizeof(file_name), "Heroines/%s/%s_%d.jpg", data->short_name, data->short_name, i+1);
            get_image(url, file_name);
            printf("Berhasil mendownload gambar %s\n",file_name);
        }
    }
    pthread_mutex_unlock(&lock);
}
void heroines(Heroine *data){
    pthread_mutex_init(&lock, NULL);
    create_directory("Heroines");
    pthread_t threads[4];
    for(int i = 0;i < 4;i++){
        pthread_create(&threads[i], NULL, download_image, &data[i]);
    }
    for(int i = 0;i < 4;i++){
        pthread_join(threads[i], NULL);
    }
}
/*
Mistaken as the Monster Duke's Wife - Lia Dellis - https://i.pinimg.com/736x/45/f0/19/45f019ea8664f6f7b5cfbb4c893e46a0.jpg
The Villainess Lives Again - Artezia Rosan - https://i.pinimg.com/736x/28/4e/98/284e982667609520530e40bc4e13e3a9.jpg
No, I Only Charmed the Princess! - Adelia Vita - https://i.pinimg.com/736x/21/21/62/212162a0e1264d20b749715dee60fda7.jpg
Darling, Why Can't We Divorce? - Ophelia Lizen - https://i.pinimg.com/736x/1c/06/fb/1c06fba73c6f21c1b4f48b5f70f74813.jpg
*/
void heroines_zip(Heroine *data, int n){
    for(int i = 0; i < n; i++){
    char uc_file_name[100] = {0};
    uppercase(data[i].manhwa, uc_file_name);
    char destination[200] = {0};
    char source[200] = {0};
    snprintf(destination, sizeof(destination), "Archive/Images/%s_%s.zip", uc_file_name, data[i].short_name);
    snprintf(source, sizeof(source), "Heroines/%s", data[i].short_name);
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"zip", "-r", destination, source, NULL};
        execvp("zip", args);
        exit(1); 
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);
    }
    }
}

void delete_heroines(Heroine *data,int n){
    for(int i = 0;i < n-1;i++){
        for(int j = i+1;j<n;j++){
            if(strcmp(data[i].short_name,data[j].short_name)>0){
                Heroine temp = data[i];
                data[i] = data[j];
                data[j]= temp;
            }
        }
    }
    for(int i = 0;i<n;i++){

        for (int j = 0;j < data[i].release_month;j++){
            char file[200] = {0};
            snprintf(file,sizeof(file),"Heroines/%s/%s_%d.jpg",data[i].short_name,data[i].short_name,j+1);
            pid_t pid = fork();
            if(pid == 0){
                char *args[] = {"rm" ,"-f",file,NULL};
                printf("%s terhapus\n",file);
                execvp("rm",args);
                exit(1);
            }
            else if(pid>0){
                waitpid(pid,NULL,0);
            }
    
        }
    }
}

int main(){
    /*
    168827/Mistaken_as_the_Monster_Dukes_Wife
    147205/The_Villainess_Lives_Again
    169731/No_I_Only_Charmed_the_Princess
    175521/Darling_Why_Cant_We_Divorce
    */
   Heroine data[4]={
       {"Lia",0,"Mistaken as the Monster Duke's Wife","https://i.pinimg.com/736x/45/f0/19/45f019ea8664f6f7b5cfbb4c893e46a0.jpg"},
       {"Artezia",0,"The Villainess Lives Again","https://i.pinimg.com/736x/28/4e/98/284e982667609520530e40bc4e13e3a9.jpg"},
       {"Adelia",0,"No, I Only Charmed the Princess!","https://i.pinimg.com/736x/21/21/62/212162a0e1264d20b749715dee60fda7.jpg"},
       {"Ophelia",0,"Darling, Why Can't We Divorce?","https://i.pinimg.com/736x/1c/06/fb/1c06fba73c6f21c1b4f48b5f70f74813.jpg"}
   };
    create_directory("Manhwa");
    parse_json("168827",&data[0].release_month);
    parse_json("147205",&data[1].release_month);
    parse_json("169731",&data[2].release_month);
    parse_json("175521",&data[3].release_month);
    create_directory("Archive");
    create_zip("Mistaken_as_the_Monster_Dukes_Wife");
    create_zip("The_Villainess_Lives_Again");
    create_zip("No_I_Only_Charmed_the_Princess");
    create_zip("Darling_Why_Cant_We_Divorce");
    heroines(data);
    create_directory("Archive/Images");
    heroines_zip(data,4);
    delete_heroines(data,4);



    return 0;
}
