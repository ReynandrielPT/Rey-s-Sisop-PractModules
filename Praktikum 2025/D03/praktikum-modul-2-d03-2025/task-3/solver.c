#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

struct thread {
    char *folder;
    char *name;
    char *url;
    int jumlah;
};

struct memory {
    char *memory;
    size_t size;
};

struct heroine_info {
    char name[128];
    char filepath[256];
    int month;
    char kapital[64];
};

pthread_mutex_t download;

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int compare_heroine(const void *a, const void *b) {
    return strcmp(((struct heroine_info*)a)->name, ((struct heroine_info*)b)->name);
}

void *download_heroine(void *arg) {
    pthread_mutex_lock(&download);
    struct thread *data = (struct thread *)arg;
    for (int i = 1; i <= data->jumlah; i++) {
        char filename[256];
        snprintf(filename, sizeof(filename), "%s/%s_%d.jpg", data->folder, data->name, i);
        pid_t pid = fork();
        if (pid == 0) {
            char *args[] = {"curl", "-s", "-L", data->url, "-o", filename, NULL};
            printf("%s terdownload\n", filename);
            execv("/usr/bin/curl", args);
            exit(1);
        } else {
            wait(NULL);
        }
    }
    pthread_mutex_unlock(&download);
    return NULL;
}

char* fetch_json(char* url) {
    CURL *curl;
    CURLcode res;
    struct memory chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    return chunk.memory;
}

void format_txt(char *a, char *b) {
    int i, j = 0;
    for (i = 0; a[i] != '\0'; i++) {
        if ((a[i] >= 'a' && a[i] <= 'z') || (a[i] >= 'A' && a[i] <= 'Z') || (a[i] >= '0' && a[i] <= '9')) {
            b[j] = a[i];
            j++;
        } else if (a[i] == ' ') {
            b[j] = '_';
            j++;
        }
    }
    b[j] = '\0';
    strcat(b, ".txt");
}

void buat_direktori(char* folder) {
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"mkdir", "-p", folder, NULL};
        execv("/bin/mkdir", args);
        exit(1);
    } else {
        wait(NULL);
    }
}

void buat_zip(char* name, char* dest) {
    pid_t pid = fork();
    if (pid == 0) {
        char *args[] = {"zip", "-jq", name, dest, NULL};
        printf("%s terzip\n", dest);
        execv("/usr/bin/zip", args);
        exit(1);
    } else {
        wait(NULL);
    }
}

void task1(char *json) {
    cJSON *json_data = cJSON_Parse(json);
    cJSON *data = cJSON_GetObjectItem(json_data, "data");

    char *status = cJSON_GetObjectItem(data, "status")->valuestring;
    char *title = cJSON_GetObjectItem(data, "title_english")->valuestring;

    cJSON *published = cJSON_GetObjectItem(data, "published");
    int year = 0, month = 0, day = 0;
    if (published) {
        cJSON *prop = cJSON_GetObjectItem(published, "prop");
        cJSON *from = cJSON_GetObjectItem(prop, "from");
        year = cJSON_GetObjectItem(from, "year")->valueint;
        month = cJSON_GetObjectItem(from, "month")->valueint;
        day = cJSON_GetObjectItem(from, "day")->valueint;
    }

    char genres[512], authors[512], themes[512];
    genres[0] = '\0';
    authors[0] = '\0';
    themes[0] = '\0';
    cJSON *genres_tmp = cJSON_GetObjectItem(data, "genres");
    cJSON *authors_tmp = cJSON_GetObjectItem(data, "authors");
    cJSON *themes_tmp = cJSON_GetObjectItem(data, "themes");

    cJSON *item;
    cJSON_ArrayForEach(item, genres_tmp) {
        char *name = cJSON_GetObjectItem(item, "name")->valuestring;
        if (strlen(genres) > 0) {
            strcat(genres, ", ");
        }
        strcat(genres, name);
    }

    cJSON_ArrayForEach(item, authors_tmp) {
        char *name = cJSON_GetObjectItem(item, "name")->valuestring;
        if (strlen(authors)) strcat(authors, ", ");
        strcat(authors, name);
    }

    cJSON_ArrayForEach(item, themes_tmp) {
        char *name = cJSON_GetObjectItem(item, "name")->valuestring;
        if (strlen(themes)) strcat(themes, ", ");
        strcat(themes, name);
    }

    char dir[256] = "Manhwa/";
    char filename[128];
    buat_direktori("Manhwa");
    format_txt(title, filename);
    strcat(dir, filename);

    FILE *f = fopen(dir, "w");
    if (f) {
        fprintf(f, "Title: %s\n", title);
        fprintf(f, "Status: %s\n", status);
        fprintf(f, "Release: %d-%02d-%02d\n", year, month, day);
        fprintf(f, "Genres: %s\n", genres);
        fprintf(f, "Theme: %s\n", themes);
        fprintf(f, "Authors: %s\n", authors);
        fclose(f);
    }

    cJSON_Delete(json_data);
}

