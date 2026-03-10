#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

typedef struct {
  char string[64];
  int amount;
} Category;

Category rekap[128];
int rekap_size;

void category_add(Category * category, char * string) {
  // printf("%d ", rekap_size);
  for (int i = 0; i < rekap_size; i++) {
    if (strcmp(category[i].string, string) == 0) {
      category[i].amount++;
      // printf("add existing %s\n", string);
      return;
    }
  }
  // printf("add new %s\n", string);
  strcpy(category[rekap_size].string, string);
  category[rekap_size].amount = 1;
  rekap_size++;
}

void zip_fetch() {
  pid_t child_id;
  int status;

  child_id = fork();
  if (child_id == 0) {
    execlp("curl", "curl" ,"-L", "-s", "https://dl.dropboxusercontent.com/scl/fi/gmy0qvbysh3h7vdexso9k/task_sisop.zip?rlkey=ism9qfjh82awg2tbtzfbrylg4&dl=0", "-o", "task_sisop.zip", NULL);
    exit(EXIT_SUCCESS);
  }
  while (wait(&status) > 0);
  FILE *zip = popen("unzip -l task_sisop.zip", "r");
  char current_line[1024];
  while (fgets(current_line, sizeof(current_line), zip)) {
    current_line[strlen(current_line) - 1] = '\0';
    char path[256];
    if (strlen(current_line) <= 30) {
      continue;
    }
    strcpy(path, current_line + 30);
    path[strlen(path)] = '\0';
    if (strstr(path, "task_") != NULL) {
      if (strstr(path, ".txt") != NULL) {
        pid_t pid;
        int unzipping;

        pid = fork();
        if (pid == 0) {
          execlp("unzip", "unzip", "-o", "-q", "-j", "task_sisop.zip", path, "-d", "task/", NULL);
          exit(EXIT_SUCCESS);
        }
        pid_t wpid;
        while ((wpid = wait(&unzipping)) > 0) {
          if (wpid == pid) {
            break;
          }
        }
      }
    }
  }

  fclose(zip);
  remove("task_sisop.zip");
  exit(EXIT_SUCCESS);
}

int compare(const void *a, const void *b) {
  char *fi = (*(char **)a);
  char *se = (*(char **)b);
  int x = 0, y = 0;
  bool task_num = false;
  for (int i = 0; i < strlen(fi); i++) {
    if (fi[i] == '_') {
      task_num = !task_num;
      continue;
    }
    if (task_num) {
      x = x * 10 + (fi[i] - '0');
    }
    if (!task_num && x != 0) break;
  }

  for (int i = 0; i < strlen(se); i++) {
    if (se[i] == '_') {
      task_num = !task_num;
      continue;
    }
    if (task_num) {
      y = y * 10 + (se[i] - '0');
    }
    if (!task_num && y != 0) break;
  }
  if (x < y) return -1;
  if (x == y) return 0;
  return 1;
}

void *bubu_work(void *args) {
  char *task_name = (char *)args;
  char task_folder[128] = "task/Bubu/task";
  char task_num[32];
  for (int i = 5; i < strlen(task_name); i++) {
    if (task_name[i] == '_') break;
    sprintf(task_num + strlen(task_num), "%c", task_name[i]);
  }
  sprintf(task_folder + strlen(task_folder), "%s/", task_num);
  mkdir(task_folder, 0777);

  char task_path[128];
  strcpy(task_path, "task/");
  strcat(task_path, task_name);
  char new_task_path[128];
  strcpy(new_task_path, task_folder);
  strcat(new_task_path, task_name);
  rename(task_path, new_task_path);

  FILE *task_file = fopen(new_task_path, "r");
  char command[128];
  fgets(command, sizeof(command), task_file);
  int quantity;
  char category[64], resolution[64];
  int idx = 0;

  for (; idx < strlen(command); idx++) {
    if (command[idx] == ' ') break;
    quantity = quantity * 10 + (command[idx] - '0');
  }

  idx++;

  for (; idx < strlen(command); idx++) {
    if (command[idx] == ' ') break;
    sprintf(resolution + strlen(resolution), "%c", command[idx]);
  }

  idx++;

  for (; idx < strlen(command); idx++) {
    sprintf(category + strlen(category), "%c", command[idx]);
  }

  if (category[strlen(category) - 1] < 'a' || category[strlen(category) - 1] > 'z'){
    category[strlen(category) - 1] = '\0';
  }

  pid_t pid;
  for (int i = 1; i <= quantity; i++) {
    pid = fork();
    int status;
    if (pid == 0){
      char url[256];
      sprintf(url + strlen(url), "https://source.unsplash.com/random/%s/?%s", resolution, category);
      char file_path[256];
      sprintf(file_path + strlen(file_path), "%simage%d.jpg", task_folder, i);
      execlp("curl", "curl", "-L", "-s", url, "-o", file_path, NULL);
      exit(EXIT_SUCCESS);
    } else {
      category_add(rekap, category);
      pid_t wpid;
      while ((wpid = wait(&status)) > 0) {
        if (wpid == pid) {
          break;
        }
      }
    }
  }

  time_t curr_time = time(NULL);
  struct tm tm = *localtime(&curr_time);
  FILE *recap = fopen("recap.txt", "a");

  fprintf(recap, "[%04d-%02d-%02d %02d:%02d:%02d]-[Bubu] Task %s completed, download %d %s images with %s resolution\n", 
    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
    tm.tm_hour, tm.tm_min, tm.tm_sec, task_num, 
    quantity, category, resolution);

  fclose(recap);
  return NULL;
}

