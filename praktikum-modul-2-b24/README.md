[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/o3jj1gXA)
<div align=center>

|    NRP     |      Name                  |
| :--------: | :------------------------: |
| 5025221091 | Fernando                   |
| 5025221113 | Reynandriel Pramas Thandya |

# Praktikum Modul 1 _(Module 1 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - HORSECHAMP](/task-1/)

- [Task 2 - Tugas Akhir Yuan dan Bubu _(Yuan's and Bubu's Final Project)_](/task-2/)

- [Task 3 - Ramalan Cuaca Rama _(Rama's Weather Forecast)_](/task-3/)

- [Task 4 - Tic Tac Toe](/task-4/)

### Laporan Resmi Praktikum Modul 1 _(Module 1 Lab Work Report)_

#### - [Task 1 - HORSECHAMP](/task-1/) By Nando

##### Soal: 
Program Lomba Balapan Kuda HORSECHAMP
DJumanto adalah seorang pengembang hebat yang telah menciptakan banyak kode keren. Saat ini, ia ingin membuat sebuah proyek baru yang disebut "horsechamp", sebuah program untuk lomba balapan kuda bernama horsechamp.c. Program ini memiliki beberapa fungsi utama:

HorseFetcher: Berfungsi untuk mengambil data kuda dari sini dan menyimpannya di komputer.

HorseRaceHooray: Berfungsi untuk melaksanakan balapan kuda dengan menggunakan file yang berisi data kuda yang akan bertanding sebagai parameter.

HorseChampionLeaderboard: Berfungsi sebagai penampil pemenang dari pertandingan yang telah terjadi sebelumnya.

Main: Berfungsi sebagai terminal interaktif untuk pengguna dengan pilihan-pilihan berikut:

Fetch Horse Data (Untuk mengambil data kuda dari internet)

Start Horse Race (Untuk melaksanakan balapan kuda menggunakan data kuda yang ada dalam satu file)

Show Winners (Untuk menampilkan pemenang dari pertandingan kuda yang telah terjadi sebelumnya)

Tugas:

- a. Buatlah fungsi **HorseFetcher**, yang bertugas mengambil data kuda dari internet dan menyimpannya dalam file dengan nama horse_1.txt, horse_2.txt, horse_3.txt, dan seterusnya. Kuda dan pemilik kuda dipisahkan dengan “:”, sebagai contoh “Morioh:DJumanto”, DJumanto adalah pemilik dari kuda Morioh

- b. Buatlah fungsi **HorseRaceHooray**, yang bertugas melaksanakan balapan kuda. Parameter yang diguakan adalah salah satu file kuda yang telah kalian fetch sebelumnya. Gunakan thread untuk menjalankan perlombaan tersebut. Setiap kuda akan melakukan perhitungan bilangan prima dengan angka acak antara 1 sampai 4000. Kuda yang menyelesaikan perhitungan lebih awal akan menempati posisi lebih tinggi dari kuda lainnya. Setelah perlombaan selesai, simpan hasil lomba dalam format HorseRace\_(nomor lomba).txt.

  Contoh format isi file adalah sebagai berikut:

  ```
  ai: posisi kuda
  H: Nama Kuda
  O: Nama Pemilik Kuda
  X: Angka yang digunakan kuda untuk menebak apakah angka tersebut prima atau tidak
  ```

  Contoh hasil output:

  ```
  ----Horse Race (Lomba keberapa dilaksanakan)------
  Date: DD/MM/YYYY
  Time: HH/MM/SS

  ai. H O X
  ai+1. H O X
  ai+2. H O X
  etc..
  a+n. H O X
  ```

- c. Buatlah fungsi **HorseChampionLeaderboard**, yang bertugas menampilkan pemenang dari pertandingan-pertandingan sebelumnya. Berikan opsi kepada pengguna untuk memilih lomba mana yang ingin dilihat hasilnya. Contoh opsi:

  ```
  Choose your Horse Race history:

  1. Horse Race 1
  2. Horse Race 2
  3. Horse Race 3
  4. Horse Race 4
  etc..
  ```

  Hasil output memiliki format sebagai berikut:

  ```
  ai: urutan posisi
  H: nama kuda
  O: nama pemilik
  N: Angka yang digunakan untuk menebak
  ```

  Contoh output adalah seperti berikut:

  ```
  ----HORSE CHAMPIONSHIP (lomba keberapa dilaksanakan)----
  Date: DD/MM/YYYY
  Time: HH/MM/SS

  ai. H O X
  ai+1. H O X
  ai+2. H O X
  ai+3. H O X
  etc…
  ai+n. H O X
  ```
##### Penjelasan:
- Full Code:
```
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
```
Penjelasan sebelum subsoal:
Code:
```
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
```
Sebelum membuat fungsi-fungsi, diperlukan pendeklarasian dan pengambilan library untuk menyediakan seluruh bahan (resources) yang diperlukan sehingga program dijalankan.
```
#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <wait.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
```
Kode tersebut merupakan perintah untuk mengambil atau mengikutkan (include) seluruh library yang diperlukan program nantinya.
```
typedef struct {
  char name[256];
  char pemilik[256];
  int angka_cek;
} Horse;
```
Lalu dibuatlah suatu struktur data dengan nama Horse. Didalamnya terdapat elemen char dengan nama "name" dengan constraint 256, kemudian char pemilik dengan constraint yang sama, dan suatu integer dengan nama angka_cek. Kegunaannya akan dijelaskan di penjelasan selanjutnya.
```
Horse horse_leaderboard[512];
int horse_pos = 0;
int horse_number_race = 1;
```
Dideklarasikan pula 3 variabel, yakni suatu array horse_leaderboard dengan tipe data Horse dan dengan constraint 512 data, integer horse_pos dengan nilai awal 0, dan integer horse_number_race dengan nilai awal 1.

- a. Fungsi Horse Fetcher
Code:
```
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
```
Fungsi horseFetcher berfungsi untuk mmendownload file kuda dan mengekstrak file tersebut
```
void horseFetcher() {
```
Pertama deklarasi suatu fungsi berupa void dengan nama `horseFetcher()`
```
 pid_t child;
  int status;
```
Mendeklarasikan suatu pid (process id) dengan nama child untuk PID anak, dan juga deklarasi integer bernama status yang nantinya digunakan untuk menentukan status dari proses anak. 
```
  child = fork();

  if (child < 0) {
    exit(EXIT_FAILURE);
  }
```
Disini digunakan fungsi 'fork()' yang berfungsi untuk membuat suatu proses anak, disini value child merupakan hasil dari 'fork()'. Kemudian dibuat suatu pengondisian untuk menentukan apabila proses anak gagal terbentuk maka program akan keluar karena error. 
```
if (child == 0) {
    execlp("curl", "curl", "-L", "-s", "https://docs.google.com/uc?export=download&id=1gJkoTbiHZcJ1M36vqZPTrj3qTQkRMElQ", "-o", "horse.zip", NULL);
    exit(EXIT_SUCCESS);
  }
```
Lalu dibuat pengondisian apabila tidak error ( child bernilai 0 ) maka jalankan perintah untuk mendownload file 'horse.zip' dari link yang sudah disediakan. Untuk mendownload digunakan perintah 'excelp' untuk menjalankan perintah mendownload. Pendownloadan menggunakan perintah "curl", lalu berikan link file yang akan didownload dan dengan hasil output ('-o') diberi nama horse.zip. Apabila pendownloadan berhasil maka keluar dari proses anak secara sukses (EXIT_SUCCESS). 
```
while(wait(&status) > 0);
  execlp("unzip", "unzip", "-o", "-q", "horse.zip", NULL);
  exit(EXIT_SUCCESS);
}
```
Setelah pendownloadan selesai, maka file yang telah di download harus di unzip. Untuk itu dibuat lah suatu while loop untuk menunggu child process download, dengan parameter ketika hasil dari fungsi wait (menunggu hingga) nilai dari status lebih dari 0 maka jalankan proses unzip. Proses unzip dapat dijalankan dengan menggunakan "execlp" dan menuliskan perintah untuk mengunzip yaitu "unzip" dengan format "("unzip", "unzip", "-o", "-q", "horse.zip", NULL);" yang artinya akan mengunzip file dengan nama "horse.zip". Argument "NULL" berfungsi untuk memberi tahu argumen terakhir kepada 'execlp'. '-q' mencegah untuk terjadinya keluaran yang tidak diinginkan, dan '-o' berarti file akan menimpa file yang sama apabila di direktori tersebut sudah ada file yang serupa sebelumnya. Kode diakhiri dengan fungsi exit(EXIT_SUCCESS) untuk keluar dari fungsi apabila proses berhasil.

- Fungsi HorseRun
Code:
```
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
```
HorseRun menggunakan argumen Horse yang diberikan ke fungsinya, menggenerate angka random asal dari 1 sampai 4000, dan cek jika angka tersebut prima.
```
void *HorseRun(void *args) {
```
Bagian tersebut berfungsi untuk mendeklarasikan suatu fungsi void bernama HorseRun 

-b. Fungsi Horse Race Hooray
```
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
```
Sesuai permintaan soal, fungsi HorseRaceHooray dibuat untuk menjalankan perlombaan secara keseluruhan. Setiap kuda yang berlomba akan melakukan perhitungan prima dengan angka acak yakni antara 1 hingga 4000. Kuda yang menyelesaikan perhitungan lebih dahulu akan menempati posisi diatas kuda lainnya. Kemudian fungsi akan menyimpan hasil perlombaan dalam bentuk file txt dengan format nama HorseRace_(nomor lomba).
```
void HorseRaceHooray() {
```
Pertama-tama deklarasikan suatu fungsi voide dengan nama HorseRaceHooray.
```
 char file_name[512];
  fprintf(stdout, "Which horse file?\n");
  fscanf(stdin, "%s", file_name);
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    fprintf(stderr, "File not found!\n");
    exit(EXIT_FAILURE);
  }
```
Bagian tersebut adalah bagian untuk menentukan file mana yang akan dibuka dan dijadikan dasar dari fungsi. Pertama deklarasikan suatu string (array of chars) dengan nama file_name dengan ukuran 512. Lalu fungsi akan meminta user untuk memasukkan nama file yang akan dipakai, inputan user dimasukkan kedalam char file_name. Kemudian file dicoba dibuka ```FILE *file = fopen(file_name, "r");```, lalu dibuatlah suatu pengondisian untuk mengetahui apakah file dapat dibuka atau tidak dengan ``` if (file == NULL)``` (jika pembukaan file bernilai NULL) maka keluarkan "File not found!" dan exit proses dengan EXIT_FAILURE.
```
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
```
Setelah file dapat dibuka, fungsi harus membuat suatu file dengan format nama horse_race_t.txt dengan t adalah nomor ke berapa file tersebut dibuat. Untuk mengecek urutan file keberapa yang sudah ada sebelumnya, fungsi perlu untuk membuka file dari horse_race_t.txt terakhir. Diawali dengan mendeklarasi string untuk format nama dari file, dengan nama horse_file_name dan dengan nilai awal yakni horse_race_1.txt. Kemudian fungsi akan membuka file dengan nama dari string horse_file_name. Karena diinginkan untuk membuka file yang terakhir maka dibuatlah suatu perulangan selama horse_file bernilai tidak NULL, yang dimana horse_file adalah fungsi membuka file jadi selama ada file yang bisa dibuka maka perulangan terus berlanjut. Dalam perulangan terdapat autoincrement pada horse_number_race yang sudah dideklarasi diawal, tujuannya adalah untuk menjadi angka diakhir format nama file, file dengan nama yang telah digabungkan dengan angka nantinya akan dibuka. Dengan mengubah horse_file_name menjadi horse_race_(horse_number_race).txt fungsi dapat membaca/membuka file berikutnya (karena seluruh file horse_race_t.txt memiliki format yang sama hanya dan semakin besar angka semakin baru file dibuat). Lalu setelah isi dari horse_file_name diubah, file sebelumnya ditutup lalu buka lagi dengan horse_file_name yang telah diganti. Dengan begitu secara otomatis perulangan akan terus dilakukan hingga file tidak ditemukan lagi. Setelah perulangan berakhir, dibuatlah suatu file dengan nama sesuai isi horse_file_name yakni format nama dengan angka terakhir ditambah 1. 
```
 fprintf(horse_file, "----Horse Race %d------\n", horse_number_race);
  time_t curr_time = time(NULL);
  struct tm tm = *localtime(&curr_time);
  fprintf(horse_file, "Date : %02d/%02d/%04d\n", 
    tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
  fprintf(horse_file, "Time : %02d/%02d/%02d\n\n", 
    tm.tm_hour, tm.tm_min, tm.tm_sec);
```
Sesuai permintaan soal, fungsi akan menuliskan hal-hal berikut ke dalam file horse_race_terbaru.txt dengan terbaru adalah angka paling baru
```
----Horse Race (Lomba keberapa dilaksanakan)------
Date: DD/MM/YYYY
Time: HH/MM/SS
```
Maka diperlukan suatu fprintf untuk mengeluarkan (print) kedalam file, dengan file yang dimasukkan adalah sesuai dengan horse_file (file horse_race_terbaru.txt yang dibuka). Untuk mendapatkan waktu dan tanggal sekarang dibutuhkan serangkaian kode yakni
```
 time_t curr_time = time(NULL);
  struct tm tm = *localtime(&curr_time);
```
Setelah waktu dan tanggal didapat masukkan kedalam file sesuai dengan format yang diberikan soal dengan fprintf.
```
  fprintf(horse_file, "Date : %02d/%02d/%04d\n", 
    tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
  fprintf(horse_file, "Time : %02d/%02d/%02d\n\n", 
    tm.tm_hour, tm.tm_min, tm.tm_sec);
```
Lalu setelah penulisan awal, file horse_race_terbaru.txt ditutup terlebih dahulu karena fungsi akan melakukan pengambilan data dan menjalankan "perlombaan" dari file awal.
```
fclose(horse_file);
```
Dibuat array thread yang akan digunakan untuk mengsimulasikan balap kuda secara bersamaan. Dibuat juga array Horse yang akan digunakan untuk menyimpan data kudanya nanti
```
pthread_t t_id[512];
Horse horses[512];
size_t t_idx = 0;
```
Setelah thread dibuat, maka dibaca setiap line di file horse yang telah dimasukkan sebagai argumen. Di dalam setiap line, diiterasi setiap karakter dalam line tersebut, dan dicari pemisah antara nama kuda dan nama pemilik, yaitu :, Jika ditemukan, maka nama kuda dan nama pemiliknya disimpan di dalam array Horse, lalu dibuat masing-masing thread menggunakan fungsi HorseRun dan argumen data kuda sebelumnya.
```
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
```
Setelah isi filenya dibaca semua, maka ditutup file tersebut, dan dijoinlah semua thread yang dipakai. Hasil akhirnya disimpan ke dalam file horse_file_name yang kemudian ditutup juga.
```
fclose(file);

for (size_t i = 0; i < t_idx; i++) {
  pthread_join(t_id[i], NULL);
}
horse_file = fopen(horse_file_name, "a");
for (int i = 0; i < horse_pos; i++) {
  fprintf(horse_file, "%d. %s %s %d\n", i+1, horse_leaderboard[i].name, horse_leaderboard[i].pemilik, horse_leaderboard[i].angka_cek);
}
fclose(horse_file);
```
Fungsi HorseChampionLeaderBoard adalah fungsi utamanya adalah membaca horse_race_x.txt dan mempresentasikannya ke stdout.
Pertama, program menanyakan ke user mengenai file horse race mana yang ingin dipresentasikan. Lalu mengprint semua file yang memiliki nama awal horse_race dari 1 sampai habis. Jika counter akhirnya 1, maka berarti tidak ada horse race file yang ditemukan, dan proses dimatikan.
```
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
```
Program pun meminta query dari user untuk mengspesifikasi file yang mana yang ingin dipresentasi. Setelah query diterima, maka dicek jika valid, lalu jika iya, dibuka race file berdasarkan query tersebut.
```
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
```
Akhirnya, diprintkan label horse championshipnya, dan semua isi file yang dispesifikasi sebelumnya.
```
fprintf(stdout, "----HORSE CHAMPIONSHIP %d----\n", horse_number_race);
char current_line[512];
fgets(current_line, sizeof(current_line), horse_file);
while (fgets(current_line, sizeof(current_line), horse_file)) {
  fprintf(stdout, "%s", current_line);
}

fclose(horse_file);
```
Saat program dijalankan, program ngecek jika terdapat satu argumen, lalu gunakan argumen tersebut untuk mengspesifikasi fungsi yang mana yang ingin digunakan.
```
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
```

#### - [Task 2 - Tugas Akhir Yuan dan Bubu _(Yuan's and Bubu's Final Project)_](/task-2/) By Nando
##### Soal 
Tugas Akhir Yuan dan Bubu

Yuan dan Bubu ditugaskan untuk mengunduh gambar oleh dosennya, namun dengan beberapa tantangan. Mereka diberikan satu folder yang berisikan file random. Dimana pada folder tersebut, terdapat beberapa file yang berisi petunjuk terkait gambar yang harus diunduh oleh Yuan dan Bubu.

- a. Pertama-tama, mereka harus mengunduh sebuah file zip dari [link berikut](https://dl.dropboxusercontent.com/scl/fi/gmy0qvbysh3h7vdexso9k/task_sisop.zip?rlkey=ism9qfjh82awg2tbtzfbrylg4&dl=0), kemudian mengekstraknya dan menghapus file zip tersebut. Setelah itu, mereka harus membuat folder baru dengan nama “task” dan memindahkan file yang berkaitan dengan petunjuk ke dalam folder “task”. File yang berkaitan memiliki penamaan dengan awalan "**task\_{id}**" dan berekstensi **txt**. Lalu hapus folder hasil zip sebelumya. Berikut struktur folder “task”:

  ```
  task/
  ├── task_0_2CtQxvh068.txt
  ├── task_1_zWjksQCero.txt
  ├── task_2_s7MsIw5hHW.txt
  └── dst.
  ```

- b. Yuan dan Bubu membagi tugas secara efisien dengan mengerjakannya secara bersamaan **(overlapping)** dan membaginya sama banyak. Yuan akan mengerjakan task dari awal, sementara Bubu dari akhir. Misalnya, jika ada 20 task, Yuan akan memulai dari task0-task1-dst dan Bubu akan memulai dari task19-task18-dst. Lalu buatlah file “recap.txt” yang menyimpan log setiap kali mereka selesai melakukan task (kriteria setiap task akan dijelaskan di poin c), dengan format sebagai berikut:

  ```
  [{time}]-[{name}] Task {id} completed, download {jumlah} {kategori} images with {resolusi} resolution
  ```

  Contoh :

  ```
  [2024-03-24 17:06:54]-[Yuan] Task 0 completed, download 1 nature images with 200x150 resolution
  [2024-03-24 17:06:58]-[Bubu] Task 19 completed, download 3 city images with 250x200 resolution
  ```

- c. Yuan dan Bubu menemukan petunjuk dalam setiap file task txt dengan format "**{jumlah} {resolusi} {kategori}**". Mereka harus mengunduh gambar sesuai dengan jumlah, resolusi, dan kategori yang disebutkan. Contohnya, jika task0 berisi "5 250x150 music", mereka mengunduh 5 gambar berukuran 250x150 dalam kategori music. Gambar diunduh dari [unsplash.com](unsplash.com) dan disimpan bersama file txt setiap task berdasarkan orang yang mengerjakan task sesuai struktur folder sebagai berikut:

  ```
  task/
  ├── Yuan/
  │   ├── task0/
  │   │   ├── task_0_2CtQxvh068.txt
  │   │   ├── gambar1.png
  │   │   ├── gambar2.png
  │   │   └── dst.
  │   ├── task1/
  │   ├── ...
  │   └── task9/
  ├── Bubu/
  │   ├── task19/
  │   ├── task18/
  │   ├── ...
  │   └── task10/
  └── rekap.txt
  ```

- d. Setelah semua tugas selesai, mereka menambahkan jumlah gambar dari setiap kategori ke dalam "recap.txt". dan pada baris terakhir tambahkan jumlah seluruh gambar yg di unduh, contoh:

  ```
  music: 20 images
  city: 10 images
  total images: 30 images
  ```

- e. Karena tugas sudah siap dikumpulkan, zip folder Yuan dan Bubu menjadi satu zip bernamakan submission.zip dan **JANGAN HAPUS** folder aslinya sehingga menyisakan struktur folder sebagai berikut:

  ```
  task/
  ├── Yuan/
  ├── Bubu/
  ├── submission.zip
  └── recap.txt
  ```

##### Penjelasan:
Full Code : 
```
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
```

Penjelasan sebelum subsoal:
Code:
```
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
```
Serangkaian kode tersebut merupakan perintah untuk mengambil (include) seluruh library yang dipakai dan mendeklarasikan variabel (struktur data) yang diperlukan untuk program kedepannya.

- a. Unduh file zip, ekstrak hanya file yang memiliki nama "task_" yang memiliki penomoran secara menaik mulai dari 0 dan ekstensi ".txt" ke dalam folder task/

Code :
```
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
```
Fungsi zip_fetch berfungsi utama untuk mengerjakan subsoal A. Alur kerja fungsinya berupa berikut :
- Fungsi membuat child process yang dibuat untuk mendownload zip file yang diminta, dan exit setelah zip file selesai didownload.
- Sementara itu, parent process menunggu child process untuk selesai mengdownload zip file yang diminta
- Setelah selesai, fungsi pun lanjut membaca isi lengkap dari zip file tersebut untuk mencari path file yang memenuhi syarat untuk diekstrak
- Caranya adalah untuk mengecek setiap line di dalam zip file, dan jika terdapat kata "task_" dan ".txt", maka diekstrak file tersebut ke dalam folder task/

![WhatsApp Image 2024-04-28 at 00 15 21_f5f6b62f](https://github.com/sisop-its-s24/praktikum-modul-2-b24/assets/79700258/6db01320-c968-4810-a4f5-58d1f70e56aa)
![WhatsApp Image 2024-04-28 at 00 15 51_9045ffba](https://github.com/sisop-its-s24/praktikum-modul-2-b24/assets/79700258/230da4c1-c9e9-4123-8d0b-718df9598e40)


- b. Membagi tugas untuk Yuan dan Bubu berdasarkan angka di file task nya

Code :
```
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

  mkdir("task/Bubu", 0777);
  mkdir("task/Yuan", 0777);
  ...
```
Fungsi divide_work di bagian kode sebelumnya adalah untuk membagi dua tugas untuk Yuan dan Bubu berdasarkan file penomoran tasknya. Pertama, dibaca isi entry folder task/ yang berisikan semua file task_x.txt, lalu disimpan ke dalam array entries yang lalu disort menggunakan fungsi compare. Fungsi compare berfungsi sebagai comparator yang mengecek penomoran task dan sorting ascending berdasarkan itu. 

![WhatsApp Image 2024-04-28 at 00 16 57_6af4b2ec](https://github.com/sisop-its-s24/praktikum-modul-2-b24/assets/79700258/e0748ffe-9448-4db1-a000-97687d87f4c5)
![WhatsApp Image 2024-04-28 at 00 16 57_05fafa00](https://github.com/sisop-its-s24/praktikum-modul-2-b24/assets/79700258/1957f1b1-7178-4429-98c5-b93aba8ed582)
![WhatsApp Image 2024-04-28 at 00 16 57_59dc8661](https://github.com/sisop-its-s24/praktikum-modul-2-b24/assets/79700258/af6f3b48-942c-4fb2-b4cc-3fea10744765)
![WhatsApp Image 2024-04-28 at 00 17 50_05ea5691](https://github.com/sisop-its-s24/praktikum-modul-2-b24/assets/79700258/618b35a1-932c-4e7c-ba90-aeb49c28459b)


- c. Mendownload foto foto Bubu dan Yuan secara bersamaan dan menulisnya ke logfile
Code :
```
void divide_work() {
  ...
  int l = 0, r = n - 1;
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
  ...
}
```
Bagian fungsi divide_work yang ini berfungsi untuk mendownload foto foto dengan spesifikasi yang ditulis di file task_x.txt masing-masing dengan bersamaan (menggunakan threading). Fungsi yang digunakan untuk threading adalah bubu_work dan yuan_work, namun perbedaan dua fungsi tersebut hanya bagian penamaannya.

```
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
```
Pertama, fungsi work akan mendeteksi penomoran file task dan membuat folder berdasarkan fungsi yang mana yang dipakai (yuan/bubu). Setelah itu, dibaca file task untuk mendapat spesifikasi yang dibutuhkan untuk mendownload foto. Lalu, digunakan spesifikasi yang dibaca untuk mendownload file yang diminta sebanyak yang dibutuhkan, sambil merekap kategori file apa yang didownload. Setelah semuanya sudah selesai, direkap file apa yang didownload langsung ke logfile.

![WhatsApp Image 2024-04-28 at 00 17 58_8d5d7414](https://github.com/sisop-its-s24/praktikum-modul-2-b24/assets/79700258/d118bb0a-09d1-4e6d-bf41-822505969163)


- d. Mencatat jumlah kategori foto dan jumlah seluruh foto ke logfile.
Kode :
```
void divide_work() {
  ...
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
```
Bagian akhir di fungsi divide_work adalah untuk mengoutputkan semua data yang telah direkap oleh fungsi work ke dalam logbook, serta total foto yang didownload.

![image](https://github.com/sisop-its-s24/praktikum-modul-2-b24/assets/79700258/37580723-3ce1-4852-8fdd-8e5f99c08865)


- e. Mengcompress folder Yuan dan Bubu ke dalam satu file zip
Kode :
```
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
```
Pertama, fungsi zip_yuan_and_bubu pindah direktori ke task/ agar di dalam zip filenya tidak ada folder task/. Lalu mengzip kedua folder Yuan dan Bubu ke satu file submission.zip.

![WhatsApp Image 2024-04-28 at 00 18 28_d7d2aa66](https://github.com/sisop-its-s24/praktikum-modul-2-b24/assets/79700258/0c58ab0a-f168-4f8e-8c62-ea9243c918a2)


#### - [Task 3 - Ramalan Cuaca Rama _(Rama's Weather Forecast)_](/task-3/) By Rey & Nando
#### Soal:
Ramalan Cuaca Rama

Rama adalah seorang mahasiswa yang mengambil mata kuliah Sistem Operasi. Ia diberikan proyek oleh dosennya untuk mengelola data cuaca dari berbagai kecamatan di Indonesia. Data tersebut tersedia dalam file zip yang bisa didownload disini. Karena suka tantangan, Rama melakukan analisis dengan menggunakan process di linux. Bantulah Rama untuk menyelesaikan tugas ini dengan satu program C bernama [weather_manager.c](./weather_manager.c). Berikut langkah-langkah yang bisa dilakukan Rama:

- a. Rama harus mendownload dan menyimpan file tersebut ke dalam penyimpanan lokal dan mengekstrak data cuaca tersebut pada folder yang bernama “weather”. Untuk menghemat memori Rama menghapus file zip tersebut.

  ```
  weather/
  ├── kecamatanforecast.csv
  └── kecamatan_geofeatures.csv
  ```

- b. Setelah berhasil, Rama melakukan data preprocessing dengan cara mencari data yang tidak masuk akal pada kolom “Suhu Udara” dan melakukan drop pada row tersebut. Lalu update dataset “kecamatanforecast.csv”.

- c. Kemudian dari dataset tersebut, Rama mengelompokkannya berdasarkan ID Lokasi dan memisahkannya menjadi dataset baru dengan format nama “city\_{ID Lokasi}” contoh “city_5012725.csv”. Semua dataset baru tersebut dimasukan ke dalam folder bernama “city_group”.

- d. Rama harus membuat logbook untuk laporan kepada dosen, sehingga semua activities diatas harus disimpan dalam “logbook.txt” dengan format { [YYYY-MM-dd HH:mm:ss] [message]}

  | Activity      | Message                                                                   |
  | ------------- | ------------------------------------------------------------------------- |
  | Downloading   | [24-4-12 12:11:10] Berhasil download file..                               |
  | Unzipping     | [24-4-12 12:20:10] Berhasil unzip file..                                  |
  | Preprocessing | [24-4-12 12:40:10] Berhasil drop row dengan {parameter} {parameter value} |
  | Grouping      | [24-4-12 12:40:10] Berhasil melakukan group data..                        |

Notes:

- Dataset “kecamatanforecast.csv” headernya memang berupa angka

- Urutan Parameter kecamatanforecast.csv (kiri ke kanan, separator ";")

  - ID Lokasi
  - Waktu dalam UTC
  - YYYY-MM-DD HH:mm:ss
  - Suhu Udara (rata-rata) Minimum dalam °C
  - Suhu Udara (rata-rata) Maksimum dalam °C
  - Kelembaban Udara (rata-rata) Minimum dalam %
  - Kelembaban Udara (rata-rata) Maksimum dalam %
  - Kelembaban Udara dalam %
  - Suhu Udara dalam °C
  - Cuaca berupa kode cuaca
  - Arah Angin berupa CARD
  - Kecepatan Angin dalam km/jam

- Hint : Menurut jurnal Science Advances (2020), tubuh manusia dapat mentoleransi suhu panas hingga 35 derajat Celcius.

- Untuk mencari outliers **tidak** perlu menggunakan metode IQR.

- Hanya file [weather_manager.c](./weather_manager.c) dan [logbook.txt](./logbook.txt) yang dikumpulkan di github.

- Gunakan `fork()`, `exec()` untuk melakukan aktivitas di atas.


#### Penjelasan:
Full Code:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX 1024

void logging(const char* act_msg) {
    char msg[1024];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(msg, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n", 
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, act_msg);
    
    FILE *logbook = fopen("logbook.txt", "a");
    fprintf(logbook, "%s", msg);
    fclose(logbook);
}

void log_drop(const char* suhu, int value) {
    char msg[100];
    sprintf(msg, "Successfully drop row with %s %d°C", suhu, value);
    logging(msg);
}

void ensure(const char *name) {
    struct stat st = {0};
    if (stat(name, &st) == -1) {
        mkdir(name, 0700);
    }
}

void downloadunzip(const char *url, const char *o) {
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        execlp("curl", "curl", url, "-Lo", "data.zip", NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
    if (WIFEXITED(status)) {
        logging("Successfully download file.");
    } 

    pid = fork();
    if (pid == 0) {
        execlp("unzip", "unzip", "-o", "data.zip", "-d", o, NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
    if (WIFEXITED(status)) {
        logging("Successfully unzip file.");
    }

    pid = fork();
    if (pid == 0) {
        execlp("rm", "rm", "data.zip", NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
}

void preprocess(const char *fp, const char *o) {
    char temp_fp[256];
    sprintf(temp_fp, "%s/temp.csv", o);

    FILE *fp_in = fopen(fp, "r");
    FILE *fp_out = fopen(temp_fp, "w");
    if (fp_in == NULL || fp_out == NULL) {
        perror("Failed to open file");
        return;
    }

    char line[MAX];
    char curr_line[MAX];
    fgets(line, sizeof(line), fp_in);
    while (fgets(line, sizeof(line), fp_in) != NULL) {
        strcpy(curr_line, line);
        char *tokens = strtok(line, ",");
        int ind = 0;
        int at;
        int keep = 1;

        while (tokens) {
            if (ind == 3) { 
                at = atoi(tokens);
                if (at > 35) {
                    keep = 0;
                    log_drop("Suhu Udara", at);
                    break;
                }
            }
            tokens = strtok(NULL, ",");
            ind++;
        }

        if (keep) {
            fprintf(fp_out, "%s", curr_line);
        }
        strcpy(line, "");
        strcpy(curr_line, "");
    }

    fclose(fp_in);
    fclose(fp_out);

    remove(fp);
    rename(temp_fp, fp);
}

void grouping(const char *input_file, const char *dir) {
    FILE *fp_in = fopen(input_file, "r");
    if (!fp_in) {
        perror("Failed to open input file");
        return;
    }

    ensure(dir);

    char line[MAX], last_id[50] = {0}, curr_line[MAX];
    FILE *fp_out = NULL;

    while (fgets(line, sizeof(line), fp_in)) {
        strcpy(curr_line, line);
        char *token = strtok(line, ",");
        char *location_id = token;

        if (fp_out && strcmp(last_id, location_id) != 0) {
            fclose(fp_out);
            fp_out = NULL;
        }

        if (!fp_out) {
            char path[256];
            snprintf(path, sizeof(path), "%s/city_%s.csv", dir, location_id);
            fp_out = fopen(path, "w");
            if (!fp_out) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            strcpy(last_id, location_id);
        }

        fprintf(fp_out, "%s", curr_line);
    }

    if (fp_out) {
        fclose(fp_out);
    }
    fclose(fp_in);
    logging("Successfully data grouping..");
}

int main() {
    const char *url = "https://drive.usercontent.google.com/u/0/uc?id=1U1-9v9pJeX5MoofwoBHCcD9nA7YWy_np&export=download";
    const char *o = "weather";
    const char *fp = "weather/kecamatanforecast.csv";

    downloadunzip(url, o);
    preprocess(fp, o);
    grouping(fp, "weather/city_group");

    return 0;
}
```
Penjelasan sebelum subsoal:
Code:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

#define MAX 1024

...

int main() {
    const char *url = "https://drive.usercontent.google.com/u/0/uc?id=1U1-9v9pJeX5MoofwoBHCcD9nA7YWy_np&export=download";
    const char *o = "weather";
    const char *fp = "weather/kecamatanforecast.csv";

 ...
    return 0;
}

```
Serangkaian kode tersebut berfungsi untuk memanggil seluruh library yang dipakai dan mendeklarasikan seluruh variabel yang akan terpakai di program nantinya.

- a. Mendownload file zip, mengekstrak file zip, dan menghapus file zip
Code:
```
void downloadunzip(const char *url, const char *o) {
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        execlp("curl", "curl", url, "-Lo", "data.zip", NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
 ...

    pid = fork();
    if (pid == 0) {
        execlp("unzip", "unzip", "-o", "data.zip", "-d", o, NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
 ...

    pid = fork();
    if (pid == 0) {
        execlp("rm", "rm", "data.zip", NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
}
```
Blok kode tersebut adalah fungsi bernama "downloadunzip" yang berguna untuk mendownload zip, mengekstrak, dan menghapus zip sebelumnya. Mulanya dibuatlah suatu child process dengan fork dan variabel int status untuk mengetahui status dari child process tersebut. Apabila child process telah terbentuk (pid==0) maka jalankan pendownloadan dengan 'exec', dan keluar secara success apabila telah berhasil. Lalu terdapat kode untuk menunggu hingga pendownloadan selesai dengan menggunakan while loop selama status lebih dari 0. Jika pendownloadan telah usai, maka program lanjut ke tahap berikutnya. Lalu dibuat lagi child process untuk unzip/ekstrak zip, kurang lebih sama seperti sebelumnya (dibuat, dilakukan, ditunggu). Setelah unzip selesai, dibuat lagi child process untuk menghapus file downloadan awal (file zip awal), sama juga seperti sebelumnya.

Untuk pemanggilan fungsi tersebut dapat dilakukan seperti berikut:
```
downloadunzip(url, o);
```
Keterangan: url adalah link yang sudah dideklarasikan sebelumnya, o adalah directory name untuk outputnya yang sudah dideklarasikan sebelumnya (folder output). 

b. Preprocess
Code:
```
void preprocess(const char *fp, const char *o) {
    char temp_fp[256];
    sprintf(temp_fp, "%s/temp.csv", o);

    FILE *fp_in = fopen(fp, "r");
    FILE *fp_out = fopen(temp_fp, "w");
    if (fp_in == NULL || fp_out == NULL) {
        perror("Failed to open file");
        return;
    }

    char line[MAX];
    char curr_line[MAX];
    fgets(line, sizeof(line), fp_in);
    while (fgets(line, sizeof(line), fp_in) != NULL) {
        strcpy(curr_line, line);
        char *tokens = strtok(line, ",");
        int ind = 0;
        int at;
        int keep = 1;

        while (tokens) {
            if (ind == 3) { 
                at = atoi(tokens);
                if (at > 35) {
                    keep = 0;
                    ...
                    break;
                }
            }
            tokens = strtok(NULL, ",");
            ind++;
        }

        if (keep) {
            fprintf(fp_out, "%s", curr_line);
        }
        strcpy(line, "");
        strcpy(curr_line, "");
    }

    fclose(fp_in);
    fclose(fp_out);

    remove(fp);
    rename(temp_fp, fp);
}
```
Bagian kode tersebut adalah untuk memproses file “kecamatanforecast.csv” yang dimana apabila ditemukan data yang janggal pada kolom "Suhu Udara" (kolom ketujuh) maka hapus baris yang mengandung data tersebut. Data dikatakan janggal apabila nilainya melebihi 35. Pertama buka filenya, apabila gagal maka keluarkan "Failed to open file". Kemudian buat suatu file temporary sebagai file transit sementara. Lalu diceklah setiap barisnya pada kolom ketujuh (jadi whileloop untuk mengecek tiap barisnya, dan whileloop lagi didalamnya untuk mengecek isi kolom-kolomnya hingga ketujuh), pemisah kolom disini merupakan koma (',') karena format file adalah '.csv'. Dicek tiap barisnya hingga kolom ketujuh, apabila data sesuai (kolom ketujuh datanya sesuai) maka baris tersebut akan dimasukkan ke dalam file temporary (dicopy), apabila data tidak sesuai (pada kolom ketujuh nilainya lebih dari 35) maka baris tersebut tidak akan dimasukkan/dicopy. Begitu seterusnya sampai data habis. Kemudian setelah selesai, tutup kedua file tersebut (file sumber dan temporary). Setelah ditutup, hapus file sumber dan ubah nama file temporary dari temp_fp menjadi fp, dengan fp adalah variabel 'const char *fp = "weather/kecamatanforecast.csv"' (jadi file sumber diganti file yang telah di preprocess). 

- c. Grouping
Code:
```
void ensure(const char *name) {
    struct stat st = {0};
    if (stat(name, &st) == -1) {
        mkdir(name, 0700);
    }
}
...
void grouping(const char *input_file, const char *dir) {
    FILE *fp_in = fopen(input_file, "r");
    if (!fp_in) {
        perror("Failed to open input file");
        return;
    }

    ensure(dir);

    char line[MAX], last_id[50] = {0}, curr_line[MAX];
    FILE *fp_out = NULL;

    while (fgets(line, sizeof(line), fp_in)) {
        strcpy(curr_line, line);
        char *token = strtok(line, ",");
        char *location_id = token;

        if (fp_out && strcmp(last_id, location_id) != 0) {
            fclose(fp_out);
            fp_out = NULL;
        }

        if (!fp_out) {
            char path[256];
            snprintf(path, sizeof(path), "%s/city_%s.csv", dir, location_id);
            fp_out = fopen(path, "w");
            if (!fp_out) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            strcpy(last_id, location_id);
        }

        fprintf(fp_out, "%s", curr_line);
    }

    if (fp_out) {
        fclose(fp_out);
    }
    fclose(fp_in);
 ...
}
```
Pada subsoal C, diminta untuk mengelompokkan data-data dari dataset tersebut berdasarkan id lokasi dan memisahkannya menjadi dataset baru dengan format nama "city_Idlokasi" lalu dimasukkan ke dalam folder bernama "city_group". Mulanya dibuatlah suatu fungsi untuk memastikan atau _ensure_ apakah file city_group sudah ada sebelumnya atau belum, jika belum maka buat folder city_group. Kemudian dibuatlah suatu fungsi grouping untuk pengelompokkannya. Grouping dimulai dengan membuka file dataset, apabila gagal maka keluarkan "Failed to open input file". Kemudian panggil fungsi ensure untuk memastikan folder city_group telah ada atau belum. Lalu kolom pertama akan menjadi nilai acuan id location. Kemudian fungsi grouping akan mengecek apakah id terakhir sama dengan id location sekarang dan ketika file output terbuka, apabila tidak (pertama kali pasti tidak karerna id terakhir pasti nilainya 0), maka lanjut ke berikutnya. Selanjutnya dicek apabila file output dibuka atau tidak, bila tidak maka buka file sesuai dengan directory dengan nama city_idlocation (bila belum ada akan otomatis terbuat), lalu fp_out (file output) diberi nilai hasil perintah membuka write file dengan directory tersebut, apabila gagal maka keluarkan "Failed to open output file". Lalu copy id location ke id terakhir (last_id), dan program akan memasukkan baris sekarang ke dalam file dengan directory yang telah dibuat (city_idlocation). Dilakukan terus menerus hingga data habis. Apabila ternyata file terbuka dan id locationnya masih sama maka tidak diperlukan untuk membuat/membuka file city_ lainnya (bisa baru bila belum ada sebelumnya, ataupun membuka yang sudah ada sebelumnya). Apabila sudah selesai maka file output dan file input ditutup. 

Untuk pemanggilan fungsi dalam program utama dapat dilakukan sebagai berikut:
```
grouping(fp, "weather/city_group");
```
Dengan nilai fp adalah "weather/kecamatanforecast.csv".

- d. Logging
Code:
void logging(const char* act_msg) {
    char msg[1024];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(msg, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n", 
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, act_msg);
    
    FILE *logbook = fopen("logbook.txt", "a");
    fprintf(logbook, "%s", msg);
    fclose(logbook);
}

void log_drop(const char* suhu, int value) {
    char msg[100];
    sprintf(msg, "Successfully drop row with %s %d°C", suhu, value);
    logging(msg);
}
```
Pada setiap fungsi juga terdapat pemanggilan fungsi logging
```
void downloadunzip(const char *url, const char *o) {
    pid_t pid = fork();
    int status;

    if (pid == 0) {
        execlp("curl", "curl", url, "-Lo", "data.zip", NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
    if (WIFEXITED(status)) {
        **logging("Successfully download file.");** <<<<======
    } 

    pid = fork();
    if (pid == 0) {
        execlp("unzip", "unzip", "-o", "data.zip", "-d", o, NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
    if (WIFEXITED(status)) {
        **logging("Successfully unzip file.");** <<<<======
    }

    pid = fork();
    if (pid == 0) {
        execlp("rm", "rm", "data.zip", NULL);
        exit(EXIT_SUCCESS);
    }
    while (wait(&status) > 0);
}

void preprocess(const char *fp, const char *o) {
    char temp_fp[256];
    sprintf(temp_fp, "%s/temp.csv", o);

    FILE *fp_in = fopen(fp, "r");
    FILE *fp_out = fopen(temp_fp, "w");
    if (fp_in == NULL || fp_out == NULL) {
        perror("Failed to open file");
        return;
    }

    char line[MAX];
    char curr_line[MAX];
    fgets(line, sizeof(line), fp_in);
    while (fgets(line, sizeof(line), fp_in) != NULL) {
        strcpy(curr_line, line);
        char *tokens = strtok(line, ",");
        int ind = 0;
        int at;
        int keep = 1;

        while (tokens) {
            if (ind == 3) { 
                at = atoi(tokens);
                if (at > 35) {
                    keep = 0;
                    **log_drop("Suhu Udara", at)**; <<<<=====
                    break;
                }
            }
            tokens = strtok(NULL, ",");
            ind++;
        }

        if (keep) {
            fprintf(fp_out, "%s", curr_line);
        }
        strcpy(line, "");
        strcpy(curr_line, "");
    }

    fclose(fp_in);
    fclose(fp_out);

    remove(fp);
    rename(temp_fp, fp);
}

void grouping(const char *input_file, const char *dir) {
    FILE *fp_in = fopen(input_file, "r");
    if (!fp_in) {
        perror("Failed to open input file");
        return;
    }

    ensure(dir);

    char line[MAX], last_id[50] = {0}, curr_line[MAX];
    FILE *fp_out = NULL;

    while (fgets(line, sizeof(line), fp_in)) {
        strcpy(curr_line, line);
        char *token = strtok(line, ",");
        char *location_id = token;

        if (fp_out && strcmp(last_id, location_id) != 0) {
            fclose(fp_out);
            fp_out = NULL;
        }

        if (!fp_out) {
            char path[256];
            snprintf(path, sizeof(path), "%s/city_%s.csv", dir, location_id);
            fp_out = fopen(path, "w");
            if (!fp_out) {
                perror("Failed to open output file");
                exit(EXIT_FAILURE);
            }
            strcpy(last_id, location_id);
        }

        fprintf(fp_out, "%s", curr_line);
    }

    if (fp_out) {
        fclose(fp_out);
    }
    fclose(fp_in);
    **logging("Successfully data grouping..");** <<<<=====
}
```
Sesuai permintaan soal, setiap pendowloadan, pengunzipan, preprocessing, dan grouping akan dilakukan logging ke dalam logbook.txt. Pertama dibuatlah suatu fungsi logging. Fungsi logging tersebut mengambil inputan pesan pada saat pemanggilan fungsi dilakukan. Kemudian fungsi logging akan mengambil waktu dan tanggal saat ini dan menyusunnya menjadi suatu pesan dengan format "[tahun-bulan-tanggal jam:menit:detik] pesan" (sesuai permintaan soal) dengan pesan merupakan pesan sesuai dengan pemanggil fungsi (jika download ya pesannya sesuai yang dipanggil saat download). Berikut ini adalah contoh pemanggilan fungsi dalam fungsi downlaodunzip:
```
logging("Successfully download file.");
```
Fungsi logging dipanggil dengan pesan inputan yakni "Successfully download file." sehingga pesan menjadi "[tahun-bulan-tanggal jam:menit:detik] Successfully download file.", begitu juga untuk fungsi-fungsi lainya.
Setelah terbentuk pesan secara lengkap (beserta waktu), fungsi akan membuka file "logbook.txt" dan memasukkan pesan kedalam file tersebut. Kemudian apabila sudah dimasukkan, file ditutup.

Adapula fungsi log_drop untuk kegunaan secara khusus yaitu untuk logging ketika preprocessing (terdapat baris yang di drop). Fungsi log_drop akan mengambil inputan dari pemanggil berupa suhu (string) dan value (integer). Berikut adalah contoh pemanggilan fungsi log_drop dari fungsi preprocess:
```
log_drop("Suhu Udara", at)
```
Fungsi log_drop dipanggil dengan memasukkan string "Suhu Udara" dan variabel 'at' untuk value dari suhunya. Ketika sampai di fungsi log_drop, fungsi akan menyusun pesan dengan format "Successfully drop row with %s %d°C" dengan '%s' adalah string 'suhu' yang berisikan "Suhu Udara" dan '%d' adalah nilai dari integer 'value' yang berisikan nilai dari integer 'at'. Lalu fungsi diakhiri dengan pemanggilan fungsi logging dengan pesan yang telah tersusun untuk memasukkan pesan (logging) ke dalam file "logbook.txt". Berikut adalah pemanggilan fungsi logging oleh fungsi log_drop:
```
logging(msg); //msg adalah pesan yang sudah disusun sebelumnya
```

#### - [Task 4 - Tic Tac Toe](/task-4/) By Rey
##### Soal:
Tic Tac Toe

Sebagai mahasiswa baru informatika, satria memiliki ide untuk membuat game tic-tac-toe yang berjalan di terminal. Tic-tac-toe merupakan permainan dengan 2 pemain yang secara bergantian menempatkan tanda 'X' atau 'O' hingga membentuk garis horizontal, vertikal, atau diagonal. Untuk membuat game ini, satria membutuhkan 2 program c:

- [game.c](./game.c): game server, program untuk mengontrol permainan
- [player.c](./player.c): program untuk menerima input dan menampilkan output pada pengguna

Berikut skema program yang telah direncanakan:

- a. Pengguna dapat memilih tipe pemain (‘X’ atau ‘O’) berdasarkan perintah dari program ‘player.c’. Jika tipe telah dipilih oleh pengguna sebelumnya, beri pesan "<tipe pemain> has already been taken, choose other player types". Informasi terkait apakah tipe pemain telah dipilih atau tidak, dapat diakses melalui shared memory. 

  Contoh output (**player.c**):

  ```
  Available player type:
  1. X
  2. O
  Enter your choice (1 or 2): 1
  'X' has already been taken, choose other player types

  Available player type:
  1. X
  2. O
  Enter your choice (1 or 2): 2
  ```

- b. Karena berjalan di terminal, program ini menggunakan nomor kotak (1-9) yang berurutan dari kiri atas ke kanan bawah untuk memudahkan pemilihan kotak. Program 'player.c' akan menampilkan kotak tic tac toe saat ini dan meminta input pengguna berupa nomor kotak sesuai gilirannya (player 'X' memulai terlebih dahulu). Input yang diberikan pengguna kemudian dikirimkan ke 'game.c' menggunakan message queue. Pada ‘game.c’, tampilkan setiap pesan yang diterima dari ‘player.c’ dengan format seperti di bawah.

  Contoh output (**player.c**):

  ```
  ----Game Started!----
  You play as 'X'

  1 | 2 | 3
  --- --- ---
  4 | 5 | 6
  --- --- ---
  7 | 8 | 9
  Select the corresponding cell number to choose a cell.

  Your turn
  Enter a cell number: 1
  ```

  Contoh output (**game.c**):

  ```
  New Message
  Sender: player X
  Message: 1
  New Message
  Sender: player O
  Message: 7
  New Message
  Sender: player X
  Message: 9
  ```

- c. Selanjutnya, ‘game.c’ harus mengecek apakah input user sesuai atau tidak berdasarkan nilai kotak permainan saat ini. Kirimkan pesan error kepada pengguna jika nomor kotak yang dipilih tidak kosong atau diluar angka 1-9. Pesan dapat dikirimkan melalui message queue dengan ‘mesg_type’ yang berbeda.

  Contoh output (**player.c**):

  ```
  Player 'X' turn
  X |   |
  --- --- ---
    |   |
  --- --- ---
    |   |

  Your turn
  Enter a cell number: 11
  Wrong input, choose cell number 1-9
  Enter a cell number: 1
  Choose other cell, 1 is filled by 'X'
  ```

- d. Jika input valid, ‘game.c’ perlu mengubah nilai pada kotak tic-tac-toe dan memutuskan apakah permainan telah selesai atau masih berlanjut. Permainan telah selesai jika kotak telah terisi semua (seri) atau terdapat simbol (‘X’ atau ‘O’) yang membentuk garis vertikal, horizontal, maupun diagonal. Kotak tic tac toe yang telah diperbarui dan hasil akhir permainan (jika sudah berakhir) dikirimkan ke 'player.c' untuk kemudian ditampilkan kepada pengguna.

  Contoh output (**player.c**):

  ```
  Your turn
  Enter a cell number: 6
  X | O |
  --- --- ---
  X | X | X
  --- --- ---
  O |   | O

  ---Game Finished!---
  You Win!
  ```

Notes:

- Untuk memainkannya, jalankan ‘game.c’ terlebih dahulu. Selanjutnya, jalankan 2 program 'player.c', 1 untuk player 'X' dan 1 untuk player 'O', sebagai antarmuka.

  Contoh:

  ![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/4684c310-53e1-445e-b18b-7c1e892179fe)

- Untuk memudahkan, kalian dapat menggunakan ‘mesg_type’ yang berbeda untuk tiap komunikasi, sebagai contoh:

  - ‘mesg_type’ = 1, untuk pesan dari player ke game server
  - ‘mesg_type’ = 2, untuk pesan dari game server ke player 'X'
  - ‘mesg_type’ = 3, untuk pesan dari game server ke player 'O'

- Format atau struktur pesan dibebaskan asal dapat memenuhi poin-poin pada soal
  
##### Penjelasan:
Full Code game.c:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define QUEUE_KEY 1234
#define BOARD_SIZE 10
#define MSG_SIZE 200

typedef struct {
    long message_type; 
    char message_text[MSG_SIZE];
} Message;

char board[BOARD_SIZE];
int roles[2] = {0, 0};  
void reset_game() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = '1' + i;
    }
    roles[0] = 0;
    roles[1] = 0;
    
}


int main() {
    int msgid;
    Message msg;
    msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    int step=0;
    int count=0;
    int num;
    char * a;
    int flag=0;
    int temp;
    reset_game();
    printf("Game Started\n");
    while (1) {
        msgrcv(msgid, &msg, sizeof(msg), 1, 0);
        if(msg.message_text[5] == 'X' &&  flag==0){
            step=1;
        }
        flag+=1;
        if (strncmp(msg.message_text, "role", 4) == 0) {
            int player_index = (msg.message_text[5] == 'X' ? 0 : 1);
            msg.message_type= (msg.message_text[5] == 'X' ? 2 : 3);
            if (roles[player_index] == 1) {
                strcpy(msg.message_text, "False");
                msgsnd(msgid, &msg, sizeof(msg), 0);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                continue;
            } else {
                roles[player_index] = 1;
                if(flag==1){
                    strcpy(msg.message_text, "True");
                }else{
                    strcpy(msg.message_text, "True1");
                }
                
            }
            msgsnd(msgid, &msg, sizeof(msg), 0);
            if(roles[0]==1 && roles[1]==1){
                msg.message_type=3;
                     strcpy(msg.message_text, "Pe");
                     msgsnd(msgid, &msg, sizeof(msg), 0);
                     msg.message_type=2;
                      strcpy(msg.message_text, "Pe");
                    msgsnd(msgid, &msg, sizeof(msg), 0);
                    continue;
             
                   }
            continue;
            }
            

        count++;
        if(count%2!=0){
                num=msg.message_text[0]-'0';
               board[num] = (step == 1 ? 'X' : 'O'); 
                 printf("New Message\nSender: player %c\nMessage: %c\n",
                   (step == 1 ? 'X' : 'O'), msg.message_text[0]);
            msg.message_type=(step == 1 ? 3 : 2);
            temp = msg.message_type;
            }
        else{
            num=msg.message_text[0]-'0';
             board[num] = (step == 0 ? 'X' : 'O');
             printf("New Message\nSender: player %c\nMessage: %c\n",
                   (step == 0 ? 'X' : 'O'), msg.message_text[0]);
            msg.message_type=(step != 1 ? 3 : 2);
            temp = msg.message_type;
             
        }
            if ((board[1]==board[2])&&((board[2]==board[3])) || (board[1]==board[4])&&((board[4]==board[7])) || (board[1]==board[5])&&((board[5]==board[9])) || (board[2]==board[5])&&((board[5]==board[8])) || (board[3]==board[6])&&((board[6]==board[9])) || (board[3]==board[5])&&((board[5]==board[7])) || (board[4]==board[5])&&((board[5]==board[6])) || (board[7]==board[8])&&((board[8]==board[9]))) {
                msg.message_type = (temp == 2 ? 3 : 2);
                sprintf(msg.message_text, "You Win");
                msgsnd(msgid, &msg, sizeof(msg), 0); 
                sprintf(msg.message_text, "You Lose");
                msg.message_type = (msg.message_type  == 2 ? 3 : 2);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sprintf(msg.message_text, "%d", num);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                reset_game();  
                printf("Game Finished\n");
                break;
            }else if(count==9){
                
                sprintf(msg.message_text, "Draw");
                msg.message_type = 3;
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sprintf(msg.message_text, "%d", num);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                msg.message_type = 2;
                sprintf(msg.message_text, "Draw");
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sprintf(msg.message_text, "%d", num);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                reset_game();  
                printf("Game Finished\n");
                break;
            }
        msgsnd(msgid, &msg, sizeof(msg), 0);
        
    }
    return 0;
}
```
Full Code player.c:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>


#define QUEUE_KEY 1234
#define MSG_SIZE 200

typedef struct {
    long message_type;  
    char message_text[MSG_SIZE];
} Message;
char board[10];
void format_board() {
    printf("\n %c | %c | %c \n---+---+---\n %c | %c | %c \n---+---+---\n %c | %c | %c \n",
            board[1], board[2], board[3],
            board[4], board[5], board[6],
            board[7], board[8], board[9]);
}

int main() {
    int msgid;
    Message msg;
    char choice;
    int temp;
    int namber;
    int first=0;
    int a = 9;
    while(a>0){
        board[a]=' ';
        a--;
    }
    char *temp2 = malloc(256);
    msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("Error accessing message queue");
        exit(1);
    }
    
    while (1) {
        printf("Available player type:\n1. X\n2. O\nEnter your choice (1 or 2): ");
        scanf(" %c", &choice);

        if (choice != '1' && choice != '2') {
            printf("Invalid choice. Please enter 1 or 2.\n");
            continue;
        }

        
        sprintf(msg.message_text, "role %c", (choice == '1' ? 'X' : 'O'));
        msg.message_type = 1;
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msgrcv(msgid, &msg, sizeof(msg), (choice == '1' ? 2 : 3), 0);
        if ( strcmp(msg.message_text, "True") == 0) {
            printf("\nWaiting for another player to join the game...\n\n");
            first=1;
            break;
        } if ( strcmp(msg.message_text, "True1") == 0) {
            break;
        }else {
            printf("'%c' has already been taken, choose other player types\n\n", choice == '1' ? 'X' : 'O');
            continue;
        }
    }

    while (1) {
        msgrcv(msgid, &msg, sizeof(msg), msg.message_type, 0);
        if(strcmp (msg.message_text, "False") == 0){
            continue;
        }
        if(strcmp(msg.message_text, "You Win") == 0 || strcmp(msg.message_text, "You Lose") == 0  || strcmp(msg.message_text, "Draw") == 0 ){
            sprintf(temp2, "%s", msg.message_text);
            if(strcmp(msg.message_text, "You Lose") == 0 || strcmp(msg.message_text, "Draw") == 0 ){
                msgrcv(msgid, &msg, sizeof(msg), msg.message_type, 0);
                int pos = msg.message_text[0]-'0';
                board[pos] = (msg.message_type == 2 ? 'O' : 'X');
            }
            printf("\n--- Last Board ---\n");
            format_board();
            printf("\n---Game Finished!---\n" );
            sprintf(msg.message_text, "%s", temp2);
            printf("%s\n", msg.message_text);
            break;}
        else if(strcmp(msg.message_text, "Pe")!=0){
                int pos = msg.message_text[0]-'0';
                board[pos] = (msg.message_type == 2 ? 'O' : 'X');
        }if(strcmp(msg.message_text, "Pe")==0){
            printf("\n----Game Started!----\nYou play as '%c'\n\n 1 | 2 | 3\n--- --- ---\n 4 | 5 | 6\n--- --- ---\n 7 | 8 | 9\n\nSelect the corresponding cell number to choose a cell.\n ", msg.message_type == 2 ? 'X' : 'O');
            format_board();
            if(first==0){
                printf("\nWaiting for the other player to make the first move...\n");
                continue;
            }
        }else{ 
            printf("\nPlayer %c's turn", msg.message_type == 2 ? 'X' : 'O');
            format_board();
        }
        int f = 0;
        printf("\nYour turn\n");
        while(!f){
            printf("Enter a cell number: ");
            scanf("%d", &namber);
            if(namber>9 || namber<1){
                printf("Wrong input, choose cell number 1-9\n");
                continue;
            }
            if(board[namber] == 'X'){
                printf("Choose other cell, %c is filled by 'X'\n", msg.message_text[0]);
            }
            else if(board[namber] == 'O'){
                printf("Choose other cell, %c is filled by 'O'\n", msg.message_text[0]);
            }else{
                f=1;
                board[namber] = (msg.message_type == 2 ? 'X' : 'O');
                sprintf(msg.message_text, "%d", namber);
            }
        }
        temp = msg.message_type;
        msg.message_type=1;
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msg.message_type = temp;
    }

    
    
    return 0;
}
```
Penjelasan sebelum sub soal (game.c):
Code:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define QUEUE_KEY 1234
#define BOARD_SIZE 10
#define MSG_SIZE 200

typedef struct {
    long message_type; 
    char message_text[MSG_SIZE];
} Message;

char board[BOARD_SIZE];
int roles[2] = {0, 0};  
void reset_game() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i] = '1' + i;
    }
    roles[0] = 0;
    roles[1] = 0;
    
}


int main() {
    int msgid;
    Message msg;
    msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    int step=0;
    int count=0;
    int num;
    char * a;
    int flag=0;
    int temp;
    reset_game();
    printf("Game Started\n");
```
Serangkaian kode diatas adalah seluruh pemanggilan seluruh library, pendeklarasian, dan pembuatan variabel dalam game.c yang nantinya seluruh hal tersebut akan dipakai kedepannya. Ada pula print outputan "Game Started" untuk menandai game.c mulai dijalankan

Penjelasan sebelum sub soal (game.c):
Code:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>


#define QUEUE_KEY 1234
#define MSG_SIZE 200

typedef struct {
    long message_type;  
    char message_text[MSG_SIZE];
} Message;
char board[10];
void format_board() {
    printf("\n %c | %c | %c \n---+---+---\n %c | %c | %c \n---+---+---\n %c | %c | %c \n",
            board[1], board[2], board[3],
            board[4], board[5], board[6],
            board[7], board[8], board[9]);
}

int main() {
    int msgid;
    Message msg;
    char choice;
    int temp;
    int namber;
    int first=0;
    int a = 9;
    while(a>0){
        board[a]=' ';
        a--;
    }
    char *temp2 = malloc(256);
    msgid = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("Error accessing message queue");
        exit(1);
    }
```
Serangkaian blok kode diatas adalah seluruh pemanggilan library, pendeklarasian, pembuatan variabel yang nantinya akan dipakai dalam keseluruhan kode player.c .

- a. Output player.c
Code player.c:
```
 while (1) {
        printf("Available player type:\n1. X\n2. O\nEnter your choice (1 or 2): ");
        scanf(" %c", &choice);

        if (choice != '1' && choice != '2') {
            printf("Invalid choice. Please enter 1 or 2.\n");
            continue;
        }

        
        sprintf(msg.message_text, "role %c", (choice == '1' ? 'X' : 'O'));
        msg.message_type = 1;
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msgrcv(msgid, &msg, sizeof(msg), (choice == '1' ? 2 : 3), 0);
        if ( strcmp(msg.message_text, "True") == 0) {
            printf("\nWaiting for another player to join the game...\n\n");
            first=1;
            break;
        } if ( strcmp(msg.message_text, "True1") == 0) {
            break;
        }else {
            printf("'%c' has already been taken, choose other player types\n\n", choice == '1' ? 'X' : 'O');
            continue;
        }
    }
```
Blok kode tersebut merupakan suatu while loop yang hanya akan berhenti ketika di break (karena always true atau 1). Kode ini akan memberikan output pertama kali kepada user yakni sebagai berikut:
```
Available player type:
1. X
2. O
Enter your choice (1 or 2): 1
```
Kemudian kode akan mengambil inputan dari user. Inputan dimasukkan kedalam variabel dan dipassing ke game.c dengan message_type 1. Setelah dicek di game.c apakah pilihan pemain yang dipilih apakah sudah terpilih sebelumnya atau belum, message dikembalikan ke player.c (jadi player.c msgsnd lalu msgrcv). Apabila pesan yang didapat "True" berarti pemain memilih pertama kali, jadi keluarkan "nWaiting for another player to join the game..." dan keluar dari loop, apabila "True1" maka user telah memilih pemain kedua jadi tidak ada kalimat yang dikeluarkan dan keluar dari loop, selain dari dua pesan tersebut ("False") maka keluarkan "'%c' has already been taken, choose other player types" dengan '%c' adalah pilihan user, lalu kode akan mengulangi loop dan meminta user memasukkan inputan sampai benar (memilih pemain yang benar). Kode juga mencegah user memasukkan inputan selain 1 dan 2 pada bagian kode berikut:
```
   if (choice != '1' && choice != '2') {
            printf("Invalid choice. Please enter 1 or 2.\n");
            continue;
        }
```
Ketika user memasukkan inputan yang salah, loop diulang dan user diminta memasukkan kembali.

Code game.c:
 while (1) {
        msgrcv(msgid, &msg, sizeof(msg), 1, 0);
        if(msg.message_text[5] == 'X' &&  flag==0){
            step=1;
        }
        flag+=1;
        if (strncmp(msg.message_text, "role", 4) == 0) {
            int player_index = (msg.message_text[5] == 'X' ? 0 : 1);
            msg.message_type= (msg.message_text[5] == 'X' ? 2 : 3);
            if (roles[player_index] == 1) {
                strcpy(msg.message_text, "False");
                msgsnd(msgid, &msg, sizeof(msg), 0);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                continue;
            } else {
                roles[player_index] = 1;
                if(flag==1){
                    strcpy(msg.message_text, "True");
                }else{
                    strcpy(msg.message_text, "True1");
                }
                
            }
Bagian kode tersebut adalah penggalan dari suatu loop yang bersifat always true. Game.c akan terus menerus menunggu dan menerima inputan dari player.c sampai game berakhir (di setiap awal loop), dan akan mengembalikan message ke player.c sesuai kebutuhan. Untuk pemilihan player, disini game.c menerima message dari player.c, kemudian dicek apakah sudah terdapat atau belum. Disini status pemain dibuat dalam suatu array integer roles dimana pada index 0 adalah status X dan index 1 adalah status O. Maka dicek apabila pilihan pemain X pada player.c, dicek lagi apakah sudah terpilih belum, bila belum maka beri nilai 1 pada roles[0] dan kirimkan pesan True1, apabila sudah kirimkan False, apabila pilihannya berlawanan X (O) kirimkan True (begitu juga sebaliknya). 

Sekedar tambahan, pengondisian diawal (penggunaan variabel step) dan flag digunakan untuk menentukan pertama kali tipe player yang dipilih pemain, (untuk menentukan urutan, kami disini urutannya tergantung dari player mana yang memilih role terlebih dahulu).

b. Memulai permainan
Code player.c:
```
 while (1) {
        msgrcv(msgid, &msg, sizeof(msg), msg.message_type, 0);
        if(strcmp (msg.message_text, "False") == 0){
            continue;
        }
...

if(strcmp(msg.message_text, "Pe")==0){
            printf("\n----Game Started!----\nYou play as '%c'\n\n 1 | 2 | 3\n--- --- ---\n 4 | 5 | 6\n--- --- ---\n 7 | 8 | 9\n\nSelect the corresponding cell number to choose a cell.\n ", msg.message_type == 2 ? 'X' : 'O');
            format_board();
            if(first==0){
                printf("\nWaiting for the other player to make the first move...\n");
                continue;
            }
        }else{ 
            printf("\nPlayer %c's turn", msg.message_type == 2 ? 'X' : 'O');
            format_board();
        }
        int f = 0;
        printf("\nYour turn\n");
        while(!f){
            printf("Enter a cell number: ");
            scanf("%d", &namber);
            if(namber>9 || namber<1){
                printf("Wrong input, choose cell number 1-9\n");
                continue;
            }
            if(board[namber] == 'X'){
                printf("Choose other cell, %c is filled by 'X'\n", msg.message_text[0]);
            }
            else if(board[namber] == 'O'){
                printf("Choose other cell, %c is filled by 'O'\n", msg.message_text[0]);
            }else{
                f=1;
                board[namber] = (msg.message_type == 2 ? 'X' : 'O');
                sprintf(msg.message_text, "%d", namber);
            }
        }
        temp = msg.message_type;
        msg.message_type=1;
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msg.message_type = temp;
    }
```
Bagian kode tersebut merupakan sebagian besar dari suatu perulangan dalam player.c. Setiap tahap perulangan, player.c akan menunggu pesan dari game.c. Untuk memulai suatu permainan akan diberikan pesan "Pe" dari game.c kepada player.c. Setelah menerima "Pe", player.c akan mengeluarkan output sesuai dengan soal. Pada tahapan awal permainan, player.c akan mengecek apakah dia pertama atau bukan, apabila pertama (first=1, akan bernilai 1 ketika True1 di kode sebelumnya), maka program akan langsung meminta input kepada user, apabila tidak maka program akan mengeluarkan "Waiting for the other player to make the first move...". Setelah user menginput, dicek apakah sesuai atau tidak, apabila tidak maka ulangi lagi penginputan (ada while loop dalam while loop), ketika inputan sudah benar maka berikan message kepada game.c dan game.c akan mengeluarkan pesan pada terminal. Pada player.c, board atau papan juga diisi pada index sesuai angka inputan user. 

Code game.c:
```
msgsnd(msgid, &msg, sizeof(msg), 0);
            if(roles[0]==1 && roles[1]==1){
                msg.message_type=3;
                     strcpy(msg.message_text, "Pe");
                     msgsnd(msgid, &msg, sizeof(msg), 0);
                     msg.message_type=2;
                      strcpy(msg.message_text, "Pe");
                    msgsnd(msgid, &msg, sizeof(msg), 0);
                    continue;
             
                   }
            continue;
            }
            

        count++;
        if(count%2!=0){
                num=msg.message_text[0]-'0';
               board[num] = (step == 1 ? 'X' : 'O'); 
                 printf("New Message\nSender: player %c\nMessage: %c\n",
                   (step == 1 ? 'X' : 'O'), msg.message_text[0]);
            msg.message_type=(step == 1 ? 3 : 2);
            temp = msg.message_type;
            }
        else{
            num=msg.message_text[0]-'0';
             board[num] = (step == 0 ? 'X' : 'O');
             printf("New Message\nSender: player %c\nMessage: %c\n",
                   (step == 0 ? 'X' : 'O'), msg.message_text[0]);
            msg.message_type=(step != 1 ? 3 : 2);
            temp = msg.message_type;
             
        }
...

msgsnd(msgid, &msg, sizeof(msg), 0);
        
    }
```
Blok kode tersebut adalah lanjutan dari while loop pada sub soal sebelumnya. Di awal, program akan mengirimkan pesan "Pe" kepada kedua player untuk memulai permainan, lalu menunggu pesan inputan dari player yang berjalan pertama. Setelah didapat maka increment pada count (dilakukan tiap tahapan) untuk menentukan giliran ganjil atau genap (player pertama bergerak pada count ganjil dan player kedua bergerak pada count genap). Lalu dicek apakah ganjil atau genap, lalu dicek lagi apakah X atau O pada tahapan tersebut, lalu apabila sudah masukkan nilai X atau O pada board, keluarkan pesan seperti pada permintaan soal, dan diakhir kirimkan pesan kembali ke player.c.

c. Jalannya permainan
Code player.c:
```
 while (1) {
        msgrcv(msgid, &msg, sizeof(msg), msg.message_type, 0);
        if(strcmp (msg.message_text, "False") == 0){
            continue;
        }
else if(strcmp(msg.message_text, "Pe")!=0){
                int pos = msg.message_text[0]-'0';
                board[pos] = (msg.message_type == 2 ? 'O' : 'X');
if(strcmp(msg.message_text, "Pe")==0){
            printf("\n----Game Started!----\nYou play as '%c'\n\n 1 | 2 | 3\n--- --- ---\n 4 | 5 | 6\n--- --- ---\n 7 | 8 | 9\n\nSelect the corresponding cell number to choose a cell.\n ", msg.message_type == 2 ? 'X' : 'O');
            format_board();
            if(first==0){
                printf("\nWaiting for the other player to make the first move...\n");
                continue;
            }
        }else{ 
            printf("\nPlayer %c's turn", msg.message_type == 2 ? 'X' : 'O');
            format_board();
        }
        int f = 0;
        printf("\nYour turn\n");
        while(!f){
            printf("Enter a cell number: ");
            scanf("%d", &namber);
            if(namber>9 || namber<1){
                printf("Wrong input, choose cell number 1-9\n");
                continue;
            }
            if(board[namber] == 'X'){
                printf("Choose other cell, %c is filled by 'X'\n", msg.message_text[0]);
            }
            else if(board[namber] == 'O'){
                printf("Choose other cell, %c is filled by 'O'\n", msg.message_text[0]);
            }else{
                f=1;
                board[namber] = (msg.message_type == 2 ? 'X' : 'O');
                sprintf(msg.message_text, "%d", namber);
            }
        }
        temp = msg.message_type;
        msg.message_type=1;
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msg.message_type = temp;
    }
```
Seperti pada soal sebelumnya, pada player.c akan mengecek apakah inputan langkah user sesuai atau tidak, apabila sesuai kirimkan ke game.c jika tidak maka prompt user kembali hingga user memasukkan inputan yang sesuai. Tambahan penjelasan dari sebelumnya, jika pesan yang diterima bukan "Pe" maka jadikan itu sebagai informasi untuk mengisi board. Apabila posisi penerima adalah X maka masukkan O kedalam board sesuai pesan, begitu juga sebaliknya.

d. Akhir Permainan
Code player.c:
```
 if(strcmp(msg.message_text, "You Win") == 0 || strcmp(msg.message_text, "You Lose") == 0  || strcmp(msg.message_text, "Draw") == 0 ){
            sprintf(temp2, "%s", msg.message_text);
            if(strcmp(msg.message_text, "You Lose") == 0 || strcmp(msg.message_text, "Draw") == 0 ){
                msgrcv(msgid, &msg, sizeof(msg), msg.message_type, 0);
                int pos = msg.message_text[0]-'0';
                board[pos] = (msg.message_type == 2 ? 'O' : 'X');
            }
            printf("\n--- Last Board ---\n");
            format_board();
            printf("\n---Game Finished!---\n" );
            sprintf(msg.message_text, "%s", temp2);
            printf("%s\n", msg.message_text);
            break;}
```
Pada player.c sebelum "Pe" dan pengecekan bukan "Pe" sebetulnya terdapat pengondisian/pengecekan apabila pesan yang diterima adalah pesan menang, kalah, ataupun seri. Jadi apabila pesan yang diterima "You Win" atau "You Lose" ataupun "Draw" maka akan masuk ke pengondisian. Lalu cek, apabila "You Lose" ataupun "Draw" maka diperlukan inputan informasi langkah terakhir lawan, disini game.c akan memberikan informasi tersebut setelah memberikan pesan "You Lose", setelah diterima, masukkan ke board, apabila "You Win" langsung ke tahap berikutnya. Tahap berikutnya, keluarkan "--- Last Board ---" dan keluarkan papan, tulisan "---Game Finished!---" dan keluarkan apakah pemain menang, kalah, atau seri. Setelah semuanya dikeluarkan, kode tersebut diakhiri dengan break atau keluar dari loop.

Code game.c:
```
 if ((board[1]==board[2])&&((board[2]==board[3])) || (board[1]==board[4])&&((board[4]==board[7])) || (board[1]==board[5])&&((board[5]==board[9])) || (board[2]==board[5])&&((board[5]==board[8])) || (board[3]==board[6])&&((board[6]==board[9])) || (board[3]==board[5])&&((board[5]==board[7])) || (board[4]==board[5])&&((board[5]==board[6])) || (board[7]==board[8])&&((board[8]==board[9]))) {
                msg.message_type = (temp == 2 ? 3 : 2);
                sprintf(msg.message_text, "You Win");
                msgsnd(msgid, &msg, sizeof(msg), 0); 
                sprintf(msg.message_text, "You Lose");
                msg.message_type = (msg.message_type  == 2 ? 3 : 2);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sprintf(msg.message_text, "%d", num);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                reset_game();  
                printf("Game Finished\n");
                break;
            }else if(count==9){
                
                sprintf(msg.message_text, "Draw");
                msg.message_type = 3;
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sprintf(msg.message_text, "%d", num);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                msg.message_type = 2;
                sprintf(msg.message_text, "Draw");
                msgsnd(msgid, &msg, sizeof(msg), 0);
                sprintf(msg.message_text, "%d", num);
                msgsnd(msgid, &msg, sizeof(msg), 0);
                reset_game();  
                printf("Game Finished\n");
                break;
            }
```
Kode tersebut adalah bagian akhir dari while loop di game.c. Blok tersebut akan mengecek pada setiap langkah apakah permainan sudah berakhir atau belum. Pertama terdapat pengondisian apabila terbentuk suatu pola berurutan (antara X atau O) yang menandakan adanya pemenang dalam permainan tersebut (dicek setiap kemungkinan garis/pola yang ada), bila ada maka lanjut ke tahap berikutnya. Tahap berikutnya pada pengondisian pertama adalah, game.c akan memberikan pesan "You Win" kepada player yang menang, dan "You Lose" kepada yang kalah, serta informasi langkah terakhir dari pemenang ke pemain yang kalah. Pengondisian kedua adalah apabila game telah berakhir (mencapai tahap 9) tetapi tidak ada pemenang ataupun yang kalah (seri) maka lanjut ke tahap berikutnya pada pengondisian tersebut. Sama seperti sebelumnya, game.c akan memberikan pesan "Draw" kepada kedua pemain dan satu informasi hanya kepada pemain yang sebelum terakhir berjalan (jadi misalkan X terakhir berjalan, maka informasi tersebut akan diberikan ke O) supaya player tersebut tahu bentuk papan terakhir dan dapat ditampilkan. Lalu ketika kedua pengondisian tersebut mencapai akhir, keluarkan "Game Finished" dan keluar dari loop.
