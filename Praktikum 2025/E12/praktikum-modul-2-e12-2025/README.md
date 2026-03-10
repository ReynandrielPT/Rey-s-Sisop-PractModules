[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/9LcL5VTQ)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241178 | Christian Mikaxelo |
| 5025221218 | Ida Bagus Gde Dimas Sutha Maha |
| 5025241227 | Safa Maulana Efendi  |

# Praktikum Modul 2 _(Module 2 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - Trabowo & Peddy Movie Night](/task-1/)
- [Task 2 - Organize and Analyze Anthony's Favorite Films](/task-2/)
- [Task 3 - Cella’s Manhwa](/task-3/)
- [Task 4 - Pipip's Load Balancer](/task-4/)
# Trabowo & Peddy Movie Night
## Deskripsi:
Trabowo dan sahabatnya, Peddy, sedang menikmati malam minggu di rumah sambil mencari film seru untuk ditonton. Mereka menemukan sebuah file ZIP yang berisi gambar-gambar poster film yang sangat menarik. File tersebut dapat diunduh dari **[Google Drive](https://drive.google.com/file/d/1nP5kjCi9ReDk5ILgnM7UCnrQwFH67Z9B/view?usp=sharing)**. Karena penasaran dengan film-film tersebut, mereka memutuskan untuk membuat sistem otomatis guna mengelola semua file tersebut secara terstruktur dan efisien. Berikut adalah tugas yang harus dikerjakan untuk mewujudkan sistem tersebut:

### Soal:
### **a. Ekstraksi File ZIP**

Trabowo langsung mendownload file ZIP tersebut dan menyimpannya di penyimpanan lokal komputernya. Namun, karena file tersebut dalam bentuk ZIP, Trabowo perlu melakukan **unzip** agar dapat melihat daftar film-film seru yang ada di dalamnya.

### Jawaban:
File film di unzip menggunakan kode
# Tranbowo-a.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void bjer(const char* zipFi) {

    pid_t pid = fork();
    if (pid == 0) {
        char *bruh[] = {"unzip", "-o", (char *)zipFi, "-d", ".", NULL};
        execvp(bruh[0], bruh); 
    } 
    else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0); 
        if (status) {
            printf("DONE\n");
        } else {
            printf("GAGAL\n");
        }
    } 
}

int main() {
     char* zipFi = "film.zip";  
    bjer(zipFi);
    return 0;
}
```
### Fungsi bjer(const char* zipFi)
```c
Fungsi bjer(const char* zipFi)
```
Function yang akan unzip file
### fork()
```c
pid_t pid = fork();
```
Melakukan percabangan proses menjadi proses induk (parent) dan proses anak (child).
### Proses Anak
```c
if (pid == 0) {
    char *bruh[] = {"unzip", "-o", (char *)zipFi, "-d", ".", NULL};
    execvp(bruh[0], bruh); 
}
```
- Proses anak menjalankan perintah unzip -o [zipFi] -d . menggunakan execvp().
- -o digunakan agar file yang sudah ada ditimpa tanpa konfirmasi.
- -d. menunjukkan bahwa hasil ekstraksi akan ditempatkan di direktori saat ini.
- execvp() menggantikan proses anak dengan proses unzip.

### Proses Induk
```c
else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0); 
```
Proses induk menunggu proses anak selesai menggunakan waitpid().
```c
if (WIFEXITED(status) && WEXITSTATUS(status) == 0){
            printf("DONE\n");
} else {
            printf("GAGAL\n");
        }
```
Setelah selesai, dicek nilai status, Jika non-nol: dianggap sukses, cetak "DONE". Jika nol: dianggap gagal, cetak "GAGAL".
### Fungsi Main()
```c
int main() {
    char* zipFi = "film.zip";  
    bjer(zipFi);
    return 0;
}
Mendefinisikan nama file zip: "film.zip". Memanggil fungsi bjer() untuk mengekstrak file tersebut.
```
### Direktori sebelum film.zip di unzip
![Image](https://github.com/user-attachments/assets/88c56552-44a3-4e1d-aa20-8e70b2900b6f)

### Direktori setelah film.zip di unzip
![Image](https://github.com/user-attachments/assets/6f28eb55-904b-4c52-9a82-6c23207f41e8)

### Output Terminal
![Image](https://github.com/user-attachments/assets/bb59d44b-b3f5-4219-932a-0a1c7944cd0d)



### **b. Pemilihan Film Secara Acak**
### Soal: 
Setelah berhasil melakukan unzip, Trabowo iseng melakukan pemilihan secara acak/random pada gambar-gambar film tersebut untuk menentukan film pertama yang akan dia tonton malam ini.

**Format Output:**

```
Film for Trabowo & Peddy: ‘<no_namafilm_genre.jpg>’
```
### Jawaban:
Pemilihan film secara acak akan dilakukan oleh file
### Trabowo-b.c
```c
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
```
Membuka direktori dengan nama yang diberikan ("film" dalam kasus ini).
```c
    DIR* dir = opendir(dirBruh);
```
bruh akan menyimpan info tentang masing-masing file yang dibaca. files adalah array untuk menyimpan nama-nama file. JumFil adalah jumlah file yang ditemukan.
```c
    struct dirent* bruh;
    char* files[1000];
    int JumFil = 0;
```
Melakukan pembacaan file dalam direktori satu per satu. Mengabaikan file "." (menunjuk ke direktori itu sendiri). Menyimpan nama file ke dalam array files.
```c
    while ((bruh = readdir(dir)) != NULL) {
        if (strcmp(bruh->d_name, ".") != 0 ) {
            files[JumFil++] = bruh->d_name;  
        }
    }
```
Menutup direktori setelah selesai membaca isinya.
```c
    closedir(dir);
```

Jika ada file yang ditemukan, pilih 1 secara acak. Cetak nama file yang dipilih dalam format: Film for Trabowo & Peddy: ‘<%s>’
```c
    if (JumFil > 0) {
        srand(time(NULL));
        int ran = rand() % JumFil;  
        printf("Film for Trabowo & Peddy: ‘<%s>’\n", files[ran]);


    } else {
        printf("lah kok ga ada?\n");
    }
