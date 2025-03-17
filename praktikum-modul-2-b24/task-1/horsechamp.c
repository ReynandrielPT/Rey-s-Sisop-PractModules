#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <wait.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

typedef struct {
  char name[256];
  char pemilik[256];
  int angka_cek;
} Horse;

Horse horse_leaderboard[512];
int horse_pos = 0;
int horse_number_race = 1;

void horseFetcher() {
  pid_t child;
  int status;

  child = fork();

  if (child < 0) {
    exit(EXIT_FAILURE);
  }

  if (child == 0) {
    execlp("curl", "curl", "-L", "-s", "https://docs.google.com/uc?export=download&id=1gJkoTbiHZcJ1M36vqZPTrj3qTQkRMElQ", "-o", "horse.zip", NULL);
    exit(EXIT_SUCCESS);
  }
  while(wait(&status) > 0);
  execlp("unzip", "unzip", "-o", "-q", "horse.zip", NULL);
  exit(EXIT_SUCCESS);
}

void *HorseRun(void *args) {
  Horse *current_horse = (Horse *)args;
  int rng = (rand() % 4000) + 1;
  current_horse->angka_cek = rng;
  for (int i = 2; i * i < rng; i++) {
    if (rng % i == 0) {
      break;
    }
  }
  horse_leaderboard[horse_pos++] = *current_horse;
  return NULL;
}

void HorseRaceHooray() {
  char file_name[512];
  fprintf(stdout, "Which horse file?\n");
  fscanf(stdin, "%s", file_name);
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    fprintf(stderr, "File not found!\n");
    exit(EXIT_FAILURE);
  }

  char horse_file_name[512] = "horse_race_1.txt";
  FILE *horse_file = fopen(horse_file_name, "r");
  while (horse_file) {
    horse_number_race++;
    strcpy(horse_file_name, "horse_race_");
    char number_as_string[32];
    sprintf(number_as_string, "%d", horse_number_race);
    strcat(horse_file_name, number_as_string);
    strcat(horse_file_name, ".txt");
    fclose(horse_file);
    horse_file = fopen(horse_file_name, "r");
  }
  horse_file = fopen(horse_file_name, "w");

  fprintf(horse_file, "----Horse Race %d------\n", horse_number_race);
  time_t curr_time = time(NULL);
  struct tm tm = *localtime(&curr_time);
  fprintf(horse_file, "Date : %02d/%02d/%04d\n", 
    tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
  fprintf(horse_file, "Time : %02d/%02d/%02d\n\n", 
    tm.tm_hour, tm.tm_min, tm.tm_sec);

  fclose(horse_file);

  pthread_t t_id[512];
  Horse horses[512];
  size_t t_idx = 0;

  char current_line[512];
  while (fgets(current_line, sizeof(current_line), file)) {
    current_line[strlen(current_line) - 2] = '\0';
    int split_index = 0;
    for (; split_index < strlen(current_line); split_index++) {
      if (current_line[split_index] == ':') {
        break;
      }
    }
    strncpy(horses[t_idx].name, current_line, split_index);
    strncpy(horses[t_idx].pemilik, current_line + split_index + 1, strlen(current_line) - split_index);

    pthread_create(&t_id[t_idx], NULL, &HorseRun, &horses[t_idx]);
    
    t_idx++;

  }
  fclose(file);

  for (size_t i = 0; i < t_idx; i++) {
    pthread_join(t_id[i], NULL);
  }
  horse_file = fopen(horse_file_name, "a");
  for (int i = 0; i < horse_pos; i++) {
    fprintf(horse_file, "%d. %s %s %d\n", i+1, horse_leaderboard[i].name, horse_leaderboard[i].pemilik, horse_leaderboard[i].angka_cek);
  }
  fclose(horse_file);
}

void HorseChampionLeaderboard() {
  fprintf(stdout, "Which race?\n");
  int i = 1;
  char horse_file_name[512] = "horse_race_1.txt";
  FILE *horse_file = fopen(horse_file_name, "r");
  while (horse_file) {
    fprintf(stdout, "%d. %s\n", i++, horse_file_name);
    strcpy(horse_file_name, "horse_race_");
    char number_as_string[32];
    sprintf(number_as_string, "%d", i);
    strcat(horse_file_name, number_as_string);
    strcat(horse_file_name, ".txt");
    fclose(horse_file);
    horse_file = fopen(horse_file_name, "r");
  }
  if (i == 1) {
    fprintf(stderr, "No horse race file found!\n");
    exit(EXIT_FAILURE);
  }
  int query;
  fscanf(stdin, "%d", &query);
  if (query <= 0 || query > i) {
    fprintf(stdout, "Invalid query!\n");
  }
  strcpy(horse_file_name, "horse_race_");
  char number_as_string[32];
  sprintf(number_as_string, "%d", query);
  strcat(horse_file_name, number_as_string);
  strcat(horse_file_name, ".txt");
  horse_file = fopen(horse_file_name, "r");

  if (horse_file == NULL) {
    fprintf(stderr, "File not found!\n");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "----HORSE CHAMPIONSHIP %d----\n", horse_number_race);
  char current_line[512];
  fgets(current_line, sizeof(current_line), horse_file);
  while (fgets(current_line, sizeof(current_line), horse_file)) {
    fprintf(stdout, "%s", current_line);
  }

  fclose(horse_file);
}

int main(int argc, char * argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Please specify argument!\n");
    exit(EXIT_FAILURE);
  }
  switch (argv[1][0]) {
    case '1':
      horseFetcher();
      break;
    case '2':
      HorseRaceHooray();
      break;
    case '3':
      HorseChampionLeaderboard();
      break;
    default:
      fprintf(stderr, "Invalid argument!\n");
      exit(EXIT_FAILURE);
  }
  exit(EXIT_SUCCESS);
}
