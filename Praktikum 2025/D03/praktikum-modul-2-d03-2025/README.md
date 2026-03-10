[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/9LcL5VTQ)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241068 | Riyan Fadli Amazzadin |
| 5025241100 | Liem, Alfred Haryanto |
| 5025221120 | Raditya Zhafran Pranuja |

# Praktikum Modul 2 _(Module 2 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - Trabowo & Peddy Movie Night](/task-1/)

- [Task 2 - Organize and Analyze Anthony's Favorite Films](/task-2/)

- [Task 3 - Cella’s Manhwa](/task-3/)

- [Task 4 - Pipip's Load Balancer](/task-4/)

### Laporan Resmi Praktikum Modul 2 _(Module 2 Lab Work Report)_

### 1. Trabowo and Peddy Movie Night
a. Trabowo langsung mendownload file ZIP tersebut dan menyimpannya di penyimpanan lokal komputernya. Namun, karena file tersebut dalam bentuk ZIP, Trabowo perlu melakukan unzip agar dapat melihat daftar film-film seru yang ada di dalamnya.

code :
``` c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();

    //Bagian 1
    if(pid == 0){
        char *args[] = {"wget", "-O", "film.zip", "https://drive.google.com/uc?export=download&id=1nP5kjCi9ReDk5ILgnM7UCnrQwFH67Z9B", NULL};
        execv("/usr/bin/wget", args);
    } else {
    //Bagian 2
        int status;
        wait(&status);
        if(WIFEXITED(status)){
            char *argv[] = {"unzip", "-q", "film.zip", NULL};
            execv("/usr/bin/unzip", argv);
        }
    }
    return 0;
}

```
Penjelasan :
- Untuk menjawab soal A, saya menggunakan parent and child process untuk menjalankan 2 eksekusi yang berbeda.
- `Bagian 1` : Bagian 1 atau child ini bertugas untuk melakukan download file yang diperlukan. Proses download dilakukan dengan menggunakan `execv` dengan command `wget`beserta kebutuhanya (format output, sumber file).
- `Bagian 2` : Bagian 2 atau parent bertugas untuk melakukan unzip dari file yang sudah didownload. Agar parent tidak melakukan unzip sebelum child selesai mendownload, maka dapat menggunakan `wait(&status)` untuk memastikan process child selesai mendownload baru parent melakukan unzip.
- Note : Link gdrive harus diubah sedikit dari yang awalnya `https://drive.google.com/file/d/'idFile'/view` menjadi `https://drive.google.com/uc?export=download&id=/'idFile'`, singkatnya mengubah format dengan menggunakan `uc?export=download&id=`. Hal ini harus dilakukan karena link yang pertama adalah link website (untuk user mendownload file), dan bukan link ke file tersebut. Agar dapat didownload menggunakan wget, maka link tersebut harus link yang mengarah ke file yang ingin di download.

Setelah program dijalankan :

![Alt text](https://media.discordapp.net/attachments/1123949811020873759/1367128794053349486/image.png?ex=68137568&is=681223e8&hm=573d3e3ab731ef34cd3071c4115a79bd35cb9211160fb4fe7b850f85e62884fb&=&format=webp&quality=lossless&width=834&height=686)

![Alt text](https://media.discordapp.net/attachments/1123949811020873759/1367128917944570006/image.png?ex=68137586&is=68122406&hm=081a9048d6fae0cc62d9c8c834441fc8b4d8928e53e9522e4afb122c0fb463d4&=&format=webp&quality=lossless&width=780&height=1016)

b. Setelah berhasil melakukan unzip, Trabowo iseng melakukan pemilihan secara acak/random pada gambar-gambar film tersebut untuk menentukan film pertama yang akan dia tonton malam ini. Format output : `Film for Trabowo & Peddy: ‘<no_namafilm_genre.jpg>’`.

code :
``` c
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
    //Bagian 1
    DIR * dir;
    char *name[101];
    struct dirent *entry;
    dir = opendir("/home/freddskii/film");

    //Bagian 2.0
    int i = 0;
    while((entry = readdir(dir)) != NULL && i < 101 ){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        name[i] = strdup(entry->d_name); i++;
    }

    //Bagian 2.1
    for(int j = 0 ; j < i ; j++){
        for(int k = 0 ; k < i-1 ; k++){
            if(extract_number(name[k]) > extract_number(name[k+1])){
                char *temp = name[k];
                name[k] = name[k+1];
                name[k+1] = temp;
            }
        }
    }

    //Bagian 3
    srand(time(NULL));
    int ran = rand() % i;
    printf("Film for Trabowo & Peddy: %s\n", name[ran]);
    closedir(dir);

    return 0;
}
```

Penjelasan :
- Karena hasil unzip dari file `netflixData.csv` berada dalam sebuah folder, disini saya menggunakan `readdir` untuk membaca file dalam folder tersebut.
- `Bagian 1` : Bagian 1 ini melakukan setup untuk menyiapkan variabel untuk menyimpan nama file (`name`), `dir` untuk membuka folder film, dan `entry` sebagai struct yang nantinya akan menyimpan data dari `readdir(dir)`. Penggunaan struct disini cukup krusial karena readdir akan mengambil tidak hanya satu data, melainkan beberapa data meliputi nama, tipe file, inode number, dan semacamnya.
- `Bagian 2.0` : While loop digunakan untuk menyimpan semua nama file kedalam variabel `name`. Variabel i digunakan untuk menghitung jumlah file yang ada tersimpan dalam variabel `name`. If statement pada while loop berguna untuk memfilter special entries, agar file `.` dan `..` tidak ikut masuk kedalam variabel `name`.
- `Bagian 2.1` : Bagian ini digunakan untuk melakukan sort nama file pada variabel `nama`. Sort ini akan berguna lebih lanjut untuk soal C.
- `Bagian 3` : Bagian 3 berguna untuk melakukan membuat seed acak. Seed acak tersebut nanti akan dimodulo dengan `i` (jumlah judul film) agar jumlah index sesuai dengan file. Index tersebut yang nantinya digunakan untuk memilih judul film secara acak.
- `extract_number` : Function extract number berguna untuk mengambil angka dari judul film untuk keperluan sorting. Sorting untul judul file menggunakan bubble sort, apabila langsung membandingkan variabel seperti `name[i]` dengan `name[i+1]`, yang akan dibandingkan adalah ASCII value dari nama file tersebut, bukan angkanya. Dengan menggunakan `sscanf` dan menyimpan angka yang diambil kedalam variabel `number`, nilai yang akan di return hanyalah angka sehingga sorting judul dapat berjalan normal.

Setelah program dijalankan :

![Alt text](https://media.discordapp.net/attachments/1123949811020873759/1367128870414717029/image.png?ex=6813757a&is=681223fa&hm=ad3315720a8e5e52cc074fead4c3ba5419b07b53c91c62c301c26f538cbb29fb&=&format=webp&quality=lossless&width=2012&height=686)

*notes : apabila program dijalankan secara cepat kemungkinan akan ada selang waktu dimana film yang dipilih secara random memiliki hasil yang sama

c. Karena Trabowo sangat perfeksionis dan ingin semuanya tertata rapi, dia memutuskan untuk mengorganisir film-film tersebut berdasarkan genre. Dia membuat 3 direktori utama di dalam folder ~/film, yaitu:

`FilmHorror`
`FilmAnimasi`
`FilmDrama`

Setelah itu, dia mulai memindahkan gambar-gambar film ke dalam folder yang sesuai dengan genrenya. Tetapi Trabowo terlalu tua untuk melakukannya sendiri, sehingga ia meminta bantuan Peddy untuk memindahkannya. Mereka membagi tugas secara efisien dengan mengerjakannya secara bersamaan (overlapping) dan membaginya sama banyak. Trabowo akan mengerjakan dari awal, sementara Peddy dari akhir. Misalnya, jika ada 10 gambar, Trabowo akan memulai dari gambar pertama, gambar kedua, dst dan Peddy akan memulai dari gambar kesepuluh, gambar kesembilan, dst. Lalu buatlah file “recap.txt” yang menyimpan log setiap kali mereka selesai melakukan task

Format log :
```
[15-04-2025 13:44:59] Peddy: 50_toystory_animasi.jpg telah dipindahkan ke FilmAnimasi
```

Setelah memindahkan semua film, Trabowo dan Peddy juga perlu menghitung jumlah film dalam setiap kategori dan menuliskannya dalam file `total.txt`. Format dari file tersebut adalah:
```
Jumlah film horror: <jumlahfilm>
Jumlah film animasi: <jumlahfilm>
Jumlah film drama: <jumlahfilm>
Genre dengan jumlah film terbanyak: <namagenre>
```

code `main` :
``` c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

typedef struct{
    int isTrabowo;
    char *name, *loc;
} fileTask;

int number(const char *fileName){
    int num;
    sscanf(fileName, "%d", &num);
    return num;
}

int main(){
    char path[] = "/home/freddskii/film";
    char *name[100];
    struct dirent *entry;
    pthread_t threads[100];

    //Bagian 1
    DIR *directory = opendir(path);
    int i = 0;
    while((entry = readdir(directory)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, "recap.txt") == 0 || strcmp(entry->d_name, "total.txt") == 0) continue;
        name[i] = strdup(entry->d_name);
        i++;
    }
    closedir(directory);

    //ini hanya untuk membuat recap.txt
    char initLog[200];
    sprintf(initLog, "%s/recap.txt", path);
    FILE *logFile = fopen(initLog, "w");
    if(logFile) fclose(logFile);

    for(int j = 0 ; j < i ; j++){
        for(int k = 0 ; k < i-1 ; k++){
            if(number(name[k]) > number(name[k+1])){
                char *temp = name[k];
                name[k] = name[k+1];
                name[k+1] = temp;
            }
        }
    }

    //Bagian 2
    int indexTrab = 0, indexPedd = i - 1;
    for(int j = 0; j < i/2; j++){
        fileTask *task1 = (fileTask *)malloc(sizeof(fileTask));
        task1->name = strdup(name[j]);
        task1->loc = path;
        task1->isTrabowo = 1;
        if(pthread_create(&threads[j], NULL, worker, task1) == 0) pthread_join(threads[j], NULL);

        fileTask *task2 = (fileTask *)malloc(sizeof(fileTask));
        task2->name = strdup(name[i-j-1]);
        task2->loc = path;
        task2->isTrabowo = 0;
        if(pthread_create(&threads[i - j - 1], NULL, worker, task2) == 0) pthread_join(threads[j], NULL);
    }

    //Ini hanya untuk membuat total.txt
    char totalPath[200];
    sprintf(totalPath, "%s/total.txt", path);
    FILE *totalFile = fopen(totalPath, "w");

    //Bagian 3
    const char *maxGenre;
    int maxCount = 0;

    if(countHorror > maxCount){maxCount = countHorror; maxGenre = "Horror";}
    if(countAnimasi > maxCount){maxCount = countAnimasi; maxGenre = "Animasi";}
    if(countDrama > maxCount){maxCount = countDrama; maxGenre = "Drama";}

    fprintf(totalFile, "Jumlah film animasi: %d\n", countAnimasi);
    fprintf(totalFile, "Jumlah film drama: %d\n", countDrama);
    fprintf(totalFile, "Jumlah film horror: %d\n", countHorror);
    fprintf(totalFile, "Genre dengan jumlah film terbanyak: %s\n", maxGenre);
    fclose(totalFile);
    return 0;
}
```
Penjelasan :
- Karena hasil unzip dari file berada dalam sebuah folder, maka untuk mengakses folder tersebut saya menggunakan `dirent.h`.
- `Bagian 1` : Bagian 1 memiliki algoritma yang sama dengan soal B. Terdapat sedikit perbedaan, yakni pada if statement yang sekarang juga menambahkan `recap.txt` dan `total.txt` agar tidak masuk ke variabel `name`. Kemudian, nama file akan di sort (agar sesuai dengan ketentuan soal)
- `Bagian 2` : Pada soal diminta agar Trabowo mengerjakan mulai dari file pertama, kemudian Peddy dari file terakhir, yang mana nantinya mereka berdua akan bertemu di tengah. Untuk data film yang nantinya akan diolah (judul, lokasi, pekerja) akan disimpan di struct yang dibuat untuk masing2 pekerja. Variabel `isTrabowo` berguna untuk menandakan siapa pekerja (pemindah file). Karena kriteria soal dimana Trabowo dan Peddy harus bekerja secara bergantian dan urut sesuai dengan judul, `pthread_join` langsung digunakan agar proses berjalan secara sekuensial (agar pekerjaan dilakukan selang seling, dan sesuai dengan urutan).
- `Bagian 3` : Bagian 3 ini berguna untuk melakukan rekap jumlah film tiap genre, dan menentukan genre dengan jumlah film terbanyak. Setelah mendapatkan jumlah film tiap genre dan genre dengan jumlah film terbanyak, maka data tersebut akan disimpan kedalam total.txt menggunakan `fprintf`.

Function `worker`
``` c
void* worker(void *arg){
    fileTask *task = (fileTask *)arg;
    char *genre = NULL;
    char old[200], newPath[200];

    //Bagian 1
    if(strstr(task->name, "horror")) genre = "FilmHorror";
    else if(strstr(task->name, "animasi")) genre = "FilmAnimasi";
    else if(strstr(task->name, "drama")) genre = "FilmDrama";

    //Bagian 2
    if(genre != NULL){
        //Bagian 2.1
        sprintf(newPath, "%s/%s", task->loc, genre);
        mkdir(newPath, 0777);

        //Bagian 2.2
        sprintf(old, "%s/%s", task->loc, task->name);
        sprintf(newPath, "%s/%s/%s", task->loc, genre, task->name);
        rename(old, newPath);

        //Bagian 2.3
        const char* worker;
        if(task->isTrabowo) worker = "Trabowo";
        else worker = "Peddy";
        log(task->loc, worker, task->name, genre);
        genreCount(genre);
    }
    return NULL;
}
```

Penjelasan :
- `Bagian 1` : Berguna untuk mengecek genre dari judul film yang sedang diproses. Untuk mengambil genre dapat menggunakan `strstr`. `strstr` ini memiliki sifat yang sama dengan grep pada shell scripting, dimana strstr akan mencari suatu kata dalam sebuah string. Bila kata tersebut ditemukan, akan mengembalikan nilai 1 (true) yang akan menjalankan if statement untuk menentukan genre dari film tersebut.
- `Bagian 2` : Bagian 2 ini adalah proses dimana file akan diolah dan dipindahkan berdasarkan genre.
- `Bagian 2.1` : Bagian ini digunakan untuk membuat sub-directory berdasarkan genre, dengan meletakkan path nya pada variabel `newPath`, dan menggunakan `mkdir` untuk membuat sub-directory nya. `0777` adalah izin agar semua user dapat mengakses sub-directory tersebut. Apabila sub-directory sudah ada, mkdir tidak akan meng-overwrite.
- `Bagian 2.2` : Untuk memindahkan file, disini saya menggunakan `rename`, yang mana untuk memindahkan file, rename membutuhkan old path dan new path dari file yang ingin dipindahkan. Oleh karena itu, dibuatlah variabel old yang berisi path sebelum dipindahkan, dan newPath yang berisi path dimana file akan dipindahkan.
- `Bagian 2.3` : Bagian ini bertugas untuk mengecek siapa pekerja/worker yang sedang memindahkan file yang sedang diolah, variabel isTrabowo didalam struct bertugas sebagai flag untuk menentukan sang pekerja (sudah ditentukan di fungsi main, pada bagian 2). Setelah menentukan pekerja, kemudian memanggil function `log` dan mengupdate jumlah genre dengan finction `genreCount`.

function `log`
``` c
void log(const char *path, const char *worker, const char *fileName, const char *genre){
    //Bagian 1
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time[20], log[200];
    strftime(time, sizeof(time), "%d-%m-%Y %H:%M:%S", t);
    sprintf(log, "%s/recap.txt", path);

    //Bagian 2
    pthread_mutex_lock(&file);
    FILE *logFile = fopen(log, "a");
    if(logFile){
        fprintf(logFile, "[%s] %s: %s telah dipindahkan ke %s\n", time, worker, fileName, genre);
        fclose(logFile);
    }
    pthread_mutex_unlock(&file);
}

```

Penjelasan :
- `Bagian 1` : Bertugas untuk mengambil waktu sekarang dan meletakkan waktu sesuai dengan format yang diinginkan pada variabel time. Kemudian, membuat file `recap.txt`.
- `Bagian 2` : Bagian ini bertugas untuk menulis log kedalam `recap.txt` dengan format `[15-04-2025 13:44:59] Peddy: 50_toystory_animasi.jpg telah dipindahkan ke FilmAnimasi`. Karena penulisan kedalam file merupakan suatu proses yang critial, maka saya menggunakan mutex lock agar ketika 1 proses melakukan write pada `recap.txt`, tidak ada proses lain yang menganggu. Setelah proses write selesai, unlock mutex agar proses lain dapat melakukan write juga.

Function `genreCount`
``` c
//ini global variable
int countHorror = 0, countAnimasi = 0, countDrama = 0;

void genreCount(const char *genre){
    pthread_mutex_lock(&count);
    if(strcmp(genre, "FilmHorror") == 0) countHorror++;
    else if(strcmp(genre, "FilmAnimasi") == 0) countAnimasi++;
    else if(strcmp(genre, "FilmDrama") == 0) countDrama++;
    pthread_mutex_unlock(&count);
}
```

Penjelasan :
- Untuk function ini hanya menghitung/menambah jumlah film dari tiap genre sesuai dengan parameter yang diberi dari function `worker`. Membandingkan string yang dikirim dapat menggunakan `strcmp`, bila genre film sama dengan salah satu if statement, maka count genre tersebut akan bertambah.


Setelah program dijalankan :

![Alt text](https://media.discordapp.net/attachments/1123949811020873759/1367129011741917264/image.png?ex=6813759c&is=6812241c&hm=84b9c58f1b8643c66d4816a6c83a305a602b1902938beaae1c261a009e710d85&=&format=webp&quality=lossless&width=752&height=438)

![Alt text](https://media.discordapp.net/attachments/1123949811020873759/1367129032205930567/image.png?ex=681375a1&is=68122421&hm=9d440156f28d3b10183e27fb6e09eecffe5da277a4813db29238a7ad28ec9ade&=&format=webp&quality=lossless&width=734&height=1052)

Contoh `recap.txt`
```
[30-04-2025 20:22:14] Trabowo: 1_businessproposal_drama.jpg telah dipindahkan ke FilmDrama
[30-04-2025 20:22:14] Peddy: 50_toystory_animasi.jpg telah dipindahkan ke FilmAnimasi
[30-04-2025 20:22:14] Trabowo: 2_coco_animasi.jpg telah dipindahkan ke FilmAnimasi
[30-04-2025 20:22:14] Peddy: 49_purplehearts_drama.jpg telah dipindahkan ke FilmDrama
[30-04-2025 20:22:14] Trabowo: 3_dilan_drama.jpg telah dipindahkan ke FilmDrama
[30-04-2025 20:22:14] Peddy: 48_monsterinc_animasi.jpg telah dipindahkan ke FilmAnimasi
[30-04-2025 20:22:14] Trabowo: 4_insideout2_animasi.jpg telah dipindahkan ke FilmAnimasi
[30-04-2025 20:22:14] Peddy: 47_megan_horror.jpg telah dipindahkan ke FilmHorror
[30-04-2025 20:22:14] Trabowo: 5_iparadalahmaut_drama.jpg telah dipindahkan ke FilmDrama
[30-04-2025 20:22:14] Peddy: 46_liloandstitch_animasi.jpg telah dipindahkan ke FilmAnimasi
[30-04-2025 20:22:14] Trabowo: 6_jumbo_animasi.jpg telah dipindahkan ke FilmAnimasi
[30-04-2025 20:22:14] Peddy: 45_lalaland_drama.jpg telah dipindahkan ke FilmDrama
```

Contoh `total.txt`
```
Jumlah film animasi: 15
Jumlah film drama: 21
Jumlah film horror: 14
Genre dengan jumlah film terbanyak: Drama
```

d. Setelah semua film tertata dengan rapi dan dikelompokkan dalam direktori masing-masing berdasarkan genre, Trabowo ingin mengarsipkan ketiga direktori tersebut ke dalam format ZIP agar tidak memakan terlalu banyak ruang di komputernya.

code :
``` c
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

    //Bagian 1
    int i = 0;
    while((entry = readdir(dir)) != NULL && i < 101 ){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strstr(entry->d_name, ".txt") != NULL) continue;
        name[i] = strdup(entry->d_name); i++;
    }

    char path[] = "/home/freddskii/film";
    for(int k = 0; k < i; k++){
        pid_t pid = fork();

        //Bagian 2.1
        if(pid == 0){
            char zipPath[512];
            snprintf(zipPath, sizeof(zipPath), "%s/%s.zip", path, name[k]);

            char *args[] = {"zip", "-r", zipPath, name[k], NULL};
            chdir(path);
            execvp("zip", args);
        }else{
            //Bagian 2.2
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
```
Penjelasan :
- Proses melakukan zip folder dan menghapusnya menggunakan parent and child process.
- `Bagian 1` : Bagian ini bertugas untuk mengambil nama-nama file (folder dalam kasus ini) yang akan di zip, dengan menghiraukan file  `recap.txt` dan `total.txt`.
- `Bagian 2.1` : Parent and child process dilakukan didalam for loop sejumlah file yang ingin di zip (3 kali untuk 3 folder). Child process bertugas untuk melakukan zip pada setiap folder.
- `Bagian 2.2` : Parent akan menunggu child untuk selesai melakukan zip, kemudian menghapus folder yang sudah di zip agar yang tersisa hanya file yang sudah di zip dan `recap.txt`, dan `total.txt`.

Setelah program dijalankan :

![Alt text](https://media.discordapp.net/attachments/1123949811020873759/1367129169967976560/image.png?ex=681375c2&is=68122442&hm=b456439fcc0f375bcb2b97d91ecb5b5fd45c68ca19cd6e459b5673ce15cb0f1e&=&format=webp&quality=lossless&width=786&height=428)

### 2. Trabowo and Peddy Movie Night
x. Agar penggunaannya semakin mudah, Anthony ingin bisa menjalankan semua proses di atas melalui sebuah antarmuka terminal interaktif dengan pilihan menu seperti berikut:

1. Download File
2. Mengelompokkan Film
3. Membuat Report

Function `main`
``` c
int main(){
    int a;
    char *path = "/home/freddskii/netflixData.csv";

    while(a != 4){
        printf("1. Download file\n");
        printf("2. Sorting film\n");
        printf("3. Ultimate report\n");
        printf("4. Exit\n");
        printf("Input : ");
        scanf("%d", &a);

        switch(a){
            case 1 :
                download();
                break;
            case 2 :
                int stat;
                pid_t pid1, pid2;

                pid1 = fork();
                if(pid1 == 0){category(path, "judul"); exit(0);}
                pid2 = fork();
                if(pid2 == 0){category(path, "tahun"); exit(0);}

                wait(&stat);
                wait(&stat);
                break;
            case 3 :
                report(path);
                break;
            case 4 :
                return 0;
        }
    }
}
```

Penjelasan :
- Untuk membuat UI, saya menggunakan while loop, dimana selama input dari user bukan 4 (4 untuk exit), maka program akan terus berjalan.

Setelah program dijalankan :

![Alt text](https://cdn.discordapp.com/attachments/1123949811020873759/1367129255644893204/image.png?ex=681375d6&is=68122456&hm=ba9998cf37ce2022e6b40de2717f6b566ef5d4c62a728f79bb874a7b41d909c3&format=webp&quality=lossless&width=1400&height=402)

a. Pernahkah kamu merasa malas untuk mengelola file ZIP yang penuh dengan data film? Anthony merasa hal yang sama, jadi dia ingin semuanya serba instan dengan hanya satu perintah. Dengan satu perintah saja, Anthony bisa:

- Mendownload file ZIP yang berisi data film-film Netflix favoritnya.
- Mengekstrak file ZIP tersebut ke dalam folder yang sudah terorganisir.
- Menghapus file ZIP yang sudah tidak diperlukan lagi, supaya tidak memenuhi penyimpanan.


Buatlah skrip yang akan mengotomatiskan proses ini sehingga Anthony hanya perlu menjalankan satu perintah untuk mengunduh, mengekstrak, dan menghapus file ZIP.

code :
``` c
void download(){
    //Bagian 1
    pid_t pid = fork();
    if(pid == 0){
        char *args[] = {"wget", "-O", "netflixData.zip", "https://drive.google.com/uc?export=download&id=12GWsZbSH858h2HExP3x4DfWZB1jLdV-J", NULL};
        execv("/usr/bin/wget", args);
    } else {
        //Bagian 2.0
        int status;
        wait(&status);
        if(WIFEXITED(status)){
            pid_t unzip_pid = fork();

            //Bagian 2.1
            if(unzip_pid == 0){
                char *argv[] = {"unzip", "-q", "netflixData.zip", NULL};
                execv("/usr/bin/unzip", argv);
            }else{
            // Bagian 2.2
                wait(&status);
                if(WIFEXITED(status)) remove("netflixData.zip");
            }
        }
    }
}
```

Penjelasan :
- Soal 2a memiliki cara kerja yang hampir sama dengan nomor 1a, yang membedakan adalah di nomor 2 ini harus menghapus file zip nya. Dengan menggunakan parent and child process, dan sedikit modifikasi pada bagian parent.
- `Bagian 1` :  Child process ini bertugas untuk mendownload file dari google drive dengan link google drive yang sudah dimodifikasi.
- `Bagian 2.0` : Parent akan menunggu child selesai mendownload, untuk menghindari segmentation fault. Bila child sudah selesai, maka parent and child process dalam parent akan dieksekusi.
- `Bagian 2.1` : Child process yang kedua akan bertugas untuk melakukan unzip kepada file yang di download oleh child utama. Proses unzip dengan menggunakan `execv` dengan command untuk melakukan unzip.
- `Bagian 2.2` : Parent process yang kedua ini akan menunggu childnya untuk selesai melakukan unzip. Bila child sudah selesai, maka parent akan menjalankan tugasnya untuk menghapus file zip yang pertama kali di download.

Setelah program dijalankan :

![Alt text](https://media.discordapp.net/attachments/1123949811020873759/1367129317808672808/image.png?ex=681375e5&is=68122465&hm=827d222593d5c4c4c36baf2796892fc0f49d4b301a5ff3a8a5c0b18df9e264c3&=&format=webp&quality=lossless&width=618&height=282)

b. Koleksi film Anthony semakin banyak dan dia mulai bingung mencari cara yang cepat untuk mengelompokkannya. Nah, Anthony ingin mengelompokkan film-filmnya dengan dua cara yang sangat mudah:

Berdasarkan huruf pertama dari judul film.
Berdasarkan tahun rilis (release year).
Namun, karena Anthony sudah mempelajari multiprocessing, dia ingin mengelompokkan kedua kategori ini secara paralel untuk menghemat waktu.


Struktur Output:

- Berdasarkan Huruf Pertama Judul Film:
    - Folder: `judul/`
    - Setiap file dinamai dengan huruf abjad atau angka, seperti `A.txt`, `B.txt`, atau `1.txt`.
    - Jika judul film tidak dimulai dengan huruf atau angka, film tersebut disimpan di file #.txt.

- Berdasarkan Tahun Rilis:
    - Folder: `tahun/`
    - Setiap file dinamai sesuai tahun rilis film, seperti `1999.txt`, `2021.txt`, dst.

Format penulisan dalam setiap file :
```
Judul Film - Tahun Rilis - Sutradara
```

Setiap proses yang berjalan akan mencatat aktivitasnya ke dalam satu file bernama `log.txt` dengan format:
```
[jam:menit:detik] Proses mengelompokkan berdasarkan [Abjad/Tahun]: sedang mengelompokkan untuk film [judul_film]
```

Contoh Log:
```
[14:23:45] Proses mengelompokkan berdasarkan Abjad: sedang mengelompokkan untuk film Avengers: Infinity War
[14:23:46] Proses mengelompokkan berdasarkan Tahun: sedang mengelompokkan untuk film Kung Fu Panda
```

code :
``` c
void category(char *file, char *type){
    int status;
    pid_t pid = fork();

    //Bagian 1
    if(pid == 0){
        char *argv[] = {"mkdir", "-p", type, NULL};
        execvp("mkdir", argv);
    } else wait(&status);

    //Bagiam 2
    FILE *data = fopen(file, "r");
    char dataLine[1000];
    fgets(dataLine, 1000, data);

    while(fgets(dataLine, 1000, data)){
        //Bagian 3
        dataLine[strcspn(dataLine, "\n")] = '\0';

        char *collum[4];
        awk(dataLine, collum);

        char *title = collum[0], *director = collum[1], *year = collum[3], fileName[50];

        //Bagian 4
        if(strcmp(type, "judul") == 0){
            log("Abjad", title);

            char mainTitle = title[0];
            if(isalnum(mainTitle))
                snprintf(fileName, sizeof(fileName), "%s/%c.txt", type, mainTitle);
            else
                snprintf(fileName, sizeof(fileName), "%s/#.txt", type);
        } else if(strcmp(type, "tahun") == 0){
            log("Tahun", title);
            snprintf(fileName, sizeof(fileName), "%s/%s.txt", type, year);
        }

        //Bagian 5
        FILE *txt = fopen(fileName, "a");
        if(txt){
            fprintf(txt, "%s - %s - %s\n", title, year, director);
            fclose(txt);
        }
    }
    fclose(data);
}
```

Penjelasan :
- Untuk mengelompokan judul film menggunakan function dan algoritma yang sama, yang membedakan hanya ketika ingin menuliskan judul film di file yang sesuai dengan kriteria.
- `Bagian 1` : Bagian 1 menggunakan parent and child process untuk membuat directory sesuai dengan ketentuan sorting (abjad/tahun). Child akan bertugas untuk membuat subdirectory, dan parent akan menunggu child, kemudian melanjutkan proses untuk melakukan sort.
- `Bagian 2` : Bagian 2 menginisialisasi untuk membaca isi dari file `netflixData.csv`. `fgets` dideklarasikan sekali untuk membaca header, sehingga ketika masuk ke loop, header tidak terbaca dan tidak dianggap sebagai data yang akan disorting.
- `Bagian 3` : Agar line dalam file dapat dibaca, maka ditambahkan terminator `\0` agar line bisa dibaca sebagai string. Kemudian pembuatan variabel collum yang nantinya akan menyimpan setiap segmen dari file yang meliputi nama film, director, negara, dan tahun. Lalu pemanggilan function `awk` untuk mengambil masing-masing data tersebut. Setelah data film tersimpan pada variabel collum, saya memindahkan data tersebut ke variabel baru agar lebih mudah untuk diolah.
- `Bagian 4` : Bagian 4 ini mengecek terlebih dahulu, file film yang sedang diolah ingin disortir berdasarkan apa, menggunakan variabel `type` yang memegang value abjad/tahun. Setiap sorting akan memanggil function `log` terlebih dahulu, dan memberikan parameter film disortir berdasarkan apa dan judul dari filmnya. Kemudian untuk sorting berdasarkan abjad, karena hanya diminta untuk mengecek pada huruf pertama, maka huruf pertama di cek menggunakan `isalnum` untuk mendeteksi apakah huruf pertama alfabet atau bukan. Bila bukan alfabet atau angka maka akan langsung masuk ke file `#.txt`, bila alfabet maka akan masuk ke file yang sesuai dengan huruf pertama. Sorting berdasarkan tahun langsung memasukannya kedalam file `<tahun>.txt`.
- `Bagian 5` : Path dari tujuan film di assign ke dalam variabel `fileName`, yang kemudian akan dibuka, kemudian dituliskan detail film sesuai dengan kriteria soal.

Function `awk`
``` c
void awk(char *line, char *collum[]){
    int flag = 0, col = 0, index = 0;
    char temp[1000], *ptr = line;

    while(*ptr != '\0'){
        if(*ptr == '"'){
            flag = !flag;
            temp[index++] = *ptr;
        } else if(*ptr == ',' && flag == 0){
            temp[index] = '\0';
            collum[col++] = strdup(temp);
            index = 0;
        } else {
            temp[index] = *ptr;
            index++;
        }
        ptr++;
    }

    temp[index] = '\0';
    collum[col++] = strdup(temp);
}
```

Penjelasan :
- Dalam file csv, tanda koma `,` berfungsi sebagai separator, namun ada juga tanda koma yang bukan sebagai separator, namun memang masih bagian dari 1 segmen, misalnya judul film yang mengandung tanda koma. Oleh karena itu, diperlukan if statement dan flag untuk menandakan apakah tanda koma berada dalam sebuah tanda petik atau tidak. Bila bertemu dengan tanda petik, maka tanda koma akan dianggap seperti karakter biasa, bukan sebagai separator. Bila koma sebagai separator, maka untuk menandakan bahwa segmen tersebut sudah berakhir, index terakhir diberi terminator `\0` kemudian variabel isi variabel `temp` dimasukan ke variabel `collum`, dan variabel `temp` dikosongkan untuk mengambil segmen berikutnya.

Function `log`
``` c
void log(char *category, char *title){
    FILE *log = fopen("log.txt", "a");
    char hour[9];

    //Bagian 1
    time_t seconds;
    struct tm *times;
    time(&seconds);
    times = localtime(&seconds);

    //Bagian 2
    strftime(hour, sizeof(hour), "%H:%M:%S", times);
    fprintf(log, "[%s] Proses mengelompokkan berdasarkan %s: sedang mengelompokkan untuk film %s\n", hour, category, title);
    fclose(log);
}
```

Penjelasan :
- `Bagian 1` : Berguna untuk mengambil waktu sekarang, kemudian menyimpan data waktu pada struct `times`, yang berisi detail-detail waktu seperti jam, menit, detik, tanggal, bulan, dan sebagainya.
- `Bagian 2` : Untuk menyesuaikan format waktu, dapat melakukan format dengan menggunakan `strftime` kepada variabel `hour`, yang nantinya `strftime` akan melakukan format otomatis sesuai dengan yang sudah kita tentukan. Kemudian, melakukan print kedalam `log.txt`.

Setelah program dijalankan :

![Alt text](https://media.discordapp.net/attachments/1123949811020873759/1367129369369247764/image.png?ex=681375f1&is=68122471&hm=bc624309b2f5f9ee5be6ac989580f4a37d8c0502a9b5474709aa2b6badcd9731&=&format=webp&quality=lossless&width=618&height=470)

![Alt text](https://media.discordapp.net/attachments/1123949811020873759/1367129490530107422/image.png?ex=6813760e&is=6812248e&hm=cfd0cddd5ab5093de537092bf9256cce4baf28b1c5caad47287c89abeed0692a&=&format=webp&quality=lossless&width=618&height=980)

![Alt text](https://media.discordapp.net/attachments/1123949811020873759/1367129469843804170/image.png?ex=68137609&is=68122489&hm=e6a470aed50f06285336cdec90d399506e68e282f024110c32cca6676914fcd2&=&format=webp&quality=lossless&width=618&height=980)

Contoh `#.txt`
```
"El patrón, radiografía de un crimen" - 2014 - Sebastián Schindel
"LSD: Love, Sex Aur Dhokha" - 2010 - Dibakar Banerjee
'76 - 2016 - Izu Ojukwu
#Selfie - 2014 - Cristina Jacob
#Selfie 69 - 2016 - Cristina Jacob
Çarsi Pazar - 2015 - Muharrem Gülmez
"Blue, Painful, Fragile" - 2020 - Shunsuke Kariyama
"My Teacher, My Obsession" - 2018 - Damián Romay
```

Contoh `2017.txt`
```
"Pop, Lock 'n Roll" - 2017 - Ziggy Hofmeyr
On the Real - 2017 - Tosin Coker
Fanatyk - 2017 - Michał Tylka
Good Time - 2017 - "Benny Safdie, Josh Safdie"
The Ballad of Lefty Brown - 2017 - Jared Moshé
```

Contoh `log.txt`
```
[20:23:50] Proses mengelompokkan berdasarkan Abjad: sedang mengelompokkan untuk film Sankofa
[20:23:50] Proses mengelompokkan berdasarkan Tahun: sedang mengelompokkan untuk film Sankofa
[20:23:50] Proses mengelompokkan berdasarkan Abjad: sedang mengelompokkan untuk film The Great British Baking Show
[20:23:50] Proses mengelompokkan berdasarkan Abjad: sedang mengelompokkan untuk film The Starling
[20:23:50] Proses mengelompokkan berdasarkan Tahun: sedang mengelompokkan untuk film The Great British Baking Show
[20:23:50] Proses mengelompokkan berdasarkan Abjad: sedang mengelompokkan untuk film Je Suis Karl
[20:23:50] Proses mengelompokkan berdasarkan Tahun: sedang mengelompokkan untuk film The Starling
[20:23:50] Proses mengelompokkan berdasarkan Abjad: sedang mengelompokkan untuk film Jeans
[20:23:50] Proses mengelompokkan berdasarkan Tahun: sedang mengelompokkan untuk film Je Suis Karl
[20:23:50] Proses mengelompokkan berdasarkan Abjad: sedang mengelompokkan untuk film Grown Ups
```

c. Sebagai penggemar film yang juga suka menganalisis, Anthony ingin mengetahui statistik lebih mendalam tentang film-film yang dia koleksi. Misalnya, dia ingin tahu berapa banyak film yang dirilis sebelum tahun 2000 dan setelah tahun 2000.

Agar laporan tersebut mudah dibaca, Anthony ingin hasilnya disimpan dalam file `report_ddmmyyyy.txt.`

*Format Output dalam Laporan*
```
i. Negara: <nama_negara>
Film sebelum 2000: <jumlah>
Film setelah 2000: <jumlah>

...
i+n. Negara: <nama_negara>
Film sebelum 2000: <jumlah>
Film setelah 2000: <jumlah>
```

Code :
``` c
typedef struct report{
    int before, after;
    char country[100];
}rep;

void report(char *file_csv){
    //Bagian 1.0
    pthread_t thread;
    pthread_create(&thread, NULL, country, file_csv);
    pthread_join(thread, NULL);

    char fileName[100], date[10];

    //Bagian 1.1
    time_t seconds;
    struct tm *timeDetail;
    time(&seconds);
    timeDetail = localtime(&seconds);

    //Bagian 2.0
    strftime(date, sizeof(date), "%d%m%Y", timeDetail);
    sprintf(fileName, "report_%s.txt", date);
    FILE *report = fopen(fileName, "w");

    //Bagian 2.1
    for(int i = 0; i < countryCount; i++){
        fprintf(report, "%d. Negara: %s\n", (i+1), stat[i].country);
        fprintf(report, "Film sebelum 2000: %d\n", stat[i].before);
        fprintf(report, "Film setelah 2000: %d\n\n", stat[i].after);
    }
    fclose(report);
}
```

Penjelasan :
- `Bagian 1.0` : Menggunakan thread untuk menjalankan function `country` yang bertugas merekap film dari masing-masing negara, yang nantinya data dari film masing-masing negara akan disimpan di struct `report`.
- `Bagian 1.1` : Mengambil tanggal sekarang, kemudian menyimpan data dari tanggal sekarang pada struct `timeDetail`.
- `Bagian 2.0` : Menyesuaikan format waktu menjadi tanggal sekarang dengan format yang diminta pada soal. Kemudian membuka file `report_<date>.txt`.
- `Bagian 2.1` : Proses write data kedalam file `report_<date>.txt` dalam for loop sejumlah negara yang terdaftar.


Function `country`
``` c
rep stat[100];
int countryCount = 0;

void *country(void *arg){
    //Bagian 1
    char *data = (char*)arg;
    FILE *file = fopen(data, "r");

    char line[1000];
    fgets(line, 1000, file);
    while(fgets(line, 1000, file)){
        //Bagian 2
        line[strcspn(line, "\n")] = '\0';

        char *collum[4];
        awk(line, collum);

        char *countri = collum[2];
        int year = atoi(collum[3]);

        //Bagian 3
        if(strlen(countri) == 0) continue;

        //Bagian 4
        int found = 0;
        for(int i = 0; i < countryCount; i++){
            if(strcmp(stat[i].country, countri) == 0){
                if(year < 2000) stat[i].before++;
                else stat[i].after++;
                found = 1;
                break;
            }
        }

        // Bagian 5
        if(!found){
            strcpy(stat[countryCount].country, countri);
            if(year < 2000){
                stat[countryCount].before = 1;
                stat[countryCount].after = 0;
            } else {
                stat[countryCount].before = 0;
                stat[countryCount].after = 1;
            }
            countryCount++;
        }
    }
    fclose(file);
    return NULL;
}
```

Penjelasan :
- Function `country` memiliki algoritma yang kurang lebih sama dengan function `category`, dengan ada sedikit perbedaan pada bagian di dalam while loop nya.
- `Bagian 1` : Bagian 1 menyiapkan semua variabel yang nantinya akan digunakan untuk mendata film beserta negaranya
- `Bagian 2` : Bagian 2 mengganti baris terakhir menjadi terminator `\0`, lalu menggunakan function `awk` untuk mengambil data. Nama negara akan disimpan pada variabel `countri`, dan tahun pada variabel year, namun karena tahun masih bertipe string pada variabel `collum`, untuk mengubah string menjadi int menggunakan `atoi`.
- `Bagian 3` : Untuk mengatasi masalah negara muncul 2 kali atau terdapat negara tanpa nama, pertama perlu dibuat if statement untuk mengecek apabila nama negara kosong, maka data tersebut tidak akan diolah.
- `Bagian 4` : Masalah negara yang muncul 2 kali pada `report_<date>.txt` kemungkinan dapat terjadi karena logic dari algoritma sebelum revisi adalah dengan menggunakan variabel `count`, bila jumlah count sama dengan jumlah `countryCount` (negara yang sudah terdaftar), maka hal tersebut menandakan bahwa negara tersebut belum terdaftar, sehingga `Bagian 5` akan dijalankan untuk mendaftarkan negaranya. `Bagian 4` ini akan berjalan apabila nama negara dari film yang sedang di cek cocok dengan nama negara yang sudah terdaftar, kemudian di cek tahun berapa film tersebut rilis dan variabel `after` atau `before` ditambah dengan 1 sesuai dengan tahun rilis.
- `Bagian 5` : Pada `Bagian 5` ini bertugas untuk mendaftarkan negara baru, dengan menyimpan nama negara pada variabel `country` dalam struct, dan melakukan inisialisasi nilai untuk jumlah film. If statement akan bekerja apabila negara belum terdaftar, ditandai dengan nilai found 0.

Code sebelum revisi :
``` c
int count = 0;
    for(int i = 0; i < countryCount; i++){
        if(strcmp(stat[i].country, countri) == 0){
            if(year < 2000) stat[i].before++;
            else stat[i].after++;
            count++;
            break;
        }
    }

    if(count == countryCount){
        strcpy(stat[countryCount].country, countri);
        if(year < 2000){
            stat[countryCount].before = 1;
            stat[countryCount].after = 0;
        } else {
            stat[countryCount].before = 0;
            stat[countryCount].after = 1;
        }
        countryCount++;
    }
```


Contoh `recap_<date>.txt`
```
1. Negara: United States
Film sebelum 2000: 236
Film setelah 2000: 1894

2. Negara: United Kingdom
Film sebelum 2000: 26
Film setelah 2000: 321

3. Negara: Germany
Film sebelum 2000: 2
Film setelah 2000: 68

4. Negara: India
Film sebelum 2000: 83
Film setelah 2000: 827

5. Negara: China
Film sebelum 2000: 0
Film setelah 2000: 58
```

# Task 3 _(Cella’s Manhwa)_
Cella, si ratu scroll Facebook, tiba-tiba terinspirasi untuk mengumpulkan informasi dan foto dari berbagai **manhwa favoritnya**. Namun, kemampuan ngoding Cella masih cetek, jadi dia butuh bantuanmu untuk membuatkan skrip otomatis agar semua berjalan mulus. Tugasmu adalah membantu Cella mengolah data manhwa dan heroine-nya.

Berikut adalah daftar manhwa bergenre shoujo/josei yang paling disukai Cella:

|    No     |      Manhwa      |
| :--------: | :------------: |
| 1 | Mistaken as the Monster Duke's Wife |
| 2 | The Villainess Lives Again |
| 3 | No, I Only Charmed the Princess! |
| 4 | Darling, Why Can't We Divorce? |

Untuk menyelesaikan beberapa persoalan setelah ini, saya membuat beberapa fungsi tambahan untuk mempermudah dalam memecahkan setiap permasalahan nanti. Berikut fungsi-fungsi nya:
##### ``Fungsi mengambil data dari API``
```c
struct memory {
    char *memory;
    size_t size;
};

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
```
- Pada kode diatas, terdapat struct ``memory`` yang berfungsi menyimpan isi jSON dari API yang kita miliki. Disini, kita menggunakan library ``cJSON`` untuk mendapatkan informasi dari API tersebut. ``CURLOPT_WRITEFUNCTION`` disini berfungsi mengambil berupa memory dari jSON yang kita dapat kemudian kita salin ke struct kita dengan ``CURL_WRITEDATA`` lalu setelah itu kita mengembalikan nilai memory saja karena yang kita perlukan hanya isi dari jSON tersebut.
##### ``Fungsi zip suatu file``
```c
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
```
- Fungsi diatas ini berguna untuk membuat zip dari suatu file sesuai dengan parameter yang ada. ``name`` disini bisa berupa nama zip sekaligus letak direktori nya lalu ``dest`` disini berupa file yang akan di-zip menuju variabel ``name``.
##### ``Fungsi membuat direktori``
```c
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
```
- Fungsi ini diatas ini berguna untuk membuat direktori baru berdasarkan parameter yang ada yaitu berupa ``folder`` yang bisa berisi nama folder atau beserta di direktori mana ingin membuat direktori baru tersebut.
##### ``Fungsi mengganti spasi menjadi underscore(_) dan menghilangkan karakter khusus``
```c
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
```
- Fungsi ini akan berguna di beberapa poin persoalan yang berguna untuk mengganti spasi dengan underscore (_) lalu menghilangkan karakter khusus dari sebuat string.
### Poin A (Summoning the Manhwa Stats)
Cella ingin mengambil data detail dari **manhwa** menggunakan [API Jikan](https://docs.api.jikan.moe/). Informasi yang diambil:

- Judul
- Status
- Tanggal rilis
- Genre
- Tema
- Author

Setelah data berhasil diambil, hasilnya harus disimpan ke dalam file teks, dengan nama file disesuaikan dengan **judul versi bahasa Inggris** (tanpa karakter khusus dan spasi diganti dengan underscore). Semua file teks disimpan dalam folder `Manhwa`.

#### Solusi
```c
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
```
#### Penjelasan
```c
cJSON *json_data = cJSON_Parse(json);
cJSON *data = cJSON_GetObjectItem(json_data, "data");
```
- ``cJSON_Parse(json)`` untuk menyimpan data JSON yang sudah dipisah-pisah perbagian sehingga data JSON dapat dioperasikan untuk mencari bagian tertentu.
- ``cJSON_GetObjectItem(json_data, "data")`` untuk mengambil bagian dengan kata kunci ``data`` yang berisi data dari manhwa itu sendiri.
```c
char *status = cJSON_GetObjectItem(data, "status")->valuestring;
char *title = cJSON_GetObjectItem(data, "title_english")->valuestring;
```
- Menginisialisai variabel ``char *`` yang berfungsi menyimpan alamat memori yang berupa string.
- ``cJSON_GetObjectItem(data, <kata_kunci>)->valuestring``, fungsi dari library cJSON ini kita akan memfilter kembali JSON yang sudah di uraikan tadi menjadi lebih spesifik dengan kata kunci tertentu, lalu ``->valuestring`` kita dapat mengambil data nya dengan tambahan pointer ke value nya. Disini kita akan mengambil judul versi bahasa Inggris dan status manhwa tersebut.
```c
cJSON *published = cJSON_GetObjectItem(data, "published");
int year = 0, month = 0, day = 0;
if (published) {
    cJSON *prop = cJSON_GetObjectItem(published, "prop");
    cJSON *from = cJSON_GetObjectItem(prop, "from");
    year = cJSON_GetObjectItem(from, "year")->valueint;
    month = cJSON_GetObjectItem(from, "month")->valueint;
    day = cJSON_GetObjectItem(from, "day")->valueint;
}
```
Bagian ini diawali kita menginisialisasi variabel integer untuk menyimpan tahun, bulan, dan hari. Kita juga membuat variabel ``cJSON`` yang mengambil bagian ``published`` yang mana di dalam bagian tersebut terdapat tanggal terbit manhwa tersebut.
- ``cJSON *prop = cJSON_GetObjectItem(published, "prop")`` dan ``cJSON *from = cJSON_GetObjectItem(prop, "from");`` dua fungsi ini akan memperkecil yaitu fokus pada isi ``prop`` yang berisi tanggal pertama kali publish.
- ``cJSON_GetObjectItem(from, <kata_kunci>)->valueint``, fungsi ini kita gunakan untuk mengambil isi dari objek sesuai kata kunci yaitu berupa year, month, atau day yang berupa integer.
```c
char genres[512], authors[512], themes[512];
genres[0] = '\0';
authors[0] = '\0';
themes[0] = '\0';
cJSON *genres_tmp = cJSON_GetObjectItem(data, "genres");
cJSON *authors_tmp = cJSON_GetObjectItem(data, "authors");
cJSON *themes_tmp = cJSON_GetObjectItem(data, "themes");
```
Bagian ini berupa inisialisasi beberapa variabel berupa ``cJSON`` dan ``char`` yang mana masing-masing akan kita operasikan.
```c
cJSON_ArrayForEach(item, genres_tmp) {
    char *name = cJSON_GetObjectItem(item, "name")->valuestring;
    if (strlen(genres) > 0) {
        strcat(genres, ", ");
    }
    strcat(genres, name);
}
```
Fungsi ``cJSON_ArrayForEach(item, genres_tmp)`` ini akan mengecek setiap array yang ada di variabel ``genres_temp`` yang sementara elemen-elemen yang ada didalamnya akan disimpan di variabel ``item``. Lalu, kita lanjutkan mengambil objek berupa ``name`` yang merupakan isi nama-nama dari genre, author, ataupun tema manhwa tersebut. Disini kita tambahkan kondisi ``strlen(genres) > 0`` karena manhwa memungkinkan memiliki lebih dari satu objek nama dari setiap genre, author, ataupun manhwa. Kita gunakan ``strcat()`` untuk secara teratur menambah ke akhir string. Fungsi ini kita gunakan juga untuk mengambil data untuk authors dan themes.
```c
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
```
- Kita inisialisasi variabel ``dir`` yang awalnya hanya berisi ``Manhwa/`` karena untuk isi selanjutnya akan kita isi dengan fungsi ``format_txt()`` yang saya buat diawal untuk mengubah nama judul sesuai kriteria yaitu tanpa spasi dan karakter khusus.
- Disini kita juga menggunakan fungsi ``buat_direktori()`` untuk memudahkan kita dalam membuat direktori baru tanpa ``mkdir()``.
- ``fopen(dir, "w")`` disini berguna untuk membuka file .txt yang nama file nya sudah sesuai kriteria dengan access "write" yaitu akan mengganti isi dari txt dengan isi yang baru.
- ``fprintf()`` disini kita gunakan untuk mencatatkan suatu output ke file yang sudah kita buka tadi lalu tidak lupa kita tutup access file nya dengan ``fclose()``.
#### Output
![image](https://github.com/rdtzaa/assets/blob/1b4bb24a0e62baaa6dd4f1c22c2a62c2dfe56213/Sistem%20Operasi/modul-2-task3.png)
#### Kendala
Kendala pada poin A ini pertama kali persoalan muncul saya masih bingung untuk cara parsing dara JSON apakah parsing manual seperti menggunakan ``strstr()`` lalu ternyata terdapat library cJSON yang dapat digunakan untuk memparse data JSON dengan fungsi dari library tersebut.
### Poin B (Seal the Scrolls)
Cella ingin agar setiap file `.txt` tadi di-**zip** satu per satu dan disimpan ke dalam folder baru bernama `Archive`. Yang dimana nama masing masing dari zip diambil dari **huruf kapital nama file**.
#### Solusi
```c
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
```
#### Penjelasan
```c
    cJSON *json_data = cJSON_Parse(json);
    cJSON *data = cJSON_GetObjectItem(json_data, "data");
    buat_direktori("Archive");
    char *title = cJSON_GetObjectItem(data, "title_english")->valuestring;
```
Sama halnya dengan poin A disini kita perlu menguraikan dulu data dari JSON yang sudah kita dapatkan lalu kita ambil bagian ``data`` saja. Disini diikut fungsi ``buat_direktori("Archive)`` karena pada poin B ini kita akan menyimpan ke direktori ``Archive``. Lalu kita pertama-tama akan mengambil judul versi bahasa Inggris terlebih dahulu.
```c
char dir[256] = "Manhwa/";
char filename[128];
char zip[128] = "Archive/";
int idx = 8;
```
- ``dir`` variabel ini akan kita gunakan untuk menunjuk direktori dari file .txt yang sudah kita punya.
- ``zip``, variabel ini kita gunakan untuk menyimpan direktori dari zip yang akan kita buat.
- ``filename`` akan kita gunakan untuk menyimpan dari nama zip yang akan dibuat.
- ``idx`` disini sebagai index dari karakter di variabel ``zip``.
```c
format_txt(title, filename);
strcat(dir, filename);
```
- Dengan fungsi ``format_txt()`` yang akan mengubah nama judul sesuai dengan kriteria lalu kita gabungkan dengan ``strcat()`` ke variabel ``dir``.
```c
for (int i = 0; filename[i] != '\0'; i++) {
    if (filename[i] >= 'A' && filename[i] <= 'Z') {
        zip[idx] = filename[i];
        idx++;
    }
}
zip[idx] = '\0';
strcat(zip, ".zip");
```
Looping disini untuk mengisi variabel ``zip`` sesuai dengan kriteria nama zip yang diminta yaitu hanya huruf kapital dari judul versi bahasa Inggris dengan cara kita menambahkan kondisi ``filename[i] >= 'A' && filename[i] <= 'Z'``. Lalu di akhir kita akan menambahkan ``.zip`` ke akhir variabel ``dir``.
```c
buat_zip(zip, dir);
```
Fungsi ini akan membuat zip dari file .txt yang berada di folder ``Manhwa/`` dan akan di zip ke folder ``Archive/``.
#### Output
![image](https://github.com/rdtzaa/assets/blob/53d6eb666317ec2684290fab5187f9157e05fc39/Sistem%20Operasi/modul-2-task3_b.png)
### Poin C (Making the Waifu Gallery)
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
#### Solusi
```c
struct thread {
    char *folder;
    char *name;
    char *url;
    int jumlah;
};

pthread_mutex_t download;

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
```
#### Penjelasan
```c
struct thread {
    char *folder;
    char *name;
    char *url;
    int jumlah;
};

pthread_mutex_t download;
```
- Inisialisasi struct ``thread`` untuk menyimpan nama folder, FMC, url, dan bulan dari beberapa manhwa.
- ``pthread_mutex_t download;`` menginisialisasi mutual exclusion untuk mengatur kerja dari masing-masing thread.
```c
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
```
Fungsi ini akan dijalankan dengan ``pthread_create()`` dengan parameter sebuah struct ``thread`. Disini juga diikuti dengan ``pthread_mutex_lock(&download);`` dan ``pthread_mutex_unlock(&download);`` yang mana fungsi ini akan membuat thread akan mendownload gambar secara urut.
- ``for (int i = 1; i <= data->jumlah; i++)``, for loop disini akan berulang sebanyak angka bulan yang disimpan di struct.
- ``snprintf(filename, sizeof(filename), "%s/%s_%d.jpg", data->folder, data->name, i)`` mengassign nilai yaitu berupa nama file dan direktori nya yang disesuaikan kriteria.
- ``char *args[] = {"curl", "-s", "-L", data->url, "-o", filename, NULL};``, disini saya menggunakan curl untuk mendownload gambar tersebut dengan tambahan ``-s`` yang berarti mode silent dan ``-L`` adalah opsi untuk menyimpan di direktori tertentu.
- ``execv("/usr/bin/curl", args)`` untuk menjalankan curl ini saya menggunakan ``fork()`` karena fungsi ``execv()`` ketika dijalankan maka kode setelahnya tidak dijalankan.
```c
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
```
- ``buat_direktori("Heroines");`` dipanggil untuk membuat folder bernama Heroines.
- Variabel ``fmc`` untuk menyimpan nama-nama Female Main Character dan ``url`` untuk menyimpan url gambar tiap heroines.
- ``pthread_t threads[jumlah_manhwa];`` menginisialisasi thread untuk mendownload gambar setiap heroines.
- ``month = cJSON_GetObjectItem(from, "month")->valueint;``, seperti pada task-task sebelumnya kita mengambil angka bulan sebagai banyak foto yang perlu di download dengan menggunakan library cJSON.
- ``snprintf(folder, sizeof(folder), "Heroines/%s", fmc[i]);`` mengassign nilai ke variabel folder berupa ``Heroines/[NAMA_HEROINES]`` lalu akan digunakan untuk membuat direktori.
- ``struct thread *data_thread = malloc(sizeof(struct thread));`` menginisialisasi struct yang akan digunakan thread nanti untuk mendowload file dan mengassign nilai dari pointer di dalam struct nya dengan ``strdup()``.
- ``pthread_create(&threads[i], NULL, download_heroine, data_thread);`` digunakan untuk membuat proses thread mendowload gambar dengan fungsi ``download_heroine`` lalu didalam fungsi tersebut terdapat ``mutex`` agar bekerja secara urut``
- ``pthread_join(threads[i], NULL);`` memastikan setiap thread berjalan dengan baik.
#### Output
![image](https://github.com/rdtzaa/assets/blob/0eec955cb3eb9c64fffd6e7e883970166413b0c8/Sistem%20Operasi/modul-2-task3_c.png)
#### Kendala
Pada poin C ini, saya kurang membaca soal bahwa download gambar ini berdasarkan urutan manhwa tidak hanya menggunakan thread yang bersama-sama sehingga untuk mengatasi ini saya menggunakan mutual exclusion untuk mengatur jalannya masing-masing thread.
### Poin D (Zip. Save. Goodbye)
Setelah semua gambar heroine berhasil diunduh, Cella ingin mengarsipkannya:

- Setiap folder heroine di-zip dengan format:
  ```
  [HURUFKAPITALNAMAMANHWA]_[namaheroine].zip
  ```
- Disimpan di folder `Archive/Images`
- Setelah zip selesai, gambar pada masing masing folder Heroine akan dihapus secara **urut dengan abjad**.
#### Solusi
```c
struct heroine_info {
    char name[128];
    char filepath[256];
    int month;
    char kapital[64];
};

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
        char zipname[512];
        snprintf(zipname, sizeof(zipname), "Archive/Images/%s_%s.zip", heroines[i].kapital, heroines[i].name);

        for (int j = 1; j <= heroines[i].month; j++) {
            char filepath[512];
            snprintf(filepath, sizeof(filepath), "%s/%s_%d.jpg", heroines[i].filepath, heroines[i].name, j);
            remove(filepath);
            printf("%s terhapus\n", filepath);
        }
    }
}

```
#### Penjelasan
```c
struct heroine_info {
    char name[128];
    char filepath[256];
    int month;
    char kapital[64];
};
void task4(char *json[], int jumlah_manhwa) {
    ...
    char* fmc[4] = {"Dellis", "Artizea", "Adelia", "Ophelia"};
    buat_direktori("Archive/Images");
    struct heroine_info heroines[4];
    ...
}
```
- Inisialisasi variabel string array ``fmc`` untuk menyimpan nama dari Heroines.
- Panggil ``buat_direktori`` untuk membuat direktori di folder ``Archive`` bernama ``Images``.
- Inisialisasi struct ``heroine_info`` untuk menyimpan nama heroines, filepath gambar, angka bulan, dan huruf kapital dari judul versi bahasa Inggris
```c
char *title = cJSON_GetObjectItem(data, "title_english")->valuestring;
char title_tmp[64];
title_tmp[0] = '\0';
for (int j = 0; title[j] != '\0'; j++) {
    if (title[j] >= 'A' && title[j] <= 'Z') {
        strncat(title_tmp, &title[j], 1);
    }
}
```
- Looping disini untuk mengambil huruf kapital saja dari data judul versi bahasa inggris yang disimpan ke variabel ``title_tmp``.
```c
char folder[128];
snprintf(folder, sizeof(folder), "Heroines/%s", fmc[i]);
month = cJSON_GetObjectItem(from, "month")->valueint;
strncpy(heroines[i].name, fmc[i], sizeof(heroines[i].name));
strncpy(heroines[i].filepath, folder, sizeof(heroines[i].filepath));
strncpy(heroines[i].kapital, title_tmp, sizeof(heroines[i].kapital));
heroines[i].month = month;
```
- Menyimpan path folder gambar dari setiap heroines ke variabel ``folder``.
- Mengambil angka bentuk integer bulan dengan fungsi ``cJSON_GetObjectItem``.
- Menyimpan nama heroines, filepath gambar, angka bulan, dan huruf kapital judul ke struct ``heroines`` dengan ``strncpy()`` agar terhindar dari ukuran yang berlebihan.
```c
int compare_heroine(const void *a, const void *b) {
    return strcmp(((struct heroine_info*)a)->name, ((struct heroine_info*)b)->name);
}

qsort(heroines, 4, sizeof(heroines[0]), compare_heroine);
```
- Dengan fungsi dari library ``stdlib`` yaitu berupa qsort kita akan mengurutkan struct berdasarkan nama agar dalam penghapusan file dapat berurutan secara abjad.
```c
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
```
Dua looping diatas hampir sama dimana perbedaaanya adalah pada bagian ``buat_zip()`` dan ``remove()``. Untuk looping pertama disini kita akan membuat path zip di variabel ``zipname`` lalu menzip gambar dari path gambar dari variabel ``filepath``. Kemudian, loop kedua akan langsung menghapus path gambar yang dituju oleh variabel ``filepath``.
#### Output
![image](https://github.com/rdtzaa/assets/blob/01662a663815c35c30f52b0584510d5925e4981e/Sistem%20Operasi/modul-2-task3_D.png)
![image](https://github.com/rdtzaa/assets/blob/01662a663815c35c30f52b0584510d5925e4981e/Sistem%20Operasi/modul2-task3_D01.png)
#### Kendala
Pada poin D ini, saya terkendala saat menzip dan menghapus file nya karena awalnya saya hanya dapat menzip beserta foldernya menggunakan ``zip -r`` dan menghapus gambar dari tiap heroines malah terhapus semua foldernya dengan ``rm -r`` lalu saya ubah logika saya tidak menjalankan satu fungs saja melainkan menggunakan looping dan menggunakan fungsi ``zip -j`` yang akan menzip dengan nama file spesifik lalu menggunakan fungsi ``remove()`` untuk menghapus file secara spesifik juga.

- [Task 4 - Pipip's Load Balancer](/task-4/)

### Client Mengirimkan pesan ke Load Balancer
Pada task ini, kami membagi program menjadi beberapa bagian:

Bagian 1 - Mengambil Input
```C
int main(int argc, char** argv)
{
    // ekstrak input dari command line
    int count = 0;
    int semicolon = 0;
    if(argc != 2){
        printf("usage: ./client '<your message>; <count>'' ");
        return 1;
    }
    int len = strlen(argv[1]);

    //cari indeks ;
    for(int i=0; i<len; i++){
        if(argv[1][i] == ';'){
            semicolon = i;
            break;
        }
    }

    //copy dari indeks 0 -> ;
    char msg[1000] = "";
    strncat(msg, argv[1], semicolon);
    
    //copy angka
    char num[5] = ""; 
    int j=0;
    for(int i=semicolon+1; i<len; i++){
        num[j] = argv[1][i];
        j++;
    }

    //convert string ke integer
    count = atoi(num);
```
Pada soal, input diberikan pada format <pesan> ; <jumlah_pesan>. Pada program kami, kami memerlakukan keseluruhan input sebagai sebuah string tunggal. Sehingga, pertama-tama program perlu mencari indeks ';'. Setelah itu, keseluruhan string hingga indeks sebelum ';' akan dicopy sebagai pesan yang akan dikirim, sedangkan bagian setelah indeks ';' sampai akhir string akan diconvert menjadi integer melalui fungsi ```atoi```

Bagian 2 - Menulis ke log
```C
pid_t child_id;
    child_id = fork();

    if(child_id == 0){
        write_to_log(msg, count);
    }
```
Program melakukan ```fork()```. Child process akan menulis ke log sedangkan parent process akan meneruskan pesan pada ```loadbalancer```. Berikut implementasi fungsi ```write_to_log()```
```C
void write_to_log(char message[1000], int count){
    pid_t child_id;
    int status;
    char cmd[1200];
    child_id = fork();

    if (child_id < 0) {
        perror("fork");
        return;
    }

    if (child_id == 0) {
        char *argv[] = {"touch", "/home/riyan/sisop_ws/sistem.log",NULL};
        execv("/bin/touch", argv);
        
    }

    else {
        while (wait(&status) > 0);
        sprintf(cmd, "echo Message from client: %s >> /home/riyan/sisop_ws/sistem.log; " "echo Message count: %d >> /home/riyan/sisop_ws/sistem.log", message, count);

        char *argv2[] = {"sh", "-c", cmd,NULL};
        execv("/bin/sh", argv2);

    }
}
```
Bagian 3 - Mengirim ke ```loadbalancer```
```C
    else {
        key_t key = 1234;
        int shmid = shmget(key, sizeof(Msg), IPC_CREAT | 0666);
        Msg* send = shmat(shmid, NULL, 0);

        send->repeat = count;
        sem_init(&send->empty, 1, 1);
        sem_init(&send->full, 1, 0);

        for(int i=0; i<count; i++){
            sem_wait(&send->empty);
            strncpy(send->message, msg, strlen(msg));
            sem_post(&send->full);
        }
        shmdt(send);
        shmctl(shmid, IPC_RMID, NULL);
    }
```
Pada bagian ini, kami mendefinisikan ```key``` secara manual sebagai ```1234``` alih-alih menggunakan ```ftok()``` agar lebih simple, mengingat hanya ada satu program lain yang akan menggunakan key tersebut. Pesan yang dikirim dimuat dalam bentuk  ```struct ``` yang didefinisikan sebagai berikut:
```C
typedef struct msg_t{
    sem_t empty;
    sem_t full;
    int repeat;
    char message[1000];
}Msg;
```
Dalam mengirimkan pesan, program menggunakan ```semaphore``` sebagai upaya sinkronisasi. Setelah mengirimkan pesan, program melakukan clean up dengan cara memanggil ```shmdt``` dan ```shmctl```

### Load Balancer Mendistribusikan Pesan ke Worker Secara Round-Robin
Pada bagian awal program, program mendefinisikan 2 ```struct```, yaitu untuk ```Msg``` untuk menerima pesan dari ```client``` dan ```Mesg_buffer``` untuk mengirim pesan ke ```worker```
```C
typedef struct msg_t {
    sem_t empty;
    sem_t full;
    int repeat;
    char message[1000];
} Msg;

typedef struct msg_buffer {
    long mesg_type;
    char mesg_text[1000];
    int order;
    bool last;
} Mesg_buffer;
```
Selanjutnya, deklarasi variabel-variabel yang diperlukan untuk mengakses shared memory dari ```client``` dan untuk mengirim pesan melalui ```message queue``` kepada ```worker```
```C
int main(int argc, char **argv){
    if (argc != 2) {
        printf("usage: %s <number of workers>\n", argv[0]);
        return 1;
    }

    int worker = atoi(argv[1]);
    if(worker<=0) {
        printf("invalid number of workers.\n");
        return 1;
    }

    // shared memory
    key_t shm_key = 1234;
    int shmid = shmget(shm_key, sizeof(Msg), 0666);
    Msg *send = (Msg *)shmat(shmid, NULL, 0);
  

    // message queue
    key_t msg_key = 4567;
    int msg_queue[worker];

    for(int i=0; i<worker; i++){
        msg_queue[i] = msgget(msg_key + i, 0666 | IPC_CREAT);
    }
```
Key untuk ```message queue``` di simpan dalam array. Banyaknya key menyesuaikan banyak worker yang ditentukan oleh user. Selanjutnya, program menerima pesan dari ```client``` dan meneruskannya pada ```worker```. Program melakukan looping dari 0 hingga ```send->repeat```. ```send->repeat``` merupakan banyak pesan yang dikirim oleh ```client```
```C
for(int i=0; i<send->repeat; i++) {
        sem_wait(&send->full);     
        write_to_log(send->message, i + 1);

        Mesg_buffer msg_send;
        msg_send.mesg_type = 1;
        msg_send.order = i+1;
        if(i==send->repeat-1) msg_send.last = true;
        else msg_send.last = false;
        strncpy(msg_send.mesg_text, send->message, strlen(send->message));

        msgsnd(msg_queue[(i)%worker], &msg_send, sizeof(msg_send.mesg_text) + sizeof(msg_send.order) + sizeof(msg_send.last), 0);

        sem_post(&send->empty);
     
    }
```
Selain itu, program juga menulis pesan yang diterima dari ```client``` pada file log, dengan implementasi fungsi ```write_to_log()``` sebagai berikut:
```C
void write_to_log(const char *message, int count){
    FILE *log = fopen("/home/riyan/sisop_ws/sistem.log", "a");
    if(log) {
        fprintf(log, "Received at lb: %s (#message %d)\n", message, count);
        fclose(log);
    }else{
        perror("fopen log file");
    }
}
```

Setelah semua pesan telah diteruskan kepada worker, program mengirimkan satu pesan tambahan yang berisi "TERMINATE" kepada ```worker``` untuk menandakan bahwa semua pesan telah terkirim.
```C
    for(int i = 0; i <worker; i++) {
        Mesg_buffer term_msg;
        term_msg.mesg_type = 1;

        strcpy(term_msg.mesg_text, "TERMINATE");
        term_msg.order = send->repeat + i;
        term_msg.last = false;

        msgsnd(msg_queue[i], &term_msg, sizeof(term_msg.mesg_text) + sizeof(term_msg.order) + sizeof(term_msg.last), 0);
        
    }
```
Langkah terakhir yang dilakukan oleh program adalah melakukan clean up
```C
 shmdt(send);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
```

### Worker Mencatat Pesan yang Diterima`dan Catat Total Pesan yang Diterima Setiap Worker di Akhir Eksekusi
Diantara keseluruhan task, program ```worker.c``` ini merupakan yang paling rumit dan membutuhkan beberapa kali perubahan pendekatan hingga mendapat hasil yang sesuai. Seperti pada program ```loadbalancer```, program ini membuat ```struct``` untuk menerima message yang dikirim oleh ```loadbalancer```
```C
typedef struct msg_buffer {
    long mesg_type;       
    char mesg_text[1000];
    int order;
    bool last;
} Mesg_buffer;
```
Berikut penjelasan lebih detail kegunaan masing-masing variabel pada ```struct```: <br>
```mesg_type``` menentukan tipe pesan yang diterima. Dalam ```message queue```, ```worker``` hanya dapat menerima pesan dengan tipe yang sama.<br>
```mesg_type``` berisi pesan yang dikirim <br>
```order``` menentukan urutan pesan saat ini <br>
```last``` merupakan ```flag``` untuk menentukan apakah pesan saat ini merupakan pesan yang terakhir. <br>

Pada program ```worker```, kami juga menggunakan ```semaphore``` untuk melakukan sinkronisasi pada tiap-tiap ```worker```. Kami menggunakan ```named semaphore``` dengan ```sem_open()``` alih-alih menggunakan ```unnamed semaphore``` dan ```sem_init()```. Hal ini dilakukan untuk mempermudah implementasi, mengingat ```sem_init()``` memerlukan ```shared memory``` antar child process agar dapat bekerja. Berikut implementasi ```semaphore``` pada program: 
```C
int main(int argc, char **argv) {
 
    if (argc != 2) {
        printf("usage: %s <number of workers>\n", argv[0]);
        return 1;
    }

    int worker = atoi(argv[1]);
    if (worker <= 0) {
        printf("invalid number of workers: %s\n", argv[1]);
        return 1;
    }

    for (int i = 0; i < worker; i++) {
        char sem_name[40];
        char sem_terminate[40];

        sprintf(sem_name, "/log_sem_%d", i);
        sprintf(sem_terminate, "/terminate_sem_%d", i);

        sem_unlink(sem_name);
        sem_unlink(sem_terminate); 

        sem_t *sem = sem_open(sem_name, O_CREAT | O_EXCL, 0666, (i == 0) ? 1 : 0);
        sem_t *semt = sem_open(sem_terminate, O_CREAT | O_EXCL, 0666, 0);
    
        sem_close(sem);
        sem_close(semt);
    }
```
Terdapat 2 ```semaphore``` yang digunakan pada masing-masing ```worker```, yaitu ```sem``` yang digunakan untuk menentukan urutan penulisan pada file log, dan ```semt``` yang digunakan untuk menentukan urutan penulisan total pesan yang diterima masing-masing worker. Program di atas berjalan pada parent process, dan bertujuan untuk menginisialisasi ```semaphore``` sebelum digunakan oleh masing-masing child process (worker). <br>
Setelah itu, program melakukan ```fork``` sesuai banyak ```worker``` yang diminta. Masing-masing ```worker``` menjalankan fungsi menerima pesan, mencatat banyak pesan yang diterima, dan menuliskannya ke log.
```C
 key_t key = 4567;
    int msg_queue[worker];
    for (int i = 0; i < worker; i++) {
        msg_queue[i] = msgget(key + i, 0666 | IPC_CREAT);
    }

    for (int i = 0; i < worker; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            Mesg_buffer msg;
            int worker_id = i;
            int count = 0;

            char my_sem_name[20], next_sem_name[20];
            char my_semt_name[40], next_semt_name[40];
            char first_semt_name[40];

            sprintf(my_sem_name, "/log_sem_%d", worker_id);
            sem_t *my_sem = sem_open(my_sem_name, 0);

            sprintf(my_semt_name, "/terminate_sem_%d", worker_id);
            sem_t *my_semt = sem_open(my_semt_name, 0);

            int next_worker = (worker_id + 1) % worker;

            sprintf(next_sem_name, "/log_sem_%d", next_worker);
            sem_t *next_sem = sem_open(next_sem_name, 0);

            sprintf(next_semt_name, "/terminate_sem_%d", next_worker);
            sem_t *next_semt = sem_open(next_semt_name, 0);

            sprintf(first_semt_name, "/terminate_sem_%d", 0);
            sem_t *first_semt = sem_open(first_semt_name, 0);
            

            while (1) {
                msgrcv(msg_queue[i], &msg, sizeof(msg.mesg_text) + sizeof(msg.order) + sizeof(msg.last), 1, 0);
             
                if (strcmp(msg.mesg_text, "TERMINATE") == 0) {
                    sem_post(next_sem);
                    break;
                }
                sem_wait(my_sem);
                write_to_log(worker_id + 1, msg.order);
                sem_post(next_sem);

                if(msg.last == true){
                    sem_post(first_semt);
                }
                

                count++;
            }

            sem_wait(my_semt);
            write_total_received(count, worker_id + 1);
            sem_post(next_semt);

            msgctl(msg_queue[i], IPC_RMID, NULL);
            _exit(0);
        }
    }
```
Berikut penjelasan lebih detail mengenai tiap-tiap ```semaphore``` pada masing-masing ```worker```: <br>
```my_sem_name[40] dan next_sem_name[40]``` merupakan buffer yang menyimpan nama semaphore. Nama ini sesuai dengan yang telah didefinisikan sebelumnya pada parent process. <br>
<br>
```my_semt_name[40] dan next_semt_name[40]``` merupakan buffer yang menyimpan nama terminate semaphore, yakni semaphore yang digunakan untuk menandakan bahwa sudah waktunya menulis jumlah pesan yang diterima masing-masing ```worker``` <br>
<br>
```my_sem``` merupakan semaphore untuk memberi tanda giliran menulis ke log. Pada mulanya, semua semaphore diset ke 0 untuk seluruh worker kecuali worker pada urutan pertama. <br>
<br>
```next_sem``` merupakan semaphore untuk memberi tanda giliran menulis ke log untuk worker selanjutnya. <br>
<br>
```my_semt``` merupakan semaphore untuk memberi tanda giliran menulis banyak pesan yang telah diterima ke log. Semua semaphore diset ke 0 untuk seluruh worker hingga pesan terakhir telah diterima. <br>
<br>
```next_semt``` merupakan semaphore untuk memberi tanda giliran menulis banyak pesan yang telah diterima ke log untuk worker selanjutnya. <br>
<br>
```first_semt``` merupakan semaphore untuk menuliskan banyak pesan yang diterima oleh worker pertama. Saat semaphore ini telah diset, menandakan seluruh pesan telah terkirim. <br>
<br>

Secara garis besar, alur kerja tiap worker adalah sebagai berikut:

- worker mengakses semua semaphore yang telah dibuat oleh parent <br> <br>
- worker memasuki perulangan ```while```, menerima pesan dari loadbalancer melalui fungsi ```msgrcv``` <br> <br>
- worker menunggu giliran untuk menulis pada log melalui ```sem_wait(my_sem)``` <br> <br>
- saat semaphore telah bernilai 1, worker menulis ke log <br> <br> 
- worker mengubah nilai semaphore untuk worker selanjutnya menjadi 1, menandakan giliran worker selanutnya untuk menulis pada log <br> <br>
- apabila msg.last dari loadbalancer bernilai true, worker mengubah nilai ```first_semt``` untuk worker pertama menjadi 1, menandakan semua pesan telah terkirim dan sudah saatnya menulis banyak pesan yang diterima tiap-tiap worker <br> <br>
- worker menerima pesan TERMINATE, keluar dari ```while loop``` <br> <br> 
- worker menunggu ```my_semt``` bernilai 1 untuk menulis banyak pesan yang diterima pada log <br> <br>
- worker mengubah nilai ```next_semt``` agar worker selanjutnya dapat menulis banyak pesean yang diterima pada log <br> <br>

### Kendala
Pada program worker, pada awalnya tiap-tiap worker tidak menulis ke log secara berurutan. Pesan telah terkirim secara round-robin, hanya urutan menulis ke log yang tidak tersinkronisasi. Untuk mengatasi masalah tersebut, kami pada mulanya menggunakan fungsi ```usleep``` untuk memberi jeda penulisan pada tiap-tiap worker. Nilai ```usleep``` akan semakin besar untuk worker pada urutan akhir. Kekurangan yang jelas dari metode ini adalah waktu delay akan menjadi cukup lama linear dengan jumlah worker. Selain itu, Saat salah satu worker telah selesai menerima pesan, ia akan secara langsung menulis jumlah pesan yang diterima meskipun worker lain masih menerima pesan. Untuk mengatasi permasalahan tersebut, digunakan semaphore yang pada akhirnya berhasil memberikan hasil yang konsisten sesuai dengan yang diminta, tanpa perlu menambah durasi program dengan signifikan.

### Hasil
- Program Client <br>
![IMG-20250430-WA0020 1](https://github.com/user-attachments/assets/a5656164-646a-4a68-8a43-03d68a16f9ea)<br>
![IMG-20250430-WA0021 1](https://github.com/user-attachments/assets/cfec397e-d79a-481a-92cd-7cd1990a986e)<br>

- Program Load Balancer <br>
![image](https://cdn.discordapp.com/attachments/1291076430993690685/1367150706959716452/image.png?ex=681389d0&is=68123850&hm=41d675a5ce5aa67575bad221da0c07b40094b9b93cbb1620273f5446dfaaf638&) <br>
![image](https://cdn.discordapp.com/attachments/1291076430993690685/1367151053316816926/image.png?ex=68138a23&is=681238a3&hm=7f1ad79f7f00f8404f7e5473e7a48e20f6aaae95644d6fd3f4c9f824a3d4ecbc&) <br>

- Program Worker <br>
![image](https://cdn.discordapp.com/attachments/1291076430993690685/1367151231587450940/image.png?ex=68138a4e&is=681238ce&hm=1e52d088c63fca021a3800831aaafaa08bc0829ae9f5e35eefb93f3f8097a1f0&) <br>
![image](https://cdn.discordapp.com/attachments/1291076430993690685/1367151446734147594/image.png?ex=68138a81&is=68123901&hm=2ead74bfdbe21e6e716027ce44429cbb049d5bfe9d61a3b73d9a40532b4f02ee&) <br>