```

### Output
![Image](https://github.com/user-attachments/assets/f639c742-9b34-4d3e-8d03-e5ddfcaeaa33)

![Image](https://github.com/user-attachments/assets/4b77084a-01d2-4866-89b6-ce543e17b1a5)

![Image](https://github.com/user-attachments/assets/675dad10-2453-4045-b834-f58817cd8782)

### **c. Memilah Film Berdasarkan Genre**
### Soal
Karena Trabowo sangat perfeksionis dan ingin semuanya tertata rapi, dia memutuskan untuk mengorganisir film-film tersebut berdasarkan genre. Dia membuat 3 direktori utama di dalam folder `~/film`, yaitu:

- **FilmHorror**
- **FilmAnimasi**
- **FilmDrama**

Setelah itu, dia mulai memindahkan gambar-gambar film ke dalam folder yang sesuai dengan genrenya. Tetapi Trabowo terlalu tua untuk melakukannya sendiri, sehingga ia meminta bantuan Peddy untuk memindahkannya. Mereka membagi tugas secara efisien dengan mengerjakannya secara bersamaan (overlapping) dan membaginya sama banyak. Trabowo akan mengerjakan dari awal, sementara Peddy dari akhir. Misalnya, jika ada 10 gambar, Trabowo akan memulai dari gambar pertama, gambar kedua, dst dan Peddy akan memulai dari gambar kesepuluh, gambar kesembilan, dst. Lalu buatlah file “recap.txt” yang menyimpan log setiap kali mereka selesai melakukan task

Contoh format log :

```
[15-04-2025 13:44:59] Peddy: 50_toystory_animasi.jpg telah dipindahkan ke FilmAnimasi
```

Setelah memindahkan semua film, Trabowo dan Peddy juga perlu menghitung jumlah film dalam setiap kategori dan menuliskannya dalam file **`total.txt`**. Format dari file tersebut adalah:

```
Jumlah film horror: <jumlahfilm>
Jumlah film animasi: <jumlahfilm>
Jumlah film drama: <jumlahfilm>
Genre dengan jumlah film terbanyak: <namagenre>
```

### Trabowo-c.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
// 3 dir 
// di saring
// trabowo dari depan, pedydy dari belakang
typedef struct {
    char filename[256];
    char genre[20];
} Movie;

void mkbjer(char *dir_name) {
    struct stat st = {0};
    if (stat(dir_name, &st) == -1) { if (mkdir(dir_name, 0700) != 0) { perror("bruh");} }}


void recap(char *worker, char *filename, char *genre) {
    FILE *logFile = fopen("recap.txt", "a");
    if (logFile == NULL) {
        perror("Fbruh");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(logFile, "[%02d-%02d-%04d %02d:%02d:%02d] %s: %s telah dipindahkan ke %s\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, worker, filename, genre);
    fclose(logFile);
}
void pindah(char *file, char *desti) {
    char pathhh[512];
    char dir_path[512];
    char desti_path[512];

    snprintf(pathhh, sizeof(pathhh), "./film/%s", file);
    snprintf(desti_path, sizeof(desti_path), "./film/%s/%s", desti, file);
    snprintf(dir_path, sizeof(dir_path), "./film/%s", desti);
    mkbjer(dir_path); 
    if (rename(pathhh, desti_path) != 0) {
        perror("bruh");
    }
}
void saring(char *dir_path, Movie *muvi, int *count) {
    DIR *dir = opendir(dir_path);
    struct dirent *entry;
    if (dir == NULL) {
        perror("dir bruh");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  
            strcpy(muvi[*count].filename, entry->d_name);
            if (strstr(entry->d_name, "horror") != NULL) {
                strcpy(muvi[*count].genre, "FilmHorror");
            } else if (strstr(entry->d_name, "animasi") != NULL) {
                strcpy(muvi[*count].genre, "FilmAnimasi");
            } else if (strstr(entry->d_name, "drama") != NULL) {
                strcpy(muvi[*count].genre, "FilmDrama");
            } else {
                continue;  
            }
            (*count)++;
        }
    }
    closedir(dir);
}

int main() {
    Movie muvi[1000];
    int muviCou = 0;
    char *dir_path = "./film";
    saring(dir_path, muvi, &muviCou);
    int i = 0, j = muviCou - 1;
    int horrorCou = 0, animasiCou = 0, dramaCou = 0;
    while (i <= j) {
        if (strstr(muvi[i].genre, "FilmHorror") != NULL) {
            horrorCou++;
        } else if (strstr(muvi[i].genre, "FilmAnimasi") != NULL) {
            animasiCou++;
        } else if (strstr(muvi[i].genre, "FilmDrama") != NULL) {
            dramaCou++;
        }
        pindah(muvi[i].filename, muvi[i].genre);
        recap("Trabowo", muvi[i].filename, muvi[i].genre);
        i++;
        if (i <= j) {
            if (strstr(muvi[j].genre, "FilmHorror") != NULL) {
                horrorCou++;
            } else if (strstr(muvi[j].genre, "FilmAnimasi") != NULL) {
                animasiCou++;
            } else if (strstr(muvi[j].genre, "FilmDrama") != NULL) {
                dramaCou++;
            }
            pindah(muvi[j].filename, muvi[j].genre);
            recap("Peddy", muvi[j].filename, muvi[j].genre);
            j--;
        }
    }
    FILE *fileTOT = fopen("total.txt", "w");
    if (fileTOT == NULL) {
        perror("bruh");
        return 1;
    }
    fprintf(fileTOT, "Jumlah film horror: %d\n", horrorCou);
    fprintf(fileTOT, "Jumlah film animasi: %d\n", animasiCou);
    fprintf(fileTOT, "Jumlah film drama: %d\n", dramaCou);
    if (horrorCou >= animasiCou && horrorCou >= dramaCou) {
        fprintf(fileTOT, "Film terbanyak: FilmHorror\n");
    } else if (animasiCou >= horrorCou && animasiCou >= dramaCou) {
        fprintf(fileTOT, "Film terbanyak: FilmAnimasi\n");
    } else {
        fprintf(fileTOT, "Film terbanyak: FilmDrama\n");
    }
    fclose(fileTOT);
    return 0;
}
/*   /\_/\
*    (= ._.)
*   / >  \>
*/
// sorry gabut mas hehehehehhee
```

Struct untuk menyimpan nama file dan genre film.
```c
typedef struct {
    char filename[256];
    char genre[20];
} Movie;
```


Mengecek apakah direktori tujuan sudah ada. Jika belum, buat direktori baru
```c
void mkbjer(char *dir_name) {
    struct stat st = {0};
    if (stat(dir_name, &st) == -1) { if (mkdir(dir_name, 0700) != 0) { perror("bruh");} }}
```


Menuliskan log pemindahan film ke file recap.txt dengan timestamp dan nama pekerja (Trabowo atau Peddy).
```c
void recap(char *worker, char *filename, char *genre) {
    FILE *logFile = fopen("recap.txt", "a");
    if (logFile == NULL) {
        perror("Fbruh");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(logFile, "[%02d-%02d-%04d %02d:%02d:%02d] %s: %s telah dipindahkan ke %s\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, worker, filename, genre);
    fclose(logFile);
}
```

Contoh format log:
```
    [30-04-2025 14:02:01] Trabowo: film_horror1.mp4 telah dipindahkan ke FilmHorror
```

Memindahkan file dari ./film/ ke subdirektori ./film/[genre] menggunakan rename(). Membuat direktori tujuan jika belum ada. Memindahkan file
```c
void pindah(char *file, char *desti) {
    char pathhh[512];
    char dir_path[512];
    char desti_path[512];

    snprintf(pathhh, sizeof(pathhh), "./film/%s", file);
    snprintf(desti_path, sizeof(desti_path), "./film/%s/%s", desti, file);
    snprintf(dir_path, sizeof(dir_path), "./film/%s", desti);
    mkbjer(dir_path); 
    if (rename(pathhh, desti_path) != 0) {
        perror("bruh");
    }
}
```