void *yuan_work(void *args) {
  char *task_name = (char *)args;
  char task_folder[128] = "task/Yuan/task";
  char task_num[32];
  for (int i = 5; i < strlen(task_name); i++) {
    if (task_name[i] == '_') break;
    sprintf(task_num + strlen(task_num), "%c", task_name[i]);
  }
  sprintf(task_folder + strlen(task_folder), "%s/", task_num);
  mkdir(task_folder, 0777);

  char task_path[128];
  strcpy(task_path, "task/");
  strcat(task_path, task_name);
  char new_task_path[128];
  strcpy(new_task_path, task_folder);
  strcat(new_task_path, task_name);
  rename(task_path, new_task_path);

  FILE *task_file = fopen(new_task_path, "r");
  char command[128];
  fgets(command, sizeof(command), task_file);
  int quantity;
  char category[64], resolution[64];
  int idx = 0;

  for (; idx < strlen(command); idx++) {
    if (command[idx] == ' ') break;
    quantity = quantity * 10 + (command[idx] - '0');
  }

  idx++;

  for (; idx < strlen(command); idx++) {
    if (command[idx] == ' ') break;
    sprintf(resolution + strlen(resolution), "%c", command[idx]);
  }

  idx++;

  for (; idx < strlen(command); idx++) {
    sprintf(category + strlen(category), "%c", command[idx]);
  }

  if (category[strlen(category) - 1] < 'a' || category[strlen(category) - 1] > 'z'){
    category[strlen(category) - 1] = '\0';
  }

  pid_t pid;
  for (int i = 1; i <= quantity; i++) {
    pid = fork();
    int status;
    if (pid == 0){
      char url[256];
      sprintf(url + strlen(url), "https://source.unsplash.com/random/%s/?%s", resolution, category);
      char file_path[256];
      sprintf(file_path + strlen(file_path), "%simage%d.jpg", task_folder, i);
      execlp("curl", "curl", "-L", "-s", url, "-o", file_path, NULL);
      exit(EXIT_SUCCESS);
    } else {
      category_add(rekap, category);
      pid_t wpid;
      while ((wpid = wait(&status)) > 0) {
        if (wpid == pid) {
          break;
        }
      }
    }
  }

  time_t curr_time = time(NULL);
  struct tm tm = *localtime(&curr_time);
  FILE *recap = fopen("recap.txt", "a");

  fprintf(recap, "[%04d-%02d-%02d %02d:%02d:%02d]-[Yuan] Task %s completed, download %d %s images with %s resolution\n", 
    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
    tm.tm_hour, tm.tm_min, tm.tm_sec, task_num, 
    quantity, category, resolution);

  fclose(recap);
  return NULL;
}

void divide_work() {
  char *entries[512];
  int idx = 0;

  DIR *task_dir = opendir("task/");
  struct dirent *entry;
  while (entry = readdir(task_dir)) {
    if (entry->d_name[0] != 't') continue;
    entries[idx++] = entry->d_name;
  }

  qsort(entries, idx, sizeof(char*), compare);

  int l = 0, r = idx - 1;

  mkdir("task/Bubu", 0777);
  mkdir("task/Yuan", 0777);

  pthread_t t_id[512];
  size_t t_idx = 0;
  while (l <= r) {
    pthread_create(&t_id[t_idx++], NULL, &yuan_work, (void*)entries[r--]);
    if (l > r) break;
    pthread_create(&t_id[t_idx++], NULL, &bubu_work, (void*)entries[l++]);
  }

  for (size_t i = 0; i < t_idx; i++) {
    pthread_join(t_id[i], NULL);
  }

  FILE *recap = fopen("recap.txt", "a");
  int total_images = 0;
  for (int i = 0; i < rekap_size; i++) {
    fprintf(recap, "%s: %d images\n", rekap[i].string, rekap[i].amount);
    total_images += rekap[i].amount;
  }
  fprintf(recap, "total images: %d images\n", total_images);
  fclose(recap);

  closedir(task_dir);

  printf("%d\n", rekap_size);
  return;
}

void zip_yuan_and_bubu() {
  chdir("task/");
  pid_t pid = fork();
  int status;
  if (pid == 0) {
    execlp("zip", "zip", "-r", "submission.zip", "Yuan/", "Bubu/", NULL);
    exit(EXIT_SUCCESS);
  }
  while (wait(&status) > 0);
}
int main(int argc, char *argv[]) {
  pid_t pid;
  int task_downloaded;
  pid = fork();
  if (pid == 0){
    zip_fetch();
    exit(EXIT_SUCCESS);
  }
  while (wait(&task_downloaded) > 0);
  printf("Hello\n");
  divide_work();
  zip_yuan_and_bubu();
}