void task2(char *json) {
    cJSON *json_data = cJSON_Parse(json);
    cJSON *data = cJSON_GetObjectItem(json_data, "data");
    buat_direktori("Archive");

    char *title = cJSON_GetObjectItem(data, "title_english")->valuestring;
    char dir[256] = "Manhwa/";
    char filename[128];
    char zip[128] = "Archive/";
    int idx = 8;

    format_txt(title, filename);
    strcat(dir, filename);

    for (int i = 0; filename[i] != '\0'; i++) {
        if (filename[i] >= 'A' && filename[i] <= 'Z') {
            zip[idx] = filename[i];
            idx++;
        }
    }
    zip[idx] = '\0';
    strcat(zip, ".zip");

    buat_zip(zip, dir);

    cJSON_Delete(json_data);
}

void task3_all(char *json[], int jumlah_manhwa) {
    buat_direktori("Heroines");

    char *fmc[4] = {"Dellis", "Artizea", "Adelia", "Ophelia"};
    char *url[4] = {
        "https://cdn.anime-planet.com/characters/primary/lia-dellis-1-285x399.webp?t=1741126489",
        "https://static.wikia.nocookie.net/the-villainess-lives-twice/images/e/e1/ArtizeaRosan.jpg/revision/latest?cb=20210407162325",
        "https://i.pinimg.com/736x/96/bc/1c/96bc1c48cfa6ce0579495eca31ebf775.jpg",
        "https://cdn.anime-planet.com/characters/primary/ophelia-lizen-1-285x399.webp?t=1744234317"
    };

    pthread_t threads[jumlah_manhwa];

    for (int i = 0; i < jumlah_manhwa; i++) {
        cJSON *json_data = cJSON_Parse(json[i]);
        cJSON *data = cJSON_GetObjectItem(json_data, "data");

        int month = 1;
        cJSON *published = cJSON_GetObjectItem(data, "published");
        cJSON *prop = cJSON_GetObjectItem(published, "prop");
        cJSON *from = cJSON_GetObjectItem(prop, "from");
        month = cJSON_GetObjectItem(from, "month")->valueint;


        char folder[128];
        snprintf(folder, sizeof(folder), "Heroines/%s", fmc[i]);
        buat_direktori(folder);

        struct thread *data_thread = malloc(sizeof(struct thread));
        data_thread->folder = strdup(folder);
        data_thread->name = strdup(fmc[i]);
        data_thread->url = strdup(url[i]);
        data_thread->jumlah = month;

        pthread_create(&threads[i], NULL, download_heroine, data_thread);
        cJSON_Delete(json_data);
    }

    for (int i = 0; i < jumlah_manhwa; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&download);
}

void task4(char *json[], int jumlah_manhwa) {
    char* fmc[4] = {"Dellis", "Artizea", "Adelia", "Ophelia"};
    buat_direktori("Archive/Images");

    struct heroine_info heroines[4];

    for (int i = 0; i < jumlah_manhwa; i++) {
        cJSON *json_data = cJSON_Parse(json[i]);
        cJSON *data = cJSON_GetObjectItem(json_data, "data");

        char *title = cJSON_GetObjectItem(data, "title_english")->valuestring;

        char title_tmp[64];
        title_tmp[0] = '\0';
        for (int j = 0; title[j] != '\0'; j++) {
            if (title[j] >= 'A' && title[j] <= 'Z') {
                strncat(title_tmp, &title[j], 1);
            }
        }

        char folder[128];
        snprintf(folder, sizeof(folder), "Heroines/%s", fmc[i]);

        int month = 1;
        cJSON *published = cJSON_GetObjectItem(data, "published");
        cJSON *prop = cJSON_GetObjectItem(published, "prop");
        cJSON *from = cJSON_GetObjectItem(prop, "from");
        month = cJSON_GetObjectItem(from, "month")->valueint;

        strncpy(heroines[i].name, fmc[i], sizeof(heroines[i].name));
        strncpy(heroines[i].filepath, folder, sizeof(heroines[i].filepath));
        strncpy(heroines[i].kapital, title_tmp, sizeof(heroines[i].kapital));
        heroines[i].month = month;

        cJSON_Delete(json_data);
    }

    qsort(heroines, 4, sizeof(heroines[0]), compare_heroine);

    for (int i = 0; i < 4; i++) {
        char zipname[512];
        snprintf(zipname, sizeof(zipname), "Archive/Images/%s_%s.zip", heroines[i].kapital, heroines[i].name);

        for (int j = 1; j <= heroines[i].month; j++) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s/%s_%d.jpg", heroines[i].filepath, heroines[i].name, j);
            buat_zip(zipname, filepath);
        }
    }
    
    for (int i = 0; i < 4; i++) {
        for (int j = 1; j <= heroines[i].month; j++) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s/%s_%d.jpg", heroines[i].filepath, heroines[i].name, j);
            remove(filepath);
            printf("%s terhapus\n", filepath);
        }
    }
}


int main() {
    char* urls[4] = {
        "https://api.jikan.moe/v4/manga/168827",
        "https://api.jikan.moe/v4/manga/147205",
        "https://api.jikan.moe/v4/manga/169731",
        "https://api.jikan.moe/v4/manga/175521"
    };

    char* json_data[4];
    for (int i = 0; i < 4; i++) {
        json_data[i] = fetch_json(urls[i]);
        task1(json_data[i]);
        task2(json_data[i]);
    }

    task3_all(json_data, 4);
    task4(json_data, 4);

    return 0;
}