Membaca semua file reguler di folder ./film dan mengidentifikasi genre berdasarkan nama file, Mengandung horror → FilmHorror, Mengandung animasi → FilmAnimasi, Mengandung drama → FilmDrama, File dengan genre tak dikenal → dilewati
```c
void saring(char *dir_path, Movie *muvi, int *count) {
    DIR *dir = opendir(dir_path);
    struct dirent *entry;
    if (dir == NULL) {
        perror("dir bruh");
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  
            strcpy(muvi[*count].filename, entry->d_name);
            if (strstr(entry->d_name, "horror") != NULL) {
                strcpy(muvi[*count].genre, "FilmHorror");
            } else if (strstr(entry->d_name, "animasi") != NULL) {
                strcpy(muvi[*count].genre, "FilmAnimasi");
            } else if (strstr(entry->d_name, "drama") != NULL) {
                strcpy(muvi[*count].genre, "FilmDrama");
            } else {
                continue;  
            }
            (*count)++;
        }
    }
    closedir(dir);
}
```

Proses utama program, Menyaring file dan menyimpan ke array muvi, Melakukan iterasi dua arah, Trabowo memproses dari awal, Peddy memproses dari akhir, Setiap file dipindahkan, dicatat log-nya, Menghitung jumlah film tiap genre, Menuliskan ringkasan total film ke total.txt
```c
int main() {
    Movie muvi[1000];
    int muviCou = 0;
    char *dir_path = "./film";
    saring(dir_path, muvi, &muviCou);
    int i = 0, j = muviCou - 1;
    int horrorCou = 0, animasiCou = 0, dramaCou = 0;
    while (i <= j) {
        if (strstr(muvi[i].genre, "FilmHorror") != NULL) {
            horrorCou++;
        } else if (strstr(muvi[i].genre, "FilmAnimasi") != NULL) {
            animasiCou++;
        } else if (strstr(muvi[i].genre, "FilmDrama") != NULL) {
            dramaCou++;
        }
        pindah(muvi[i].filename, muvi[i].genre);
        recap("Trabowo", muvi[i].filename, muvi[i].genre);
        i++;
        if (i <= j) {
            if (strstr(muvi[j].genre, "FilmHorror") != NULL) {
                horrorCou++;
            } else if (strstr(muvi[j].genre, "FilmAnimasi") != NULL) {
                animasiCou++;
            } else if (strstr(muvi[j].genre, "FilmDrama") != NULL) {
                dramaCou++;
            }
            pindah(muvi[j].filename, muvi[j].genre);
            recap("Peddy", muvi[j].filename, muvi[j].genre);
            j--;
        }
    }
    FILE *fileTOT = fopen("total.txt", "w");
    if (fileTOT == NULL) {
        perror("bruh");
        return 1;
    }
    fprintf(fileTOT, "Jumlah film horror: %d\n", horrorCou);
    fprintf(fileTOT, "Jumlah film animasi: %d\n", animasiCou);
    fprintf(fileTOT, "Jumlah film drama: %d\n", dramaCou);
    if (horrorCou >= animasiCou && horrorCou >= dramaCou) {
        fprintf(fileTOT, "Film terbanyak: FilmHorror\n");
    } else if (animasiCou >= horrorCou && animasiCou >= dramaCou) {
        fprintf(fileTOT, "Film terbanyak: FilmAnimasi\n");
    } else {
        fprintf(fileTOT, "Film terbanyak: FilmDrama\n");
    }
    fclose(fileTOT);
    return 0;
}
```

Bonus Kaomoji :3
```c
/*   /\_/\
*    (= ._.)
*   / >  \>
*/
// sorry gabut mas hehehehehhee
```


### Direktori sebelum Trabowo-c.c dijalankan
![Image](https://github.com/user-attachments/assets/56d318a4-4ea4-48c6-96d7-08ba0e63cb61)


### Direktori setelah Trabowo-c.c dijalankan
![Image](https://github.com/user-attachments/assets/c965e2f1-931d-40a8-99a1-d8e082ae20eb)


### Isi dari file recap.txt
![Image](https://github.com/user-attachments/assets/f3e163c9-93dd-4914-a4ea-f789dc2a8ff0)

### Isi dari file total.txt
![Image](https://github.com/user-attachments/assets/fe732372-718f-42b4-a02c-5a2e293570e3)

### Bonus
![Image](https://github.com/user-attachments/assets/fa4cd522-98e7-4236-835a-191a9e1e0275)


### **d. Pengarsipan Film**
### Soal
Setelah semua film tertata dengan rapi dan dikelompokkan dalam direktori masing-masing berdasarkan genre, Trabowo ingin mengarsipkan ketiga direktori tersebut ke dalam format **ZIP** agar tidak memakan terlalu banyak ruang di komputernya.

### Jawaban
File di zip dengan
# Trabowo-d.c
```c
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

```



zipFi: Nama file output ZIP. bruh[]: Argumen untuk perintah terminal:, zip -r filmdone.zip film
```c
    char *zipFi = "filmdone.zip";
    char *bruh[] = {"zip", "-r", zipFi, "film", NULL};  
```
Membuat child process untuk menjalankan zip.
```c
    pid_t pid = fork();
```
Jika fork gagal, tampilkan pesan kesalahan dan keluar.
```c
    if (pid == -1) {
        perror("fork failed");
        return 1;
```

Child process menjalankan perintah zip. Jika execvp gagal, maka akan mencetak "gagal".
```c
        execvp("zip", bruh);
        perror("gagal");
```
Parent menunggu child selesai (waitpid).
```c
        int status;
        waitpid(pid, &status, 0);
```
Jika child keluar dengan status sukses (exit code 0), tampilkan Zip DONE. Jika tidak, tampilkan GAGAL.
```c
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("Zip DONE: %s\n", zipFi);
        } else {
            printf("GAGAL\n");
        }
    }
```

    
### Hasil Zip
![Image](https://github.com/user-attachments/assets/2f01dd83-11fb-435e-b7c8-eb0ef5e83289)

### Output Terminal
![Image](https://github.com/user-attachments/assets/0f876345-533c-46bd-8969-aaa245ba78de)


  ## Organize and Analyze Anthony's Favorite Films

Anthony sedang asyik menonton film favoritnya dari Netflix, namun seiring berjalannya waktu, koleksi filmnya semakin menumpuk. Ia pun memutuskan untuk membuat sistem agar film-film favoritnya bisa lebih terorganisir dan mudah diakses. Anthony ingin melakukan beberapa hal dengan lebih efisien dan serba otomatis.

> Film-film yang dimaksud adalah film-film yang ada di dalam file ZIP yang bisa diunduh dari **[Google Drive](https://drive.google.com/file/d/12GWsZbSH858h2HExP3x4DfWZB1jLdV-J/view?usp=drive_link)**.

Berikut adalah serangkaian tugas yang Anthony ingin capai untuk membuat pengalaman menonton filmnya jadi lebih menyenangkan:

### **a. One Click and Done!**

Pernahkah kamu merasa malas untuk mengelola file ZIP yang penuh dengan data film? Anthony merasa hal yang sama, jadi dia ingin semuanya serba instan dengan hanya satu perintah. Dengan satu perintah saja, Anthony bisa:

- Mendownload file ZIP yang berisi data film-film Netflix favoritnya.
- Mengekstrak file ZIP tersebut ke dalam folder yang sudah terorganisir.
- Menghapus file ZIP yang sudah tidak diperlukan lagi, supaya tidak memenuhi penyimpanan.
- 
### Jawaban :

```c
void folder(const char *nama) {
    struct stat st = {0};
    if (stat(nama, &st) == -1) {
        mkdir(nama, 0777);
    }
}
```

Cek apakah folder nama sudah ada. Jika belum, buat dengan permission penuh (0777)


```c
void one_click() {
    pid_t pid = fork();
    if (pid == 0) {
        char *url = "https://drive.google.com/uc?export=download&id=12GWsZbSH858h2HExP3x4DfWZB1jLdV-J";
        char *argv[] = {"wget", "-O", "netflixData.zip", url, NULL};
        execv("/usr/bin/wget", argv);
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
        pid = fork();
        if (pid == 0) {
            mkdir("/home/ubuntu/prak2", 0777);
            char *argv[] = {"unzip", "-o", "netflixData.zip", "-d", "/home/ubuntu/prak2", NULL};
            execv("/usr/bin/unzip", argv);
            exit(EXIT_FAILURE);
        } else {
            wait(NULL);
            pid = fork();
            if (pid == 0) {
                char *argv[] = {"rm", "-f", "netflixData.zip", NULL};
                execv("/bin/rm", argv);
                exit(EXIT_FAILURE);
            } else {
                wait(NULL);
                printf("Download dan ekstrak selesai\n");
            }
        }
    }
}
```
- Pertama membuat proses anak (fork()) untuk menjalankan perintah download, selanjutnya proses anak menjalankan wget dengan execv untuk download ZIP dan menyimpannya sebagai netflixData.zip lalu menunggu hingga download selesai. selanjutnya ekstrak isi ZIP ke folder /home/ubuntu/prak2 menunggu hingga download selesai, dan yang terakhir menghapus file ZIP setelah selesai diekstrak

### b. Sorting Like a Pro

Koleksi film Anthony semakin banyak dan dia mulai bingung mencari cara yang cepat untuk mengelompokkannya. Nah, Anthony ingin mengelompokkan film-filmnya dengan dua cara yang sangat mudah:

1. Berdasarkan huruf pertama dari judul film.
2. Berdasarkan tahun rilis (release year).

Namun, karena Anthony sudah mempelajari **multiprocessing**, dia ingin mengelompokkan kedua kategori ini secara paralel untuk menghemat waktu.

**Struktur Output:**

- **Berdasarkan Huruf Pertama Judul Film:**

  - Folder: `judul/`
  - Setiap file dinamai dengan huruf abjad atau angka, seperti `A.txt`, `B.txt`, atau `1.txt`.
  - Jika judul film tidak dimulai dengan huruf atau angka, film tersebut disimpan di file `#.txt`.

- **Berdasarkan Tahun Rilis:**
  - Folder: `tahun/`
  - Setiap file dinamai sesuai tahun rilis film, seperti `1999.txt`, `2021.txt`, dst.

Format penulisan dalam setiap file :

```
Judul Film - Tahun Rilis - Sutradara
```

Setiap proses yang berjalan akan mencatat aktivitasnya ke dalam satu file bernama **`log.txt`** dengan format:

```
[jam:menit:detik] Proses mengelompokkan berdasarkan [Abjad/Tahun]: sedang mengelompokkan untuk film [judul_film]
```

**Contoh Log:**

```
[14:23:45] Proses mengelompokkan berdasarkan Abjad: sedang mengelompokkan untuk film Avengers: Infinity War
[14:23:46] Proses mengelompokkan berdasarkan Tahun: sedang mengelompokkan untuk film Kung Fu Panda
```

### Jawaban 

```c
void log(const char *kategori, const char *judul) {
    FILE *log = fopen("log.txt", "a");
    if (!log) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(log, "[%02d:%02d:%02d] sort by %s: untuk %s\n",
            t->tm_hour, t->tm_min, t->tm_sec, kategori, judul);
    fclose(log);
}
```
- Menulis log saat proses pengelompokan berjalan

```c
void sorting() {
    folder("judul");
    FILE *in = fopen("/home/ubuntu/prak2/netflixData.csv", "r");
    if (!in) return;
    char line[MAX_LINE];
    fgets(line, sizeof(line), in); 
    while (fgets(line, sizeof(line), in)) {
        char *judul = strtok(line, ",");
        char *sutradara = strtok(NULL, ",");
        char *negara = strtok(NULL, ",");
        char *tahun = strtok(NULL, ",\n");

        if (!judul || !tahun || !sutradara) continue;
        char first = toupper(judul[0]);
        char filename[100];
        if (isalnum(first)) sprintf(filename, "judul/%c.txt", first);
        else sprintf(filename, "judul/#.txt");

        FILE *out = fopen(filename, "a");
        if (out) {
            fprintf(out, "%s - %s - %s\n", judul, tahun, sutradara);
            fclose(out);
        }
        log("abjad", judul);
    }
    fclose(in);
}
```

- Mengambil huruf pertama dari judul (A-Z, 0-9), jika bukan huruf/angka → simpan ke #.txt

```c
void proses_tahun() {
    folder("tahun");
    FILE *in = fopen("/home/ubuntu/prak2/netflixData.csv", "r");
    if (!in) {
        perror("File tidak ditemukan");
        return;
    }
    char line[MAX_LINE];
    fgets(line, sizeof(line), in); 
    while (fgets(line, sizeof(line), in)) {
        char *judul = strtok(line, ",");
        char *sutradara = strtok(NULL, ",");
        char *negara = strtok(NULL, ",");
        char *tahun = strtok(NULL, ",\n");

        if (!judul || !tahun) continue;

        while (*tahun == ' ') tahun++;

        if (strlen(tahun) == 0 || !isdigit(tahun[0])) continue;

        char filename[256];
        sprintf(filename, "tahun/%s.txt", tahun);

        FILE *out = fopen(filename, "a");
        if (out) {
            fprintf(out, "%s - %s - %s\n", judul, tahun, sutradara ? sutradara : "N/A");
            fclose(out);
        }
        log("tahun", judul);
    }
    fclose(in);
}
```

- Ambil tahun dari CSV, buang spasi awal, validasi dengan `isdigit`, dan jika sutradara kosong → tulis "N/A"

```c
void group() {
    pid_t pid1 = fork();
    if (pid1 == 0) {
        sorting();
        exit(0);
    }
    pid_t pid2 = fork();
    if (pid2 == 0) {
        proses_tahun();
        exit(0);
    }
    wait(NULL);
    wait(NULL);
    printf("selesai.\n");
}
```

- Membuat dua fork ( ) menjalankan sorting( ) dan proses_tahun( ) secara paralel

### c. The Ultimate Movie Report

Sebagai penggemar film yang juga suka menganalisis, Anthony ingin mengetahui statistik lebih mendalam tentang film-film yang dia koleksi. Misalnya, dia ingin tahu berapa banyak film yang dirilis **sebelum tahun 2000** dan **setelah tahun 2000**.

Agar laporan tersebut mudah dibaca, Anthony ingin hasilnya disimpan dalam file **`report_ddmmyyyy.txt`**.

**Format Output dalam Laporan:**

```
i. Negara: <nama_negara>
Film sebelum 2000: <jumlah>
Film setelah 2000: <jumlah>

...
i+n. Negara: <nama_negara>
Film sebelum 2000: <jumlah>
Film setelah 2000: <jumlah>
```

### Jawaban

```c
void trim(char *str) {
    int start = 0;
    int end = strlen(str) - 1;
    while (isspace(str[start])) start++;
    while (end >= start && isspace(str[end])) end--;
    int i;
    for (i = start; i <= end; i++) str[i - start] = str[i];
    str[i - start] = '\0';
}

void clean_string(char *str) {
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (isprint(str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}
```

- fungsi `trim()` untuk menghapus spasi di awal dan akhir string
- fungsi `clean_string()` untuk menghapus karakter non-printable

```c
void laporan() {
    FILE *in = fopen("/home/ubuntu/prak2/netflixData.csv", "r");
    if (!in) {
        perror("not found!");
        return;
    }

    char *data[1000];
    int j_data = 0;
    char line[MAX_LINE];
    char n_unix[MAX_NEGARA][1001];
    int n_count = 0;

    fgets(line, sizeof(line), in); 
    while (fgets(line, sizeof(line), in)) {
        if (j_data < 1000) {
            data[j_data++] = strdup(line);
        }
    }
    fclose(in);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char filename[1001];
    sprintf(filename, "report_%02d%02d%04d.txt", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    FILE *out = fopen(filename, "w");

    for (int i = 0; i < j_data; i++) {
        char *copy = strdup(data[i]);
        char *judul = strtok(copy, ",");
        char *sutradara = strtok(NULL, ",");
        char *negara_ptr = strtok(NULL, ",");
        char *tahun_str = strtok(NULL, ",\n");

        if (negara_ptr && tahun_str) {
            char negara[1001];
            strcpy(negara, negara_ptr);
            trim(negara);
            clean_string(negara);
            int tahun = atoi(tahun_str);

            if (strlen(negara) > 0) {
                int found = -1;
                for (int j = 0; j < n_count; j++) {
                    if (strcmp(n_unix[j], negara) == 0) {
                        found = j;
                        break;
                    }
                }

                if (found == -1) {
                    strcpy(n_unix[n_count], negara);
                    found = n_count++;
                    fprintf(out, "%d. Negara: %s\n", found + 1, n_unix[found]);
                    fprintf(out, "Film sebelum 2000: 0\n");
                    fprintf(out, "Film setelah 2000: 0\n\n");
                }

                int sebelum = 0;
                int sesudah = 0;
                for (int k = 0; k < j_data; k++) {
                    if (data[k] != NULL) {
                        char *temp_copy = strdup(data[k]);
                        char *judul_temp = strtok(temp_copy, ",");
                        char *sutradara_temp = strtok(NULL, ",");
                        char *temp_negara_ptr = strtok(NULL, ",");
                        char *temp_tahun_str = strtok(NULL, ",\n");
                        if (temp_negara_ptr && temp_tahun_str) {
                            char temp_negara[1001];
                            strcpy(temp_negara, temp_negara_ptr);
                            trim(temp_negara);
                            clean_string(temp_negara);
                            int temp_tahun = atoi(temp_tahun_str);
                            if (strcmp(temp_negara, n_unix[found]) == 0) {
                                if (temp_tahun < 2000) {
                                    sebelum++;
                                } else {
                                    sesudah++;
                                }
                            }
                        }
                        free(temp_copy);
                    }
                }

                fseek(out, 0, SEEK_SET);
                for (int l = 0; l < n_count; l++) {
                    int sblm = 0;
                    int ssdh = 0;
                    for (int k = 0; k < j_data; k++) {
                        if (data[k] != NULL) {
                            char *temp_copy2 = strdup(data[k]);
                            char *judul_temp2 = strtok(temp_copy2, ",");
                            char *sutradara_temp2 = strtok(NULL, ",");
                            char *temp_negara_ptr2 = strtok(NULL, ",");
                            char *temp_tahun_str2 = strtok(NULL, ",\n");
                            if (temp_negara_ptr2 && temp_tahun_str2) {
                                char temp_negara2[1001];
                                strcpy(temp_negara2, temp_negara_ptr2);
                                trim(temp_negara2);
                                clean_string(temp_negara2);
                                int temp_tahun2 = atoi(temp_tahun_str2);
                                if (strcmp(temp_negara2, n_unix[l]) == 0) {
                                    if (temp_tahun2 < 2000) sblm++;
                                    else ssdh++;
                                }
                            }
                            free(temp_copy2);
                        }
                    }
                    fprintf(out, "%d. Negara: %s\n", l + 1, n_unix[l]);
                    fprintf(out, "Film sebelum 2000: %d\n", sblm);
                    fprintf(out, "Film setelah 2000: %d\n\n", ssdh);
                }
            }
        }
        free(copy);
    }
    fclose(out);

    for (int i = 0; i < j_data; i++) free(data[i]);
    printf("done %s\n", filename);
}
```

- membaca semua data dan ambil kolom negara, simpan daftar unik untuk setiap negara, lalu akan membuat file `report`. Ambil negara unik dengan cara loop untuk kolom negara dan lalu creat filenya, dan selanjutnya saya looping untuk menghitung film sebelum dan sesudah 2000 dan di tulis di file

### d. Menu 
Agar penggunaannya semakin mudah, Anthony ingin bisa menjalankan semua proses di atas melalui sebuah antarmuka terminal interaktif dengan pilihan menu seperti berikut:
1. Download File
2. Mengelompokkan Film
3. Membuat Report

```c
int main() {
    int pilihan;
    while (1) {
        printf("\n-=+=- MENU -=+=-\n");
        printf("1. Download File \n");
        printf("2. Mengelompokkan Film \n");
        printf("3. Membuat Report\n");
        printf("4. Keluar\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);
        getchar();

        switch (pilihan) {
            case 1:
                one_click(); break;
            case 2:
                group(); break;
            case 3:
                laporan(); break;
            case 4:
                exit(0);
        }
    }
    return 0;
}
```
- Menampilkan menu untuk pengguna agar bisa memilih proses yang ingin dijalankan

### Hasil OutPut

![hasil](https://github.com/user-attachments/assets/1b8da91d-472c-4deb-84e9-97e0ef4186e8)

![hasil](https://github.com/user-attachments/assets/b365190e-dee3-4152-91fc-e03e8ef24ce7)

![hasil](https://github.com/user-attachments/assets/19dc323d-41b6-4a87-8c5f-460870da0068)

![hasil](https://github.com/user-attachments/assets/046acbe4-f83d-4969-9bdd-8a1cd9437e8e)

![hasil](https://github.com/user-attachments/assets/7bb98f66-6f97-49d2-9b79-d337113b8e89)

![hasil](https://github.com/user-attachments/assets/2de8d352-7988-44d3-8330-9f68e25d1f71)

![hasil](https://github.com/user-attachments/assets/6715dc20-fecb-4c5a-bc89-e44df76a0a58)


# **Cella’s Manhwa**

Cella, si ratu scroll Facebook, tiba-tiba terinspirasi untuk mengumpulkan informasi dan foto dari berbagai **manhwa favoritnya**. Namun, kemampuan ngoding Cella masih cetek, jadi dia butuh bantuanmu untuk membuatkan skrip otomatis agar semua berjalan mulus. Tugasmu adalah membantu Cella mengolah data manhwa dan heroine-nya.

Berikut adalah daftar manhwa bergenre shoujo/josei yang paling disukai Cella:

|    No     |      Manhwa      |
| :--------: | :------------: |
| 1 | Mistaken as the Monster Duke's Wife |
| 2 | The Villainess Lives Again |
| 3 | No, I Only Charmed the Princess! |
| 4 | Darling, Why Can't We Divorce? |

### **a. Summoning the Manhwa Stats**

Cella ingin mengambil data detail dari **manhwa** menggunakan [API Jikan](https://docs.api.jikan.moe/). Informasi yang diambil:

- Judul
- Status
- Tanggal rilis
- Genre
- Tema
- Author

Setelah data berhasil diambil, hasilnya harus disimpan ke dalam file teks, dengan nama file disesuaikan dengan **judul versi bahasa Inggris** (tanpa karakter khusus dan spasi diganti dengan underscore). Semua file teks disimpan dalam folder `Manhwa`.

**Contoh format isi file:**

```
Title: The Villain's Daughter-in-Law
Status: Publishing
Release: 2024-10-16
Genre: Fantasy, Romance
Theme: Time Travel
Author: Na, Reuyan, Kim, Dael
```

**&#128161; Hint**

Contoh Penggunaan Simple API:
1. Kunjungi situs **MyAnimeList**.

2. Cari **manhwa** yang diinginkan.

3. Sebagai contoh:

    `https://myanimelist.net/manga/154063/The_Perks_of_Being_a_Villainess`

3. Gunakan **154063** sebagai **ID** untuk melakukan pengambilan data (scraping).

4. Sehingga **endpoint API** akan menjadi seperti berikut:

    `https://api.jikan.moe/v4/manga/154063`
  
**Jawaban :**

Berikut **ID** dari setiap Manhwa :

   
    168827/Mistaken_as_the_Monster_Dukes_Wife

    147205/The_Villainess_Lives_Again
    
    169731/No_I_Only_Charmed_the_Princess
    
    175521/Darling_Why_Cant_We_Divorce



1. Scraping Data menggunakan external library cJSON
```c
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
    
 
```

2. Formatting data untuk dimasukkan dalam file .txt dan memanggil function `file_name` untuk mengambil judul sesuai perintah *(spasi dan karakter unik dihilangkan)*

```c
 sscanf(from->valuestring, "%*d-%d", release_month); //mengambil data untuk nilai release_month *untuk soal C
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

```
3. Fungsi file_name untuk mengganti karakter spasi dengan ``_``

```c
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

```
### **b. Seal the Scrolls**

Cella ingin agar setiap file `.txt` tadi di-**zip** satu per satu dan disimpan ke dalam folder baru bernama `Archive`. Yang dimana nama masing masing dari zip diambil dari **huruf kapital nama file**.

**Jawaban :**

1. Menggunakan `fork()` dan `execvp` untuk men-zip file ke dalam folder `Archive`, dengan format penamaan menggunakan function `uppercase`
```c
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
```

### **c. Making the Waifu Gallery**

Setiap manhwa memiliki heroine alias **Female Main Character (FMC)**. Cella ingin mengunduh gambar heroine dari internet, dengan jumlah unduhan sesuai dengan **bulan rilis manhwa**.

**Contoh:**

- Jika rilis bulan Februari → unduh **2 foto**
- Jika rilis bulan Desember → unduh **12 foto**
- Format nama file: `Heroine_1.jpg`, `Heroine_2.jpg`, dst.

Selain itu, Cella ingin melakukan pengunduhan **sesuai urutan** daftar manhwa yang tertera pada deskripsi di atas, dan proses pengunduhan harus menggunakan **thread**, karena Cella malas menunggu. Sebagai contohnya, gambar heroine dari manhwa Mistaken as the Monster Duke's Wife harus diunduh terlebih dahulu dan tidak boleh didahului oleh gambar heroine dari manhwa lainnya.

Seluruh gambar akan disimpan dalam folder Heroines. Di dalam folder Heroines, akan terdapat subfolder dengan nama depan atau nama panggilan heroine dari masing-masing manhwa.

Struktur folder yang diinginkan:

```
Heroines/
├── Alisha/
│   ├── Alisha_1.jpg
│   └── Alisha_2.jpg
└── Dorothea/
    ├── Dorothea_1.jpg
    └── Dorothea_2.jpg
```

**Jawaban :**

***Struct pada main berisi nama dan url gambar dari FMC :***
```c
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
``` 



1. Function `get_image` untuk scraping data biner dari foto di internet menggunakan library `curl`
```c
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

```

2. Function `download_image` untuk `forking` process dalam membuat folder dan memasukkan file kedalam folder yang telah dibuat sesuai dengan `release_month` yang telah di scraping di awal. Serta menggunakan `mutex` agar suatu process dapat berjalan hingga selesai tanpa intervensi .
```c
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

```
3. Function `heroines` untuk menjalankan process mengunduh gambar dari internet menggunakan `thread` dan inisialisasi `mutex`
```c
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
```
### **d. Zip. Save. Goodbye**

Setelah semua gambar heroine berhasil diunduh, Cella ingin mengarsipkannya:

- Setiap folder heroine di-zip dengan format:
  ```
  [HURUFKAPITALNAMAMANHWA]_[namaheroine].zip
  ```
- Disimpan di folder `Archive/Images`
- Setelah zip selesai, gambar pada masing masing folder Heroine akan dihapus secara **urut dengan abjad**.

**Jawaban :**

1. Function `heroines_zip` untuk men-zip menggunakan `fork` dengan execvp dan format penamaan huruf besar dengan menggunakan function `uppercase`.
```c
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
```
2. Function `delete_heroines` untuk menghapus isi dari setiap folder heroines dengan sorting agar menghapus sesuai abjad

```c
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
```
### **Fungsi `main` :**
- Berisi struct data heroines yang terdiri dari nama FMC, bulan release, judul, dan url gambar
- `create_directory` untuk `Manhwa`, `Archive`, dan `Images`
```c
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
```
### Hasil Terminal :
![Image](https://github.com/user-attachments/assets/ccf8d7d2-31c8-4689-ae40-9454dc2963bb)

![Image](https://github.com/user-attachments/assets/a6165508-4326-4e60-8198-4f95f1166282)

### Kendala :
- Sempat terkenal `segmentation fault (core dumped)` karena menggunakan `scanf` saat scraping release_month. Solusinya menggunakan `sscanf` agar tidak bufferflow


### Notes &#9888;

- Gunakan `fork()` dan `exec()` untuk soal a, b, dan, d.

- `system()` **HANYA BOLEH DIGUNAKAN (bila ingin)** untuk soal c.

- **Dilarang keras menggunakan external script!**.

- Dilarang menggunakan `mkdir()`.

- Hanya `solver.c` yang dikumpulkan pada GitHub.

---



  # Pipip's Load Balancer

Pipip, seorang pengembang perangkat lunak yang tengah mengerjakan proyek distribusi pesan dengan sistem load balancing, memutuskan untuk merancang sebuah sistem yang memungkinkan pesan dari client bisa disalurkan secara efisien ke beberapa worker. Dengan menggunakan komunikasi antar-proses (IPC), Pipip ingin memastikan bahwa proses pengiriman pesan berjalan mulus dan terorganisir dengan baik, melalui sistem log yang tercatat dengan rapi.

### **a. Client Mengirimkan Pesan ke Load Balancer**
### Soal
Pipip ingin agar proses `client.c` dapat mengirimkan pesan ke `loadbalancer.c` menggunakan IPC dengan metode **shared memory**. Proses pengiriman pesan dilakukan dengan format input dari pengguna sebagai berikut:

```
Halo A;10
```

**Penjelasan:**

- `"Halo A"` adalah isi pesan yang akan dikirim.
- `10` adalah jumlah pesan yang ingin dikirim, dalam hal ini sebanyak 10 kali pesan yang sama.

Selain itu, setiap kali pesan dikirim, proses `client.c` harus menuliskan aktivitasnya ke dalam **`sistem.log`** dengan format:

```
Message from client: <isi pesan>
Message count: <jumlah pesan>
```

Semua pesan yang dikirimkan dari client akan diteruskan ke `loadbalancer.c` untuk diproses lebih lanjut.
### Jawaban
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

typedef struct {
    char isi[256];
    int jum;
    int stat; 
} shared;

int main() {
    char input[300];
    printf("Masukkan pesan dan jumlah\nformat: (pesan;jumlah): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    char *isi = strtok(input, ";");
    char *jum_str = strtok(NULL, ";");

    int jum = atoi(jum_str);

    int shmid = shmget(37, sizeof(shared), IPC_CREAT | 0666);
   

    shared *data = (shared *)shmat(shmid, NULL, 0);
    strncpy(data->isi, isi, 256);
    data->jum = jum;
    data->stat = 1;

    FILE *log = fopen("system.log", "a");
    if (log) {
        fprintf(log, "Message from client: %s\n", isi);
        fprintf(log, "Message count: %d\n", jum);
        fclose(log);
    }

    printf("DONE\n");
    return 0;
}
#
```
### Penjelasan
Struct ini merepresentasikan data yang disimpan di shared memory: isi: pesan teks (maksimal 256 karakter). jum: jumlah (integer). stat: status data (misalnya 1 untuk menandakan data baru tersedia).

```c
typedef struct {
    char isi[256];
    int jum;
    int stat; 
} shared;
```

Program meminta input dengan format pesan;jumlah. fgets() digunakan untuk membaca input dari terminal. strcspn() digunakan untuk menghapus karakter newline (\n).
```c
    char input[300];
    printf("Masukkan pesan dan jumlah\nformat: (pesan;jumlah): ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;
```

strtok() digunakan untuk memisahkan input berdasarkan tanda ;. Bagian pertama (isi) adalah pesan, bagian kedua (jum_str) adalah jumlah.
```c
    char *isi = strtok(input, ";");
    char *jum_str = strtok(NULL, ";");
```
atoi() mengubah string jum_str menjadi integer. shmget() membuat atau mengakses shared memory dengan key 37.


```c
    int jum = atoi(jum_str);

    int shmid = shmget(37, sizeof(shared), IPC_CREAT | 0666);
```
shmat() menyambungkan program ke shared memory. Isi data (isi, jum, stat) ditulis ke shared memory. stat = 1 digunakan untuk menandai bahwa data baru telah dikirim.
```c
    shared *data = (shared *)shmat(shmid, NULL, 0);
    strncpy(data->isi, isi, 256);
    data->jum = jum;
    data->stat = 1;
```
Pesan dan jumlah dicatat ke file system.log untuk kebutuhan debugging atau dokumentasi. Jika berhasil print DONE
```c
    FILE *log = fopen("system.log", "a");
    if (log) {
        fprintf(log, "Message from client: %s\n", isi);
        fprintf(log, "Message count: %d\n", jum);
        fclose(log);
    }

    printf("DONE\n");
    return 0;
}
```
### Hasil Pada Terminal
![Image](https://github.com/user-attachments/assets/ac7df1e0-52b7-4c0d-8fe0-a36343d03538)

### Terciptanya System.log
![Image](https://github.com/user-attachments/assets/028acfee-6809-4127-9e5d-1908d1bc721d)

### Isi dari System.log
![Image](https://github.com/user-attachments/assets/30c649de-774f-4db4-92b0-33092839be49)

### **b. Load Balancer Mendistribusikan Pesan ke Worker Secara Round-Robin**
### Soal
Setelah menerima pesan dari client, tugas `loadbalancer.c` adalah mendistribusikan pesan-pesan tersebut ke beberapa **worker** menggunakan metode **round-robin**. Sebelum mendistribusikan pesan, `loadbalancer.c` terlebih dahulu mencatat informasi ke dalam **`sistem.log`** dengan format:

```
Received at lb: <isi pesan> (#message <indeks pesan>)
```

Contoh jika ada 10 pesan yang dikirimkan, maka output log yang dihasilkan adalah:

```
Received at lb: Halo A (#message 1)
Received at lb: Halo A (#message 2)
...
Received at lb: Halo A (#message 10)
```

Setelah itu, `loadbalancer.c` akan meneruskan pesan-pesan tersebut ke **n worker** secara bergiliran (round-robin), menggunakan **IPC message queue**. Berikut adalah contoh distribusi jika jumlah worker adalah 3:

- Pesan 1 → worker1
- Pesan 2 → worker2
- Pesan 3 → worker3
- Pesan 4 → worker1 (diulang dari awal)

Dan seterusnya.

Proses `worker.c` bertugas untuk mengeksekusi pesan yang diterima dan mencatat log ke dalam file yang sama, yakni **`sistem.log`**.
### Jawaban
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <unistd.h>

typedef struct {
    char isi[256];
    int jum;
    int stat;
} shared;

typedef struct {
    long bruh;
    char isi[256];
} mes;

int main() {
    int jum_worker;
    printf("jumlah worker: ");
    scanf("%d", &jum_worker);

    int que[jum_worker];
    for (int i = 0; i < jum_worker; i++) {
        key_t key = 73 + i;
        que[i] = msgget(key, IPC_CREAT | 0666);
        
    }
    int shmid = shmget(37, sizeof(shared), 0666);
    shared *data = (shared *)shmat(shmid, NULL, 0);
    FILE *log = fopen("system.log", "a");

    int giliran = 0;
    while (1) {
        if (data->stat == 1) {
            for (int i = 0; i < data->jum; i++) {
                if (log) {
                    fprintf(log, "Received at lb: %s (#mes %d)\n", data->isi, i + 1);
                    fflush(log);
                }

                mes msg;
                msg.bruh = 1;
                snprintf(msg.isi, sizeof(msg.isi), "%s", data->isi);
                msgsnd(que[giliran], &msg, sizeof(msg.isi), 0);
                giliran = (giliran + 1) % jum_worker;
                usleep(10000); 
            }
            data->stat = 0;
            if (log) fclose(log);
            break;
        }
        usleep(50000);
    }

    return 0;
}

```
### Penjelasan
shared: struct untuk data yang dikirim melalui shared memory (sama seperti di program client). mes: struct untuk data yang dikirim melalui message queue. Field bruh (biasanya mtype) adalah tipe pesan (dibutuhkan oleh sistem message queue).

```c
typedef struct {
    char isi[256];
    int jum;
    int stat;
} shared;

typedef struct {
    long bruh;
    char isi[256];
} mes;
```
Pengguna diminta memasukkan jumlah worker. Untuk tiap worker, dibuat message queue dengan key 73 + i (unik untuk setiap worker). msgget akan membuat message queue jika belum ada.
```c
    int jum_worker;
    printf("jumlah worker: ");
    scanf("%d", &jum_worker);

    int que[jum_worker];
    for (int i = 0; i < jum_worker; i++) {
        key_t key = 73 + i;
        que[i] = msgget(key, IPC_CREAT | 0666);
        
    }
```
Program mengakses shared memory (key 37) yang sebelumnya telah diisi oleh program client.
```c
    int shmid = shmget(37, sizeof(shared), 0666);
    shared *data = (shared *)shmat(shmid, NULL, 0);
```
File system.log dibuka untuk mencatat pesan-pesan yang diterima dan dikirim.
```c
    FILE *log = fopen("system.log", "a");
```
Program terus memantau shared memory. Saat stat == 1, artinya ada data baru yang siap dikirim. Loop for akan mengirim sebanyak jum pesan sesuai jumlah yang dikirim client.
```c
    int giliran = 0;
    while (1) {
        if (data->stat == 1) {
            for (int i = 0; i < data->jum; i++) {
```
Membuat pesan dengan isi yang diambil dari shared memory. msgsnd() mengirimkan pesan ke message queue sesuai urutan giliran. Round-robin digunakan: pesan dikirim ke worker 0, lalu 1, lalu 2, ... dan kembali ke 0 lagi. usleep(10000) memberi jeda kecil agar proses stabil.
```c
                mes msg;
                msg.bruh = 1;
                snprintf(msg.isi, sizeof(msg.isi), "%s", data->isi);
                msgsnd(que[giliran], &msg, sizeof(msg.isi), 0);
                giliran = (giliran + 1) % jum_worker;
                usleep(10000); 
```
Setelah selesai mengirim semua pesan, shared memory di-reset (stat = 0) untuk menandakan bahwa tidak ada data baru. File log ditutup dan program keluar dari loop.      
```c
            data->stat = 0;
            if (log) fclose(log);
            break;
```

### Output Terminal
![Image](https://github.com/user-attachments/assets/11f606fc-1028-4bfe-b5c8-d8b54b10c5b6)

### Update System.log
![Image](https://github.com/user-attachments/assets/c52d77a4-c8bb-46fa-84e5-7634e0125b2a)

### **c. Worker Mencatat Pesan yang Diterima**

Setiap worker yang menerima pesan dari `loadbalancer.c` harus mencatat pesan yang diterima ke dalam **`sistem.log`** dengan format log sebagai berikut:

```
WorkerX: message received
```

### **d. Catat Total Pesan yang Diterima Setiap Worker di Akhir Eksekusi**

Setelah proses selesai (semua pesan sudah diproses), setiap worker akan mencatat jumlah total pesan yang mereka terima ke bagian akhir file **`sistem.log`**.

```
Worker 1: 3 messages
Worker 2: 4 messages
Worker 3: 3 messages
```

**Penjelasan:**
3 + 4 + 3 = 10, sesuai dengan jumlah pesan yang dikirim pada soal a

---
### Worker.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

typedef struct {
    long jenis;
    char isi[256];
} Message;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Format: ./worker <nomor_worker>\n");
        return 1;
    }

    int id_worker = atoi(argv[1]);
    key_t key = 73 + (id_worker - 1);
    int msgid = msgget(key, 0666);

    FILE *log = fopen("system.log", "a");

    Message msg;
    int counter = 0;

    while (1) {
        ssize_t result = msgrcv(msgid, &msg, sizeof(msg.isi), 0, IPC_NOWAIT);
        if (result > 0) {
            fprintf(log, "Worker%d: message received\n", id_worker);
            fflush(log);
            counter++;
        } else {
            if (counter > 0) break; 
            usleep(100000); 
        }
    }

    fprintf(log, "Worker %d: %d messages\n", id_worker, counter);
    fclose(log);
    return 0;
}

```
### Penjelasan

Struct Message digunakan untuk menerima data dari message queue. jenis (mtype): jenis atau tipe pesan (harus bertipe long). isi: isi pesan (maksimal 256 karakter)

```c
typedef struct {
    long jenis;
    char isi[256];
} Message;
```

Program harus dijalankan dengan argumen satu angka, yaitu nomor worker (misalnya ./worker 1).
```c
    if (argc != 2) {
        printf("Format: ./worker <nomor_worker>\n");
        return 1;
    }
```
ID worker diubah menjadi integer. Key message queue disesuaikan dengan urutan yang dibuat load balancer (73 + index). msgget() mengakses message queue berdasarkan key.
```c
int id_worker = atoi(argv[1]);
key_t key = 73 + (id_worker - 1);
int msgid = msgget(key, 0666);
```
Membuka log file untuk mencatat aktivitas worker. counter akan menghitung jumlah pesan yang diterima.
```c
FILE *log = fopen("system.log", "a");
Message msg;
int counter = 0;
```
msgrcv() menerima pesan dari message queue. IPC_NOWAIT berarti program tidak akan menunggu jika tidak ada pesan.
```c
    while (1) {
        ssize_t result = msgrcv(msgid, &msg, sizeof(msg.isi), 0, IPC_NOWAIT);
```
Jika pesan diterima, worker mencatatnya di log dan menaikkan counter.
```c
        if (result > 0) {
            fprintf(log, "Worker%d: message received\n", id_worker);
            fflush(log);
            counter++;
```
Jika belum ada pesan yang diterima (counter == 0), worker menunggu sebentar lalu cek ulang. Setelah menerima minimal satu pesan, jika sudah tidak ada lagi, worker keluar dari loop.
```c
        } else {
            if (counter > 0) break; 
            usleep(100000); 
        }
    }
```
Mencatat total pesan yang diterima sebelum worker selesai.
```c
    fprintf(log, "Worker %d: %d messages\n", id_worker, counter);
    fclose(log);
```
### Output Terminal
![Image](https://github.com/user-attachments/assets/7e293e07-fb53-4037-a34e-21845a6dc6cf)

### Update System.log
![Image](https://github.com/user-attachments/assets/6dae0bcb-e289-4e80-a611-869546136bfc)

    

### Laporan Resmi Praktikum Modul 2 _(Module 2 Lab Work Report)_

Tulis laporan resmi di sini!

_Write your lab work report here!_
