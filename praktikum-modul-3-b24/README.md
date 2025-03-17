[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/jYae_yK9)
<div align=center>

|    NRP     |            Name            | 
| :--------: | :------------------------: |
| 5025231091 |          Fernando          |
| 5025231113 | Reynandriel Pramas Thandya |

# Praktikum Modul 3 _(Module 3 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - Jess No Limit Push Rank](/task-1/) by Rey

- [Task 2 - BNBWT](/task-2/) by Rey, Fernando

- [Task 3 - Volume Expansion](/task-3/) by Rey

- [Task 4 - WOTA Competition](/task-4/) by Fernando

### Laporan Resmi Praktikum Modul 3 _(Module 3 Lab Work Report)_

- [Task 1 - Jess No Limit Push Rank](/task-1/) by Rey
#### Soal
##### Jess No Limit Push Rank

Jess No Limit sudah siap untuk comeback ke dunia professional esport Mobile Legend. Dia sudah bersusah payah untuk pushrank hingga ke Mythic Immortal dan membuktikan ke dunia bahwa dia adalah pemain yang paling handal. Ketika dia dan timnya kembali latihan, mereka kesulitan untuk menentukan role yang cocok bagi Jess No Limit. Mereka butuh bantuan kalian dalam hal ini, karena hero ML yang begitu banyak pada dataset mobile legend.

- a. Download dataset menggunakan `kaggle datasets download -d irwansyah10010/mobilelegend` setelah berhasil mendownload dengan format file .zip, langkah selanjutnya adalah mengekstrak file tersebut. Kalian melakukannya di dalam file [ayamgoreng.c](./ayamgoreng.c) untuk semua pengerjaannya

- b. Jess No Limit ingin mengetahui hero apa saja yang terbaik dari setiap role yang ada dengan perhitungan seperti berikut:

  |   Role   |        Rumus Skillpoints        |
  | :------: | :-----------------------------: |
  |   Tank   |               HP                |
  | Assassin |         Physical Attack         |
  | Fighter  |      HP + Physical Attack       |
  | Marksman | Physical Attack \* Attack Speed |
  |   Mage   |        Mana / Mana Regen        |
  | Support  |         Mana Regen + HP         |

  `ayamgoreng.c` harus bisa **mengoutputkan semua hero yang terbaik pada setiap role secara default tanpa argumen**, namun juga bisa menampilkan sesuai dengan **role yang diberikan pada argumen** pada saat pemanggilan program (`./ayamgoreng [role]`). Data hero beserta rolenya dapat dilihat pada file identitas.csv, sedangkan data hero beserta atributnya dapat dilihat pada file atribut-hero.csv. Keluarkan informasi mengenai hero dan skillpointsnya dengan format `Hero terbaik adalah (nama hero) dengan skillpoint (jumlah skillpoint)`.

- c. Jess No Limit menyadari bahwa sistem kalian sangat berguna dan ingin sistem ini bisa diakses oleh teman satu timnya. Oleh karena itu, kalian perlu menjadikan sistem yang dibuat ke sebuah Docker Container agar mudah di-distribute dan dijalankan di lingkungan lain tanpa perlu setup environment dari awal. Buatlah Dockerfile yang berisi semua langkah yang diperlukan untuk setup environment dan menentukan bagaimana aplikasi harus dijalankan.

  Setelah sukses membuat sistem berbasis Docker, Jess No Limit merasa bahwa sistem ini tidak hanya berguna untuk dirinya sendiri, tetapi juga akan akan membantu para fansnya yang terpencar di seluruh dunia dalam merekrut pemain berpotensi tinggi. Namun, satu tantangan muncul, yaitu bagaimana caranya para fans dapat mengakses dan menggunakan sistem yang telah diciptakan?

- d. Merasa terpanggil untuk membantu Jess No Limit lebih jauh, kalian memutuskan untuk mem-publish Docker Image sistem ke Docker Hub, sebuah layanan cloud yang memungkinkan kalian untuk membagikan aplikasi Docker kalian ke seluruh dunia. Output dari pekerjaan ini adalah file Docker kalian bisa dilihat secara public pada `https://hub.docker.com/r/{Username}/joki-ml-bang`.

- e. Melihat betapa banyaknya fans Jess No Limit, kalian diminta untuk menerapkan skala pada layanan menggunakan **Docker Compose** dengan instance sebanyak 6 (untuk setiap role). Pastikan setiap instance memanggil program untuk menampilkan hero terbaik sesuai dengan role yang diberikan pada argumen. Penamaan setiap instance sama dengan role yang akan ditampilkan (instance mage, instance fighter, dst).

###### Notes

- Pada poin c, command yang dijalankan pada docker file tidak terdapat argumen dan akan menampilkan semua role

#### Penjelasan
Full Code ayamgoreng.c:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Hero{
    char nama[1024];
    char role[1024];
    int hp;
    int pa;
    float as;
    int mana;
    float mreg;
    float sp;
    struct Hero *next;
} Hero;

Hero* read(const char *filename, int isAttributeFile) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        return NULL;
    }

    Hero *head = NULL, *tail = NULL;
    char buffer[1024];
    int isFirstLine = 1; 
    if(isAttributeFile){
        while (fgets(buffer, sizeof(buffer), file)) {
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        Hero *newHero = (Hero*)malloc(sizeof(Hero));
        if (!newHero) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(file);
            return head;
        }
        memset(newHero, 0, sizeof(Hero)); 

        char *token;
        int colIndex = 0;
        token = strtok(buffer, ";");
        while (token != NULL) {
            switch(colIndex) {
                case 0: 
                    strncpy(newHero->nama, token, 1023);
                    break;
                case 1:
                    newHero->hp = atoi(token);
                    break;
                case 3:
                    newHero->pa = atoi(token);
                    break;
                case 5: 
                    newHero->as = atof(token);
                    break;
                case 7: 
                    newHero->mana = atoi(token);
                    break;
                case 8: 
                    newHero->mreg = atof(token);
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ";");
            colIndex++;
        }

        newHero->next = NULL;
        if (head == NULL) {
            head = tail = newHero;
        } else {
            tail->next = newHero;
            tail = newHero;
        }
    }
    }else { 
    while (fgets(buffer, sizeof(buffer), file)) {
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        Hero *newHero = NULL;
        char *token;
        int colIndex = 0;
        char *roles[2] = {NULL, NULL}; 
        int roleCount = 0;

        token = strtok(buffer, ";");
        while (token != NULL) {
            if (colIndex == 0) {  
                newHero = (Hero*)malloc(sizeof(Hero));
                if (!newHero) {
                    fprintf(stderr, "Memory allocation failed\n");
                    fclose(file);
                    return head;
                }
                memset(newHero, 0, sizeof(Hero));
                strncpy(newHero->nama, token, 1023);
            } else if (colIndex == 1) { 
                char *slash = strchr(token, '/');
                if (slash) {  
                    *slash = '\0'; 
                    roles[0] = token;
                    roles[1] = slash + 1;
                    roleCount = 2;
                } else {
                    roles[0] = token;
                    roleCount = 1;
                }
            }
            token = strtok(NULL, ";");
            colIndex++;
        }

        for (int i = 0; i < roleCount; i++) {
            Hero *heroEntry = i == 0 ? newHero : (Hero*)malloc(sizeof(Hero));
            if (i > 0) {  
                memcpy(heroEntry, newHero, sizeof(Hero));
            }
            strncpy(heroEntry->role, roles[i], 1023); 
            heroEntry->next = NULL;

            if (head == NULL) {
                head = tail = heroEntry;
            } else {
                tail->next = heroEntry;
                tail = heroEntry;
            }
        }
    }
}

    
    fclose(file);
    return head;
}


void calculate(Hero *hero) {
    if (strncmp(hero->role, "TANK", 4) == 0) {
        hero->sp = hero->hp;
    } else if (strncmp(hero->role, "ASSASSIN", 8) == 0) {
        hero->sp = hero->pa;
    } else if (strncmp(hero->role, "FIGHTER", 7) == 0) {
        hero->sp = hero->hp + hero->pa;
    } else if (strncmp(hero->role, "MM", 2) == 0) {
        hero->sp = hero->pa * hero->as;
    } else if (strncmp(hero->role, "MAGE", 4) == 0) {
        hero->sp = hero->mana / hero->mreg;
    } else if (strncmp(hero->role, "SUPPORT", 7) == 0) {
        hero->sp = hero->mreg + hero->hp;
    } else {
        hero->sp = 0;
    }
}

void print(Hero *heroes, char * argu) {
    Hero *bestHeroes[6] = {NULL};
    for (int i = 0; i < 6; i++) {
        bestHeroes[i] = NULL;
    }

    Hero *current = heroes;
    while (current) {
        int roleIndex = -1;
        if (strncmp(current->role, "TANK", 4) == 0) roleIndex = 0;
        else if (strncmp(current->role, "ASSASSIN", 8) == 0) roleIndex = 1;
        else if (strncmp(current->role, "FIGHTER", 7) == 0) roleIndex = 2;
        else if (strncmp(current->role, "MM", 2) == 0) roleIndex = 3;
        else if (strncmp(current->role, "MAGE", 4) == 0) roleIndex = 4;
        else if (strncmp(current->role, "SUPPORT", 7) == 0) roleIndex = 5;

        if (roleIndex != -1) {
            if (bestHeroes[roleIndex] == NULL || bestHeroes[roleIndex]->sp < current->sp) {
                bestHeroes[roleIndex] = current;
            }
        }
        current = current->next;
    }

    for (int i = 0; i < 6; i++) {
        if(argu != NULL){
            if(strcmp(bestHeroes[i]->role, argu)==0){
                printf("Hero dengan role %s adalah %s dengan skill points %.2f\n",
                   bestHeroes[i]->role, bestHeroes[i]->nama, bestHeroes[i]->sp);
                break;
            }
        }else{
            if (bestHeroes[i]) {
            printf("Hero dengan role %s adalah %s dengan skill points %.2f\n",
                   bestHeroes[i]->role, bestHeroes[i]->nama, bestHeroes[i]->sp);
        }
    }
}
}
void free_hero(Hero *list) {
    while (list) {
        Hero *tmp = list;
        list = list->next;
        free(tmp);
    }
}


void download(){
    FILE *file = fopen("Data Mobile Legend", "r");
    if (file) {
        fclose(file);
        return;
    }
    system("kaggle datasets download -d irwansyah10010/mobilelegend");
    system("unzip mobilelegend.zip");
}


int main(int ar, char *arv[]){
    download();
    const char * path2 = "Data Mobile Legend/Atribut/atribut-hero.csv";
    const char * path = "Data Mobile Legend/identitas.csv";
    Hero *heroes = read(path, 0);
    Hero *attributes = read(path2, 1);

 


 Hero *currentAttr = attributes;
    while (currentAttr) {
        Hero *currentHero = heroes;
        while (currentHero) {
            if (strcmp(currentHero->nama, currentAttr->nama) == 0) {
                currentHero->hp = currentAttr->hp;
                currentHero->pa = currentAttr->pa;
                currentHero->as = currentAttr->as;
                currentHero->mana = currentAttr->mana;
                currentHero->mreg = currentAttr->mreg;
            }
            currentHero = currentHero->next;
        }
        currentAttr = currentAttr->next;
    }

     Hero *tempHero = heroes;
    while (tempHero) {
        calculate(tempHero);
        tempHero = tempHero->next;
    }



    if(ar>1){
        print(heroes, arv[1]);
    }else{
        print(heroes, NULL);
    }
    
    


    free_hero(heroes);
    free_hero(attributes);


    return 0;
}

```

- a. Membuat ayamgoreng.c yang dapat mendownload file dataset dengan kaggle dan dapat mengunzip/mengekstrak file tersebut
Sebelum membuat kode yang menggunakan kaggle, diperlukan python3 dan kaggle sehingga keduanya harus diinstal terlebih dahulu di sistem operasi. Selain itu diperlukan pula akun kaggle beserta token yang nantinya digunakan dalam pendownloadan.
Berikut adalah kode untuk mendownload dan menginstal python3:
```
sudo apt-get install -y python3-pip
```
Lalu selanjutnya kode untuk mendownload dan menginstal kaggle:
```
sudo apt install kaggle
```
Diperlukan pula folder .kaggle dalam root, berikut ini adalah kode untuk membuat folder tersebut:
```
mkdir -p /root/.kaggle
```

Lalu buat akun kaggle dan generate token. Setelah token didapat, masukkan token kaggle ke ~/.kaggle di dalam root:
```
mv Downloads/kaggle.json /root/.kaggle/ 
```
Asumsikan Downloads adalah lokasi awal kaggle.json

Permission atau izin didalam root/.kaggle/kaggle.json juga diperlukan maka:
```
chmod 600 /root/.kaggle/kaggle.json
```
Note:
- Apabila perintah tidak diperbolehkan sistem operasi, gunakan sudo
- Apabila gcc dan unzip belum diunduh, lakukan pula kode berikut:
```
sudo apt install gcc
sudo apt install unzip
```

Barulah program dapat dijalankan:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
```
Blok kode tersebut berguna untuk mengimpor library yang dibutuhkan program kedepannya. 

```
void download(){
    FILE *file = fopen("Data Mobile Legend", "r");
    if (file) {
        fclose(file);
        return;
    }
    system("kaggle datasets download -d irwansyah10010/mobilelegend");
    system("unzip mobilelegend.zip");
}
```
Untuk mendownload dataset, dibuatlah suatu fungsi. Fungsi tersebut mulanya akan mengecek apabila file data set (folder Data Mobile Legend) sudah ada dan dapat dibaca maka fungsi akan berhenti (return), apabila belum aada maka fungsi akan mendownload file dengan menggunakan perintah system yang didalamnya mengandung perintah kaggle (sesuai permintaan soal). Selain mendownload, fungsi juga akan mengunzip file dataset yang telah didownload dengan 'system("unzip mobilelegend.zip");'

Berikut adalah contoh pemanggilan fungsi download dalam fungsi utama:
```
int main(){
download();
...
}
```

- b. Program ayamgoreng.c harus dapat menghitung skill point hero berdasarakan rumus di soal dan mengeluarkan hero terbaik dari setiap role ataupun dari semua role

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Hero{
    char nama[1024];
    char role[1024];
    int hp;
    int pa;
    float as;
    int mana;
    float mreg;
    float sp;
    struct Hero *next;
} Hero;
```
Blok kode tersebut merupakan bagian dari program untuk mengimpor semua library yang diperlukan dan mendeklarasikan/menginisialisasikan sebuah struct yang nantinya digunakan untuk setiap Hero dari data. Struct ini juga menggunakan konsep seperti list, sehingga nantinya bisa menunjuk pada hero selanjutnya.

```
Hero* read(const char *filename, int isAttributeFile) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        return NULL;
    }

    Hero *head = NULL, *tail = NULL;
    char buffer[1024];
    int isFirstLine = 1; 
    if(isAttributeFile){
        while (fgets(buffer, sizeof(buffer), file)) {
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        Hero *newHero = (Hero*)malloc(sizeof(Hero));
        if (!newHero) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(file);
            return head;
        }
        memset(newHero, 0, sizeof(Hero)); 

        char *token;
        int colIndex = 0;
        token = strtok(buffer, ";");
        while (token != NULL) {
            switch(colIndex) {
                case 0: 
                    strncpy(newHero->nama, token, 1023);
                    break;
                case 1:
                    newHero->hp = atoi(token);
                    break;
                case 3:
                    newHero->pa = atoi(token);
                    break;
                case 5: 
                    newHero->as = atof(token);
                    break;
                case 7: 
                    newHero->mana = atoi(token);
                    break;
                case 8: 
                    newHero->mreg = atof(token);
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ";");
            colIndex++;
        }

        newHero->next = NULL;
        if (head == NULL) {
            head = tail = newHero;
        } else {
            tail->next = newHero;
            tail = newHero;
        }
    }
    }else { 
    while (fgets(buffer, sizeof(buffer), file)) {
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        Hero *newHero = NULL;
        char *token;
        int colIndex = 0;
        char *roles[2] = {NULL, NULL}; 
        int roleCount = 0;

        token = strtok(buffer, ";");
        while (token != NULL) {
            if (colIndex == 0) {  
                newHero = (Hero*)malloc(sizeof(Hero));
                if (!newHero) {
                    fprintf(stderr, "Memory allocation failed\n");
                    fclose(file);
                    return head;
                }
                memset(newHero, 0, sizeof(Hero));
                strncpy(newHero->nama, token, 1023);
            } else if (colIndex == 1) { 
                char *slash = strchr(token, '/');
                if (slash) {  
                    *slash = '\0'; 
                    roles[0] = token;
                    roles[1] = slash + 1;
                    roleCount = 2;
                } else {
                    roles[0] = token;
                    roleCount = 1;
                }
            }
            token = strtok(NULL, ";");
            colIndex++;
        }

        for (int i = 0; i < roleCount; i++) {
            Hero *heroEntry = i == 0 ? newHero : (Hero*)malloc(sizeof(Hero));
            if (i > 0) {  
                memcpy(heroEntry, newHero, sizeof(Hero));
            }
            strncpy(heroEntry->role, roles[i], 1023); 
            heroEntry->next = NULL;

            if (head == NULL) {
                head = tail = heroEntry;
            } else {
                tail->next = heroEntry;
                tail = heroEntry;
            }
        }
    }
}

    
    fclose(file);
    return head;
}
```
Fungsi tersebut adalah fungsi yang berguna untuk membaca file `.csv` yang berisikan hero berserta rolenya dan hero berserta atributnya. Pertama fungsi akan mengambil string filename yang merupakan nama file yang akan dibaca, kemudian juga akan mengambil integer isAttributefile yang gunanya sebagai flag atau boolean untuk menentukan apakah file yang sedang dibaca adalah file yang berisikan nama dan role atau nama beserta atribut hero (karena perlu membaca dua file dengan format atau data yang harus diambil berbeda lokasi kolomnya). Kemudian file filename dibuka untuk dibaca. Lalu deklarasikan struct Hero bernama head dan tail yang nantinya akan menjadi list dari hero. Apabila yang dibuka adalah file atribut maka fungsi akan membaca data dari kolom index ke 0, 1, 3, 5, 7, 8. Kolom dari file '.csv' dipisahkan melalui titik koma (;) maka dibuatlah suatu mekanisme dimana fungsi akan membaca setiap line (while (fgets(buffer, sizeof(buffer), file)), lalu setiap barisnya akan membaca setiap kolom dengan token hingga kolom habis (while (token != NULL)). Mengapa hanya kolom 0, 1, 3, 5, 7, 8? karena data yang diperlukan untuk menghitung skill point dari setiap hero hanya membutuhkan data pada kolom-kolom tersebut. Setelah kolom-kolom dibaca, hasil bacaan dimasukkan ke dalam suatu mekanisme untuk membuat list dari hero dan kemudian di return kembali ke pemanggil fungsi. 

Apabila yang dibuka adalah file berisikan nama dan role hero maka sama seperti sebelumnya fungsi akan membaca setiap line dan membaca kolom dari line tersebut, hanya saja kolom yang dibaca hanya kolom index 0 dan ke 1, karena pada file tersebut hanya diperlukan data pada kolom index 0 (nama hero) dan kolom index 1 (role hero). Kemudian apabila semua data sudah dibaca, return list hero ke pemanggil fungsi. Untuk pembacaan, keduanya (atribut dan file nama role) dibaca dari line kedua (line pertama diskip) karena line pertama hanya mengandung nama-nama kolom, bukan data yang dibutuhkan.

```
    while (fgets(buffer, sizeof(buffer), file)) {
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }
```
Untuk pemanggilan fungsi di fungsi main dapat dilakukan seperti ini:
```
    const char * path2 = "Data Mobile Legend/Atribut/atribut-hero.csv";
    const char * path = "Data Mobile Legend/identitas.csv";

    Hero *heroes = read(path, 0);
    Hero *attributes = read(path2, 1);

```

```
void calculate(Hero *hero) {
    if (strncmp(hero->role, "TANK", 4) == 0) {
        hero->sp = hero->hp;
    } else if (strncmp(hero->role, "ASSASSIN", 8) == 0) {
        hero->sp = hero->pa;
    } else if (strncmp(hero->role, "FIGHTER", 7) == 0) {
        hero->sp = hero->hp + hero->pa;
    } else if (strncmp(hero->role, "MM", 2) == 0) {
        hero->sp = hero->pa * hero->as;
    } else if (strncmp(hero->role, "MAGE", 4) == 0) {
        hero->sp = hero->mana / hero->mreg;
    } else if (strncmp(hero->role, "SUPPORT", 7) == 0) {
        hero->sp = hero->mreg + hero->hp;
    } else {
        hero->sp = 0;
    }
}
```
Fungsi tersebut berguna untuk menghitung skill point tiap hero berdasarkan rolenya. Fungsi dipanggil dengan memasukkan variabel list dari hero. Setelah dipanggil, fungsi akan mengecek bila role dari suatu hero sesuai dengan suatu role maka skill point hero tersebut akan dihitung dengan rumus berdasarkan role tersebut. Misalkan apabila suatu hero rolenya adalah TANK maka skill point dari hero tersebut adalah sesuai dengan nilai dari hp hero tersebut. Contoh lain apabila role suatu hero adalah FIGHTER maka skill point dari hero tersebut dihasilkan dari penjumlahan antara hp dan physical attack hero tersebut. Role dan rumus penghitungan skill point sudah ditentukan di soal. Berikut ini adalah contoh pemanggilan fungsi calculate:

```
  Hero *tempHero = heroes;
    while (tempHero) {
        calculate(tempHero);
        tempHero = tempHero->next;
    }

```
Suatu list of hero dibuat terlebih dahulu untuk menunjuk pada list heroes. Kemudian selama tempHero memiliki value (ada isinya) maka calculate tempHero, lalu tempHero nilainya akan menjadi tempHero selanjutnya. Jadi selama perulangan, tempHero akan dicalculate lalu pindah ke sebelahnya, calculate lagi, pindah ke selanjutnya, dan seterusnya. 

```
void print(Hero *heroes, char * argu) {
    Hero *bestHeroes[6] = {NULL};
    for (int i = 0; i < 6; i++) {
        bestHeroes[i] = NULL;
    }

    Hero *current = heroes;
    while (current) {
        int roleIndex = -1;
        if (strncmp(current->role, "TANK", 4) == 0) roleIndex = 0;
        else if (strncmp(current->role, "ASSASSIN", 8) == 0) roleIndex = 1;
        else if (strncmp(current->role, "FIGHTER", 7) == 0) roleIndex = 2;
        else if (strncmp(current->role, "MM", 2) == 0) roleIndex = 3;
        else if (strncmp(current->role, "MAGE", 4) == 0) roleIndex = 4;
        else if (strncmp(current->role, "SUPPORT", 7) == 0) roleIndex = 5;

        if (roleIndex != -1) {
            if (bestHeroes[roleIndex] == NULL || bestHeroes[roleIndex]->sp < current->sp) {
                bestHeroes[roleIndex] = current;
            }
        }
        current = current->next;
    }

    for (int i = 0; i < 6; i++) {
        if(argu != NULL){
            if(strcmp(bestHeroes[i]->role, argu)==0){
                printf("Hero dengan role %s adalah %s dengan skill points %.2f\n",
                   bestHeroes[i]->role, bestHeroes[i]->nama, bestHeroes[i]->sp);
                break;
            }
        }else{
            if (bestHeroes[i]) {
            printf("Hero dengan role %s adalah %s dengan skill points %.2f\n",
                   bestHeroes[i]->role, bestHeroes[i]->nama, bestHeroes[i]->sp);
        }
    }
}
}
```
Merupakan suatu fungsi untuk menampilkan hero terbaik. Apabila program dijalankan dengan tanpa argumen, maka program akan mengeluarkan hero terbaik sesuai dengan role-rolenya. Apabila program dijalankan dengan argumen yang menunjukkan role dari suatu hero, maka program (fungsi) akan menampilkan hero terbaik dari role tersebut. Fungsi akan dipanggil dan membutuhkan variabel list dari hero dan string argument sebagai data atau masukkan dari fungsi. Kemudian fungsi akan membuat suatu array dari Hero untuk menampung hero-hero terbaik dari setiap rolenya (6 role, jadi array Hero dengan 6 Hero didalamnya), dan diberi nilai NULL atau kosong diawalnya (inisialisasi). Lalu fungsi juga membuatu suatu list of Hero dengan nama current untuk menampung isi dari list hero inputan. Selama list current tersebut masih terdapat isinya (tidak bernilai NULL) maka program didalamnya dilakukan terus menerus (looping). Program di dalam looping pada intinya adalah pengecekan hero terbaik tiap rolenya dengan membuat suatu integer roleIndex, suatu role dari hero ditandai dengan angka roleIndex misalkan role MAGE akan ditandai dengan roleIndex 4. Integer roleIndex nantinya akan dipakai sebagai indexing dari list hero terbaik (bestHeroes), sehingga komparasi value skill point tiap heronya akan dibandingkan sesuai dengan role nya (misalkan MAGE maka akan dicompare dengan bestHeroes[4] apabila ternyata hero sekarang memiliki skill point yang lebih banyak, maka data dari hero sekarang (current) dimasukkan ke bestHeroes[4], begitu seterusnya hingga data habis). Setelah perulangan tersebut dibuat lagi suatu perulangan supaya ada integer i untuk indexing (jadi bisa mencari hero dari i = 0 hingga i = 5 dan juga bisa untuk menampilkan hero dari index ke 0 hingga 5). Apabila terdapat argumen (argumen bernilai tidak NULL) maka masuk ke pengondisian pertama dan fungsi akan mengecek, apabila role hero pada index sekarang sesuai dengan argumen maka tampilkan pesan 
"Hero dengan role %s adalah %s dengan skill points %.2f\n", dengan %s pertama adalah role hero dan %s kedua adalah nama hero dan %.2f adalah skill poiht. Pengondisian pertama dilakukan terus hingga role hero pada index tersebut sesuai dengan argumen (linear searching). Setelah role yang sesuai dengan argumen ditemukan maka keluar dari loop. Apabila ternyata tidak ada argumen (argumen = NULL) maka tampilkan hero terbaik setiap rolenya dengan format pesan seperti sebelumnya. 

Berikut adalah contoh pemanggilan fungsi tersebut:
```
  if(ar>1){
        print(heroes, arv[1]);
    }else{
        print(heroes, NULL);
    }
```
Terdapat pengondisian, apabila jumlah argumen yang dimasukkan saat program dijalankan lebih dari satu, maka fungsi yang dipanggil adalah 'print(heroes, arv[1])', dengan heroes adalah list dari hero dan arv[1] adalah argumen pada index ke satu (setelah ./ayamgoreng). Apabila jumlah argumen hanya satu maka 'print(heroes, NULL)'.

```
void free_hero(Hero *list) {
    while (list) {
        Hero *tmp = list;
        list = list->next;
        free(tmp);
    }
}
```
Selanjutnya dibuatlah fungsi untuk membersihkan (membebaskan memory dari) seluruh dynamic array atau dynamic list dari memory. Fungsi akan dipanggil dan diberi masukkan suatu dynamic array, lalu fungsi tersebut akan membebaskan isi dari list atau dynamic array tersebut.

Berikut adalah contoh pemanggilan fungsi free_hero:
```
   free_hero(heroes);
    free_hero(attributes);
```
Pemanggilan fungsi tersebut digunakkan untuk membersihkan/membebaskan list heroes dan list attributes. 

Fungsi utama:
```
int main(int ar, char *arv[]){
    download();
    const char * path2 = "Data Mobile Legend/Atribut/atribut-hero.csv";
    const char * path = "Data Mobile Legend/identitas.csv";
    Hero *heroes = read(path, 0);
    Hero *attributes = read(path2, 1);

 


 Hero *currentAttr = attributes;
    while (currentAttr) {
        Hero *currentHero = heroes;
        while (currentHero) {
            if (strcmp(currentHero->nama, currentAttr->nama) == 0) {
                currentHero->hp = currentAttr->hp;
                currentHero->pa = currentAttr->pa;
                currentHero->as = currentAttr->as;
                currentHero->mana = currentAttr->mana;
                currentHero->mreg = currentAttr->mreg;
            }
            currentHero = currentHero->next;
        }
        currentAttr = currentAttr->next;
    }

     Hero *tempHero = heroes;
    while (tempHero) {
        calculate(tempHero);
        tempHero = tempHero->next;
    }



    if(ar>1){
        print(heroes, arv[1]);
    }else{
        print(heroes, NULL);
    }
    
    


    free_hero(heroes);
    free_hero(attributes);


    return 0;
}
```
Pertama fungsi utama harus mendeklarasikan jumlah argumen (int ar) dan array of string argumen (char * arv[]). Kemudian fungsi utama akan memanggil fungsi untuk mendownload dan mengunzip file dataset (sesuai subtask sebelumnya). Setelah mendownload, fungsi utama akan mendeklarasikan path dari file dan membaca file-file tersebut sesuai dengan path diretorynya. Setelah dibaca dibuat program untuk menyatukan antara list attributes dengan list heroes.
Berikut adalah program tersebut:
```
 Hero *currentAttr = attributes;
    while (currentAttr) {
        Hero *currentHero = heroes;
        while (currentHero) {
            if (strcmp(currentHero->nama, currentAttr->nama) == 0) {
                currentHero->hp = currentAttr->hp;
                currentHero->pa = currentAttr->pa;
                currentHero->as = currentAttr->as;
                currentHero->mana = currentAttr->mana;
                currentHero->mreg = currentAttr->mreg;
            }
            currentHero = currentHero->next;
        }
        currentAttr = currentAttr->next;
    }
```
Mulanya dibuat list Hero bernama currentAttr untuk menunjuk list attributes. Lalu selama nilai dari currentAttr tidak NULL (ada isinya) maka lakukan perulangan. Di dalam perulangan tersebut dibuat suatu list Hero bernama currentHero untuk menunjuk list heroes, lalu selama currentHero ada isinya maka komparasikan nama dari currentHero dan nama dari currentAttr (karena satu-satunya data yang dapat menggabungkan data dari file heroes dan attribut hero hanya nama dari hero tersebut). Apabila hasil komparasi "sama" maka masukkan seluruh data-data attribut hero (currentAttr) ke dalam variabel-variabel attribut dari hero (currentHero). Apabila sudah tidak sesuai maka currentHero akan berpindah ke selanjutnya (next). Loop kedua dilakukan terus menerus hingga data currentHero habis, lalu currentAttr akan berpindah ke selanjutnya (next) dan loop kedua diulang lagi. Looping akan berhenti sepenuhnya ketika currentAttr habis. Disini dibutuhkan dua looping karena bisa saja terdapat satu hero yang memiliki dua struct karena rolenya dua (1 hero dengan 2 role akan dijadikan 2 struct hero dengan nama yang sama tetapi rolenya berbeda).

Setelah data digabungkan maka hitung skill point tiap heronya. Apabila penghitungan telah selesai, tampilkan hero beserta pesan dengan berdasarkan argumen yang dimasukkan. Di akhir fungsi utama, bebaskan seluruh list dengan pemanggilan fungsi "free_hero" lalu akhiri program (return 0).

Bukti Screenshot a dan b:

![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/62c98bae-a0a3-4467-a2bc-c0a187c6fa74)



- c. Membuat dockerfile untuk menjalankan program ayamgoreng.c
Full Code Dockerfile:
```
FROM ubuntu:22.04

WORKDIR /app

RUN apt-get update 
RUN apt-get install -y gcc
RUN apt-get install -y python3-pip
RUN apt-get install -y unzip
RUN pip3 install kaggle


RUN mkdir -p /root/.kaggle
RUN echo '{"username":"reynandriel","key":"2ad7ef98289c27683b38802b7ad03d71"}' > kaggle.json 
RUN mv kaggle.json /root/.kaggle/
RUN chmod 600 /root/.kaggle/kaggle.json

COPY ayamgoreng.c /app/

RUN gcc -o ayamgoreng ayamgoreng.c

CMD ["./ayamgoreng"]
```
Pertama ambil dockerimage sesuai dengan ubuntu versi atau dengan tag 22.04. Lalu inisialisasikan directory atau lokasi program akan dijalankan yaitu pada /app dengan menggunakan WORKDIR. Kemudian untuk menjalankan program diperlukan "update" untuk memperbarui hingga versi terbaru, kemudian menginstall gcc, python3-pip, unzip, dan kaggle. Penginstalan dan update dilakukan dengan perintah RUN. 

Blok kode selanjutnya digunakan untuk memenuhi kebutuhan kaggle, sesuai subtask a diperlukan folder pada root, token kaggle, pemindahan token kaggle, dan pengaturan permission. Hanya saja disini terdapat perbedaan. Daripada mendownload file token kaggle, disini dimasukkan serangkaian teks ke dalam kaggle.json (file token kaggle) yakni '{"username":"reynandriel","key":"2ad7ef98289c27683b38802b7ad03d71"}'. Isi teks tersebut adalah username beserta token key saya (pengerja soal) supaya mempermudah dalam penerapannya (walaupun lebih tidak aman). Jadi kaggle.json bukan didownload melainkan dibuat dan nantinya akan berisi teks tersebut. Pembuatan beserta pengisian kaggle.json digunakan perintah echo yang dijalankan dengan RUN apabila ingin dijalankan di dockerfile. 

Lalu dilakukan penduplikasian ayamgoreng.c ke dalam directory yang digunakan sebagai tempat container akan berjalan (work directory) yakni ke dalam folder app di dalam container.

Untuk menjalankan ayamgoreng.c maka file C harus dicompile terlebih dahulu dengan "gcc -o ayamgoreng ayamgoreng.c" dan dijalankan menggunakan RUN
Setelah dicompile, soal meminta apabila container dijalankan, maka program dijalankan secara default atau tanpa argument maka gunakan CMD untuk menjalankan program di dalam container sehingga terbentuklah serangkaian perintah "CMD ["./ayamgoreng"]".

Untuk menjalankan container, dockerimage harus dibuat terlebih dahulu. Berikut adalah cara membangun/membuat dockerimage dari dockerfile:
```
docker build -t dockerimage .
```
Note:
- Pastikan lokasi sekarang berada di lokasi yang memiliki ayamgoreng.c (saya disini menggunakan lokasi ./praktikum-modul-3-b24/task-1/)
- Pastikan docker juga sudah berjalan (apabila menggunakan docker desktop, pastikan docker desktop telah berjalan)

Kemudian jalankan dockerimage supaya menjadi container:
```
docker run dockerimage
```
Note:
- Pastikan lokasi sekarang juga berada di lokasi dockerimage dibuat
- Pastikan docker juga sudah berjalan (apabila menggunakan docker desktop, pastikan docker desktop telah berjalan)

Bukti Screenshot c:
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/ffc50e4b-c3d2-4c46-9bd5-60f12dfc5801)
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/371f22c9-61bc-4ae1-b91e-3a949e313ada)



- d. Mengupload/mengunggah file dockerimage ke dalam dockerhub
Untuk mengunggah ke dockerhub, diperlukan akun dockerhub terlebih dahulu. Setelah akun dockerhub terbuat, buat repository dockerhub dengan nama joki-ml-bang (sesuai permintaan soal). Kemudian login dockerhub di terminal sistem operasi dengan
```
docker login 
```
Setelah kode tersebut dimasukkan, maka user akan diminta untuk login. Maka isi sesuai akun dockerhub

Diperlukan pula untuk membuat tag pada dockerimage:
```
docker tag dockerimage rgx26/joki-ml-bang
```
dengan dokcerimage adalah nama dari docker image, rgx26 adalah username dan joki-ml-bang adalah nama repository

Kemudian untuk mengupload dockerimage ke dalam repository tersebut dapat dilakukan dengan kode seperti berikut:
```
docker push 
```

Bukti Screenshot d:
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/fc0e35fc-287a-4925-8818-1abaf9508686)
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/72709ab9-668d-4746-bafa-3eca3b4f8bbd)



- e. Membuat docker compose yang dapat menjalankan 6 kontainer setiap rolenya (./ayamgoreng TANK sebanyak 6 kali, ./ayamgoreng ASSASSIN sebanyak 6 kali, dan seterusnya)
Full code docker-compose.yml:
```
version: '3'

services:
  tank:
    image: dockerimage
    command: ["./ayamgoreng", "TANK"]
    scale: 6
    
  assassin:
    image: dockerimage
    command: ["./ayamgoreng", "ASSASSIN"]
    scale: 6

  fighter:
    image: dockerimage
    command: ["./ayamgoreng", "FIGHTER"]
    scale: 6

  marksman:
    image: dockerimage
    command: ["./ayamgoreng", "MM"]
    scale: 6

  mage:
    image: dockerimage
    command: ["./ayamgoreng", "MAGE"]
    scale: 6

  support:
    image: dockerimage
    command: ["./ayamgoreng", "SUPPORT"]
    scale: 6
```
Penjelasan:
```
version: '3'
```
Merupakan penginisialisasian versi dari docker-compose yang akan dipakai

Lalu tiap role heronya dibuat suatu service atau instance, misalkan role "TANK"
```
services:
  tank:
    image: dockerimage
    command: ["./ayamgoreng", "TANK"]
    scale: 6
```
Nama service di inisialisasi terlebih dahulu yakni 'tank', lalu image yang dipakai yakni 'dockerimage', command atau perintah yang dilakukan saat docker container dijalankan yakni "./ayamgoreng TANK" dan terakhir sesuai permintaan soal, setiap service (setiap role) dijalankan 6 kali. 

Bukti Screenshot e:
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/fb6f7ff0-1cd1-4914-baa0-9af635e093a4)
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/f5d1a510-fa4a-4fa1-81f0-b9a9a357248b)
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/faa51726-ad1a-4050-93a9-9579e9095b32)


- [Task 2 -BNWT](/task-2/) by Daffa,Rey,Fernando

#### Soal

Skill ini bisa mengabulkan segala permintaan apapun di dunia tersebut, mulai dari senjata, sihir, standpower, dsb. Semua dapat dibuat asalkan permintaan tersebut dibungkus dalam sebuah service docker container. Namun, dia tidak tahu service apa yang bagus untuk bisa merebut Morioh Cho dari Pucci. Dia pun meminta kalian, para warga Morioh Cho, untuk bekerjasama membantu Ben White menyusun docker compose nya.

- a. Buatlah 2 file, yaitu [server.c](./server.c) dan [client.c](./client.c).

  Disaat `server.c` pertama kali dijalankan, dia akan meminta prompt user untuk mengetikkan berapa banyak client yang akan dilayani (range 1-5).

  Disaat `client.c` pertama kali dijalankan, dia akan meminta prompt user untuk mengetikkan username. Username ini kemudian dikirim ke server, untuk di check apakah sudah ada sebelumnya atau tidak. Jika sudah ada, maka akan meminta prompt lagi untuk mengetikkan username sampai diterima oleh server.

- b. Ketika semua client yang dibutuhkan oleh server telah terkumpul, setiap client akan mulai diberikan prompt untuk mengetikkan service yang dibutuhkan, yaitu `Nama Container`, `Image yang Digunakan`, `Perintah Saat Kontainer Dimulai`, dan `Volume`. Urutan client yang mengirimkan request service ke server ditentukan dari waktu mereka register. Contoh: Jika username ragnar registrasi pertama kali, lalu diikuti username towel, maka ragnar yang akan mengirim request terlebih dahulu, lalu diikuti towel, dst.

- c. Setelah semua request service dari client terkumpul, server lalu menggabungkannya menjadi sebuah file docker compose. Lalu jalankan file docker compose yang dibuat tadi

- d. Tidak sampai situ, setelah docker compose sebelumnya selesai dijalankan, `client.c` akan meminta prompt lagi dari user. Prompt yang diberikan sesuai dengan point (b). Jika setiap client melakukan hal yang sama pada point (b), maka server akan menghasilkan file docker compose yang baru. Lalu container yang telah dijalankan sebelumnya akan dihancurkan, yang kemudian akan digantikan oleh service terbaru yang telah di request dari client tadi.

### Notes:

- Untuk komunikasi `client.c` dan `server.c`, kalian bebas mau menggunakan IPC jenis apapun.
- Dilarang menggunakan `system()``

#### Penjelasan
Full Code client.c
```
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct
{
    long message_type;
    char user[64];
    char nama[64];
    char image[64];
    char cmd[300];
    char volum[64];
    int login;
    int client_id;
    int repeat;
} Message;

void clr_buf() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}


int main() {
    key_t key = 911;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    char temp[64];
    Message msg;
    int current_client_id = 0;

    while (1) {
        printf("Masukkan username: ");
        scanf("%s", temp);
        strcpy(msg.user, temp);
        msg.message_type = 6;
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msgrcv(msgid, &msg, sizeof(msg), 7, 0);

        if (msg.login == 1) {
            break;
        }

        printf("Udah ada!\n");
    }
    
    printf("Passed as client id %d\n", msg.client_id);
    current_client_id = msg.client_id;
    
    while(1){
        msg.login = 0;
        if (msg.client_id > 1) printf("Waiting for client %d...\n", msg.client_id - 1);
        msgrcv(msgid, &msg, sizeof(msg), msg.client_id, 0);
        printf("Infokan kontainer yang ingin kamu buat\n");
        printf("Masukkan nama kontainer yang akan kamu buat : ");
        scanf("%s",msg.nama);
        printf("Masukkan image yang digunakan : ");
        scanf("%s",msg.image);
        clr_buf();
        printf("Masukkan perintah saat kontainer dimulai : ");
        scanf("%[^\n]s",msg.cmd);
        printf("Masukkan volume dari container : ");
        scanf("%s",msg.volum);
        msg.message_type=6;
        msgsnd(msgid,&msg,sizeof(msg),0);
        printf("Menunggu konfirmasi %d\n", current_client_id);
        msgrcv(msgid,&msg,sizeof(msg),current_client_id,0);
        printf("Kontainer berhasil dibuat\nApakah anda ingin membuat kontainer lagi? (1/0) : ");
        scanf("%d",&msg.repeat);
        msg.message_type=6;
        msgsnd(msgid,&msg,sizeof(msg),0);
        if (msg.repeat == 0) {
            break;
        }
    }
    
    return 0;
}
```


Penjelasan:
```
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
```
Merupakan sejumlah kode untuk mengimpor seluruh library yang diperlukan oleh program

```
typedef struct
{
    long message_type;
    char user[64];
    char nama[64];
    char image[64];
    char cmd[300];
    char volum[64];
    int login;
    int client_id;
    int repeat;
} Message;
```
Merupakan pendeklarasian suatu struktur (struct) dengan nama Message yang bertujuan untuk menampung segala variabel yang diperlukan dalam pengiriman pesan nantinya (karena satu pesan bisa beberapa variabel sekaligus). 

```
void clr_buf() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
```
Blok kode tersebut adalah fungsi pembersihan buffer input yang berguna untuk menghindari pembacaan input yang tidak diinginkan pada scanf() berikutnya.

```
int main() {
    key_t key = 911;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    char temp[64];
    Message msg;
    int current_client_id = 0;
```
Pada fungsi utama, pada mulanya diperlukan untuk membuat suatu kunci unik untuk message queue dan menginisialisasi message queue, serta menginisialisasi variabel untuk menyimpan informasi pengguna dan ID klien saat ini.

```
    while (1) {
        printf("Masukkan username: ");
        scanf("%s", temp);
        strcpy(msg.user, temp);
        msg.message_type = 6;
        msgsnd(msgid, &msg, sizeof(msg), 0);
        msgrcv(msgid, &msg, sizeof(msg), 7, 0);

        if (msg.login == 1) {
            break;
        }

        printf("Udah ada!\n");
    }
```
Berikutnya terdapat blok kode untuk meminta inputan user berupa username. Disini diperlukan loop karena bisa saja inputan username tidak sesuai (sudah ada) dan perlu penginputan username hingga inputan benar. Lalu dikirimkan username user dipassing ke server.c, dan dicek di server.c apakah sudah ada yang login dengan username yang sama atau belum. Selama server.c belum mengirim pesan, client.c akan menunggu hingga pesan terkirim. Bila belum ada (sesuai) maka server.c akan mengirim pesan dengan variabel login pada pesan (msg) bernilai 1. Lalu keluar dari loop. Bila tidak sesuai maka keluarkan "Udah ada!(enter)" dan loop berlanjut. 

```
    printf("Passed as client id %d\n", msg.client_id);
    current_client_id = msg.client_id;
```
Serangkaian kode diatas digunakan untuk mengecek saja (debugging) id client saat ini dengan menampilkan kalimat "Passed as client id (msg.client_id)", lalu nilai dari current_client_id disesuaikan dengan msg.client_id. Perlu diketahui bahwa pengiriman pesan oleh server.c sebelumnya juga telah memberikan client id kepada client.c yang nantinya berguna sebagai message_type dari client tersebut. 

```
while(1){
        msg.login = 0;
        if (msg.client_id > 1) printf("Waiting for client %d...\n", msg.client_id - 1);
        msgrcv(msgid, &msg, sizeof(msg), msg.client_id, 0);
        printf("Infokan kontainer yang ingin kamu buat\n");
        printf("Masukkan nama kontainer yang akan kamu buat : ");
        scanf("%s",msg.nama);
        printf("Masukkan image yang digunakan : ");
        scanf("%s",msg.image);
        clr_buf();
        printf("Masukkan perintah saat kontainer dimulai : ");
        scanf("%[^\n]s",msg.cmd);
        printf("Masukkan volume dari container : ");
        scanf("%s",msg.volum);
        msg.message_type=6;
        msgsnd(msgid,&msg,sizeof(msg),0);
        printf("Menunggu konfirmasi %d\n", current_client_id);
        msgrcv(msgid,&msg,sizeof(msg),current_client_id,0);
        printf("Kontainer berhasil dibuat\nApakah anda ingin membuat kontainer lagi? (1/0) : ");
        scanf("%d",&msg.repeat);
        msg.message_type=6;
        msgsnd(msgid,&msg,sizeof(msg),0);
        if (msg.repeat == 0) {
            break;
        }
    }
    
    return 0;
}
```
Selanjutnya diperlukan suatu loop untuk mengambil inputan berupa pesan yang ingin dimasukkan ke dalam docker-compose.yml. Pertama buat msg.login menjadi 0 nilainya karena sudah tidak diperlukan lagi. Lalu client.c mengecek apakah client id sekarang lebih dari 1, bila iya maka keluarkan "Waiting for client (msg.client_id-1)" untuk menunjukkan bahwa client.c sekarang sedang menunggu client.c lainnya (yang lebih dahulu urutannya). Bila client id sekarang adalah 1 maka langsung saja ke tahap berikutnya. Tahap berikutnya client.c akan menunggu pesan dari server.c untuk konfirmasi bahwa sekarang adalah gilirannya berjalan. Kemudian client.c akan meminta user untuk memasukkan nama container, image, command, dan volume yang ingin dimasukkan ke docker-compose.yml. Setelah terisi semua oleh user, kirimkan semua inputan tersebut dalam suatu Message ke server.c. Bila pengiriman telah selesai, keluarkan kalimat "Menunggu konfirmasi (client id sekarang)" untuk menandakan bahwa sekarang client.c sedang menunggu konfirmasi dari server.c dan tunggu pesan dari server.c (msgrcv). Setelah konfirmasi didapat, keluarkan "Kontainer berhasil dibuat (enter) Apakah anda ingin membuat kontainer lagi?(1/0) : " yang berguna untuk memberi tahu user bahwa kontainer telah berhasil dibuat dan menanyakan apakah ingin membuat kontainer lagi (loop berhenti atau tidak). Bila user memilih 1 maka loop tetap berjalan dalam client.c tersebut, apabila tidak maka looping akan berhenti (break) dan program client.c berhenti. Status lanjut atau tidak nya client.c juga perlu dikirimkan kepada server.c supaya server.c tahu apakah client tersebut masih lanjut atau tidak.


--------

Full code server.c
```
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    long message_type;
    char user[64];
    char nama[64];
    char image[64];
    char cmd[300];
    char volum[64];
    int login;
    int client_id;
    int repeat;
} Message;

int checkusername(Message msgc[5], Message msga, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(msgc[i].user, msga.user) == 0) {
            return 0;
        }
    }
    return 1;
}


void create_docker_compose(Message msg) {
    FILE *file = fopen("./docker-compose.yml", "w");
    if (!file) {
        perror("Failed to open docker-compose.yml");
        return;
    }
    fprintf(file, "version: '3'\nservices:\n");
    fprintf(file, "  %s:\n    image: %s\n    container_name: %s\n    command: \"%s\"\n    volumes:\n      - %s\n",
            msg.nama, msg.image, msg.nama, msg.cmd, msg.volum);
    fclose(file);
}

int main() {
    key_t key = 911;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    Message msg[5], msgf;
    int count = 0, jumlah;
    char pesan[256];

    printf("Enter number of clients (1-5): ");
    scanf("%d", &jumlah);

    while (count < jumlah) {
        printf("Waiting for client %d...\n", count + 1);
        msgrcv(msgid, &msgf, sizeof(msgf), 6, 0);
        printf("Client with username %s connected\n", msgf.user);
        if (checkusername(msg, msgf, count)) {
            strcpy(msg[count].user, msgf.user);
            printf("Username %s accepted\n", msg[count].user);
            msgf.login = 1;
            msgf.client_id = count + 1;
            msgf.message_type = 7;
            msgsnd(msgid, &msgf, sizeof(msgf), 0);
            printf("Confirmation sent to client %d\n", count + 1);
            count++;
        } else {
            msgf.login = 0;
            printf("Username %s rejected\n", msgf.user);
            msgf.message_type = 7;
            msgsnd(msgid, &msgf, sizeof(msgf), 0);
        }
    }
    for (int i = 1; i <= jumlah; i++) {
        msgf.message_type = i;
        msgsnd(msgid, &msgf, sizeof(msgf), 0);
        printf("Client %d notified\n", i);
        msgrcv(msgid, &msgf, sizeof(msgf), 6, 0);
        printf("Client %d responded\n", i);
        strcpy(msg[i-1].nama, msgf.nama);
        strcpy(msg[i-1].image, msgf.image);
        strcpy(msg[i-1].cmd, msgf.cmd);
        strcpy(msg[i-1].volum, msgf.volum);
        printf("Info from client %d saved, creating docker compose.\n", i);

        create_docker_compose(msg[i-1]);
    
        int status;
        pid_t pid = fork();
        if (pid == 0) {
            execlp("docker", "docker", "compose", "down", NULL);
            exit(0);
        } else while (wait(&status) > 0);
        pid = fork();
        if (pid == 0) {
            execlp("docker", "docker", "compose", "build", NULL);
            exit(0);
        } else while (wait(&status) > 0);
        pid = fork();
        if (pid == 0) {
            execlp("docker", "docker", "compose", "up", "-d", NULL);
            exit(0);
        } else while (wait(&status) > 0);
        printf("Finished creating docker compose, sending confirmation to client %d\n", i);
        msgf.message_type = i;
        msgsnd(msgid, &msgf, sizeof(msgf), 0);
        printf("Waiting for client %d to respond\n", i);
        msgrcv(msgid, &msgf, sizeof(msgf), 6, 0);
        printf("Client %d responded with %d\n", i, msgf.repeat);
        if (msgf.repeat != 0) i--;
    }
    printf("All clients are finished. Closing\n");
    
    msgctl(msgid, IPC_RMID, NULL);  // Clean up the message queue
    return 0;
}
```

Penjelasan:
```
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
```
Kode ini mengimpor library yang diperlukan untuk operasi input/output, komunikasi antar proses (IPC) melalui message queue, operasi string, dan beberapa fungsi standar lainnya.

```
typedef struct {
    long message_type;
    char user[64];
    char nama[64];
    char image[64];
    char cmd[300];
    char volum[64];
    int login;
    int client_id;
    int repeat;
} Message;
```
Definisi struktur Message digunakan untuk menyimpan informasi tentang pesan yang akan dikirim melalui antrean pesan. Struktur ini mencakup jenis pesan, username, nama kontainer, image Docker, command yang akan dijalankan, volume, dan beberapa informasi kontrol lainnya.

```
int checkusername(Message msgc[5], Message msga, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(msgc[i].user, msga.user) == 0) {
            return 0;  // User sudah ada
        }
    }
    return 1;  // User baru
}
```

Fungsi checkusername digunakan untuk memeriksa apakah username yang diinput oleh klien sudah terdaftar dalam sistem. Fungsi ini menerima tiga parameter: sebuah array msgc yang berisi data pengguna yang telah terdaftar sebelumnya, sebuah struktur msga yang berisi data pengguna saat ini, dan sebuah integer count yang menunjukkan jumlah pengguna yang telah terdaftar. Di dalam fungsi, terdapat loop for yang berjalan dari 0 hingga count - 1. Dalam setiap iterasi, fungsi membandingkan username yang ada di dalam array msgc dengan username dari pengguna saat ini menggunakan fungsi strcmp. Jika ditemukan kecocokan (artinya username sudah ada), fungsi akan langsung mengembalikan nilai 0, yang menandakan bahwa username tersebut sudah terpakai. Jika loop selesai dan tidak ditemukan username yang sama, fungsi mengembalikan nilai 1, yang berarti username tersebut belum terpakai dan valid untuk didaftarkan. Secara keseluruhan, fungsi ini membantu memastikan bahwa setiap username dalam sistem adalah unik.  

```
void create_docker_compose(Message msg) {
    FILE *file = fopen("./docker-compose.yml", "w");
    if (!file) {
        perror("Failed to open docker-compose.yml");
        return;
    }
    fprintf(file, "version: '3'\nservices:\n");
    fprintf(file, "  %s:\n    image: %s\n    container_name: %s\n    command: \"%s\"\n    volumes:\n      - %s\n",
            msg.nama, msg.image, msg.nama, msg.cmd, msg.volum);
    fclose(file);
}
```

Fungsi create_docker_compose digunakan untuk membuat file docker-compose.yml dan menulis konfigurasi untuk layanan Docker berdasarkan informasi yang diberikan dalam struktur Message. Jika gagal membuka file, program akan mencetak pesan kesalahan. Kemudian, fungsi ini menulis versi docker-compose, nama layanan, image Docker, perintah yang akan dijalankan, dan volume yang digunakan ke dalam file. Akhirnya, file ditutup setelah selesai menulis.


```
int main() {
    key_t key = 911;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    Message msg[5], msgf;
    int count = 0, jumlah;
    char pesan[256];
```
Fungsi main() diawali dengan inisialisasi key_t key yang diberi nilai 911. Nilai ini digunakan sebagai identifier unik untuk antrean pesan yang akan diakses atau dibuat. Kemudian, fungsi msgget dipanggil dengan parameter key dan 0666 | IPC_CREAT. Ini mengatur perizinan untuk mengakses antrean pesan dan menciptakannya jika belum ada. Variabel msgid yang dihasilkan adalah identifier untuk antrean pesan yang akan digunakan untuk komunikasi antar proses. Array Message msg[5] diinisialisasi untuk menyimpan pesan dari hingga lima klien, sementara msgf adalah variabel untuk menyimpan pesan yang sedang diproses. Variabel count diinisialisasi dengan nilai 0, digunakan untuk menghitung jumlah klien yang terhubung, dan jumlah akan menyimpan batas maksimal klien yang dapat terhubung, yang akan diminta dari pengguna nantinya. Array pesan[256] disediakan untuk menampung pesan teks yang mungkin digunakan dalam proses komunikasi.

```
    printf("Enter number of clients (1-5): ");
    scanf("%d", &jumlah);

    while (count < jumlah) {
        printf("Waiting for client %d...\n", count + 1);
        msgrcv(msgid, &msgf, sizeof(msgf), 6, 0);
        printf("Client with username %s connected\n", msgf.user);
        if (checkusername(msg, msgf, count)) {
            strcpy(msg[count].user, msgf.user);
            printf("Username %s accepted\n", msg[count].user);
            msgf.login = 1;
            msgf.client_id = count + 1;
            msgf.message_type = 7;
            msgsnd(msgid, &msgf, sizeof(msgf), 0);
            printf("Confirmation sent to client %d\n", count + 1);
            count++;
        } else {
            msgf.login = 0;
            printf("Username %s rejected\n", msgf.user);
            msgf.message_type = 7;
            msgsnd(msgid, &msgf, sizeof(msgf), 0);
        }
    }

```
Pada bagian kode ini, server mulai dengan meminta pengguna untuk memasukkan jumlah klien yang akan terhubung, dengan batas maksimum 5 klien. Setelah itu, server masuk ke dalam loop yang akan berjalan sejumlah jumlah klien yang dimasukkan.

Dalam setiap iterasi loop:

1. Server mencetak pesan yang menunggu koneksi dari klien selanjutnya.
2. msgrcv digunakan untuk menerima pesan dari klien dengan message type 6. Pesan ini diharapkan mengandung informasi username dari klien yang terhubung.
3. Setelah menerima pesan, server mencetak bahwa klien dengan username tersebut telah terhubung.
4. Server kemudian memanggil fungsi checkusername untuk memastikan bahwa username tersebut belum digunakan oleh klien lain yang telah terdaftar. Jika username belum ada:
- Server menyalin username tersebut ke array msg yang menyimpan data klien.
- Menandai klien sebagai terverifikasi (login = 1).
- Menetapkan ID klien (client_id).
- Mengirimkan konfirmasi kembali ke klien menggunakan msgsnd dengan message type 7, menandakan bahwa klien telah berhasil terdaftar.
- Mencetak pesan konfirmasi terkirim dan meningkatkan counter count.
5. Jika username sudah ada, server:
- Menetapkan status login klien sebagai 0 untuk menunjukkan penolakan.
- Mengirim pesan penolakan kembali ke klien menggunakan msgsnd dengan message type 7.
- Mencetak bahwa username telah ditolak.

Loop ini berlanjut sampai semua klien yang diharapkan telah terhubung dan diproses. Kode ini efektif mengatur koneksi awal antara server dan klien dalam konteks setup server multi-klien.

```
 for (int i = 1; i <= jumlah; i++) {
        msgf.message_type = i;
        msgsnd(msgid, &msgf, sizeof(msgf), 0);
        printf("Client %d notified\n", i);
        msgrcv(msgid, &msgf, sizeof(msgf), 6, 0);
        printf("Client %d responded\n", i);
        strcpy(msg[i-1].nama, msgf.nama);
        strcpy(msg[i-1].image, msgf.image);
        strcpy(msg[i-1].cmd, msgf.cmd);
        strcpy(msg[i-1].volum, msgf.volum);
        printf("Info from client %d saved, creating docker compose.\n", i);

        create_docker_compose(msg[i-1]);
    
        int status;
        pid_t pid = fork();
        if (pid == 0) {
            execlp("docker", "docker", "compose", "down", NULL);
            exit(0);
        } else while (wait(&status) > 0);
        pid = fork();
        if (pid == 0) {
            execlp("docker", "docker", "compose", "build", NULL);
            exit(0);
        } else while (wait(&status) > 0);
        pid = fork();
        if (pid == 0) {
            execlp("docker", "docker", "compose", "up", "-d", NULL);
            exit(0);
        } else while (wait(&status) > 0);
        printf("Finished creating docker compose, sending confirmation to client %d\n", i);
        msgf.message_type = i;
        msgsnd(msgid, &msgf, sizeof(msgf), 0);
        printf("Waiting for client %d to respond\n", i);
        msgrcv(msgid, &msgf, sizeof(msgf), 6, 0);
        printf("Client %d responded with %d\n", i, msgf.repeat);
        if (msgf.repeat != 0) i--;
    }
    printf("All clients are finished. Closing\n");
    
    msgctl(msgid, IPC_RMID, NULL);  // Clean up the message queue
    return 0;
}
```
Dalam potongan kode ini, kita melihat proses interaksi antara server dan klien di mana server mengelola pembuatan dan pengelolaan kontainer Docker untuk setiap klien secara berurutan. Berikut penjelasan dari setiap bagian kode:

1. Pengiriman dan Penerimaan Pesan: Loop dimulai dengan mengirim pesan kepada klien menggunakan msgsnd() yang memberi tahu klien bahwa gilirannya telah tiba untuk mengirim informasi. msgrcv() digunakan untuk menerima respons dari klien, yang berisi data detail kontainer seperti nama, image, command, dan volume.
2. Pemrosesan Informasi Kontainer: Setelah menerima data dari klien, server menyimpan informasi tersebut ke dalam array msg dan menampilkan pesan bahwa data klien telah disimpan dan proses pembuatan docker compose akan dimulai.
3. Pembuatan Docker Compose: Fungsi create_docker_compose() dipanggil dengan data kontainer klien untuk menulis file docker-compose.yml yang berisi konfigurasi yang diperlukan untuk menjalankan kontainer tersebut.
4. Pengelolaan Kontainer Docker: Server kemudian melakukan serangkaian operasi Docker menggunakan pemanggilan proses fork() dan execlp(). Ini termasuk menurunkan kontainer yang sedang berjalan jika ada (docker compose down), membangun kontainer baru dari konfigurasi (docker compose build), dan memulai kontainer tersebut (docker compose up -d). Setiap perintah dijalankan di proses anak yang terpisah, dan server menunggu setiap proses anak selesai.
5. Konfirmasi ke Klien dan Siklus Ulangi: Setelah kontainer berhasil dibuat dan dijalankan, server mengirim konfirmasi ke klien bahwa prosesnya telah selesai. Server kemudian menunggu respons dari klien apakah mereka ingin mengulangi proses atau tidak (msgf.repeat). Jika klien meminta pengulangan, loop akan mengulang iterasi untuk klien tersebut.
6. Penutupan dan Pembersihan: Setelah semua klien selesai dan tidak ada lagi yang mengulangi proses, server menampilkan pesan bahwa semua klien telah selesai dan proses server akan ditutup. Server kemudian membersihkan antrean pesan menggunakan msgctl() dengan perintah IPC_RMID untuk menghapus antrean pesan tersebut dari sistem.

Kode ini secara efektif menunjukkan bagaimana sebuah server dapat mengelola permintaan multipel dari klien dalam setting yang membutuhkan koordinasi dan manajemen sumber daya, seperti dalam kasus pembuatan dan pengelolaan kontainer Docker.


- [Task 3 - Volume Expansion](/task-3/) by Rey
#### Soal
Volume Expansion

Dhafin sangat suka dengan anime Jujutsu Kaisen. Suatu hari dia tertidur dan mendapati dirinya menjadi Yuji. Dia lalu memikirkan nama teknik yang dapat melawan domain milik Sukuna, agar dirinya tidak bernasib seperti Gojo. Dhafin lalu memikirkan suatu teknik. Mengapa tidak menyegel Sukuna kedalam docker container yang terisolasi dari dunia luar. Dengan referensi tersebut, Dhafin ingin membuat project, dimana 2 buah container atau lebih dapat saling mengirim pesan satu sama lain. Dhafin menamai teknik ini _**[!!! VOLUME EXPANSION !!!].**_ Saat Dhafin sudah hampir selesai membuat teknik _**[!!! VOLUME EXPANSION !!!],**_ tiba-tiba dia diserang petir Kashimo sehingga dia bangun dari tidurnya. Dhafin kemudian sangat menyesal karena algoritma pembuatan _**[!!! VOLUME EXPANSION !!!]**_ sudah hilang di mimpinya. Jadi tolong wujudkan mimpi Dhafin agar menjadi kenyataan.

- a. Buatlah program [chat.c](./chat.c) yang dapat menulis pesan dan membaca pesan pada sebuah file. dimana pesan yang akan ditulis harus dijalankan menggunakan multithread, **sehingga 1 thread mewakili 1 kata**. contoh:

  - untuk chat : ./chat write halo saya dhafin.txt

    thread 1 mengolah kata halo

    thread 2 mengolah kata saya

  - untuk read: ./chat read dhafin.txt

  ![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/8c27b46a-77c4-4c86-9915-20f808add8e8)

- b. Buatlah Docker Image yang didalamnya berisi `chat.c` yang telah dibuat sebelumnya dan juga sudah tercompile dengan gcc (alias compile program c nya harus pada Dockerfile) dan beri nama **dockermessage**.

  ![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/5d0586d4-77c5-4792-a641-dd852449a607)

- c. Lalu buatlah [pengaturan.c](./pengaturan.c) yang dapat membuat container baru yang unik dan menghapus container tersebut dari host, dengan docker image yang telah kalian buat sebelumnya menggunakan Dockerfile. contoh:

  - untuk membuat container dengan nama dhafin: `./pengaturan create dhafin`

  - untuk menghapus container dhafin: `./pengaturan delete dhafin`

  - untuk melihat semua container yang tersedia `./pengaturan list`

  - untuk login atau masuk ke dalam container `./pengaturan login dhafin`

- d. Modifikasi `chat.c` dan build ulang menjadi docker image `dockermessage:v2` agar dapat mengirim chat ke container lain dan tambahkan argument pada pengaturan.c agar dapat connect dan uncoonnect sebuah container. (agar container dapat chat dengan container tujuan, container yang ingin chat harus connect terlebih dahulu ke container tujuan. Dan ketika sebuah container terbuat, secara otomatis dia juga memiliki kotak pesan yang nantinya dapat dia read dan dikirimi pesan oleh container lain). contoh:

  - untuk connect container dhafin ke container wahid: `./pengaturan connect dhafin wahid`

  - untuk unconnect container dhafin ke container wahid: `./pengaturan unconnect dhafin wahid`

  - ketika user sedang dalam container dhafin dan ingin mengirim pesan ke container wahid: `./chat write halo teman wahid`

    Note: container tujuan selalu menjadi argument terakhir

  - ketika user sedang dalam container wahid dan ingin mengirim pesan ke dhafin: `./chat write halo teman dhafin`

    Note: container tujuan selalu menjadi argument terakhir

  - untuk melihat isi kotak pesan di container saat ini: `./chat read`

  _connect dulu agar dapat kirim pesan, lalu ketika unconnect tidak bisa kirim pesan_

  _nantinya pesan akan terlihat dalam kotak pesan dalam format seperti ini:_

  `[isi pesan] from [nama pengirim], nama pengirim harus sama dengan nama container pengirim`

#### Penjelasan:
- a. Membuat program chat.c yang memiliki fungsi untuk memasukkan kalimat ke dalam suatu file dengan tiap kata dari kalimatnya ditulis menggunakan threading
Full Code:
```
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
```
Penjelasan:
```
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
```
Pertama-tama program mengimpor seluruh library yang akan dipakai, mutex untuk threading, condition untuk threading, integer counter, dan sebuah struct bernama ThreadArg yang dimana nantinya akan berperan sebagai isi dari thread dalam proses penulisannya (untuk menyimpan lokasi file, kata, dan index dari kata).

```
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
```
Blok kode tersebut merupakan fungsi untuk menuliskan kata ke dalam suatu file. Mulanya fungsi akan membuat thread dan mengunci mutex, lalu apabila index dari thread sekarang tidak sesuai dengan counter maka thread kali ini harus menunggu. Apabila sudah sesuai, maka tulis ke dalam path file kata yang pada thread tersebut. Lalu bersihkan variabel file pada thread. Counter diincrement supaya bergerak ke kata berikutnya. Terakhir mutex di unlock supaya bisa digunakan oleh thread lainnya. 

```
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
    
```
Merupakan fungsi untuk menulis pesan yang mau dikirimkan. Sebelumnya fungsi akan mengecek apakah pengirim dan penerima nya sama, apabila sama maka keluarkan "Pengirim dan penerima tidak bisa sama" lalu exit. Apabila benar maka cek apakah kedua penerima dan pengirim terkonek dengan cara mengecek di folder /app/data/connection apakah ada file bernama penerimapengirim.txt (atau sebaliknya). Bila ada maka lanjut tulis pesan, apabila tidak maka keluarkan "Kedua kontainer tidak terkoneksi" lalu exit. Lalu fungsi akan masuk ke proses pembuatan thread dengan memasukkan setiap kata dari pesan ke dalam struct ThreadArg. Kemudian fungsi akan menjalankan thread-thread dan memanggil fungsi write_word untuk menulis tiap kata. Sesuai permintaan soal, pesan yang dikirimkan berformat "pesan from pengirim" maka tambahkan "from" dan nama pengirim menjadi dua thread baru, lalu jalankan dengan memanggil fungsi write_word. Setelah semua thread dijalankan, join semua thread dan tutup seluruh file, hapus thread condition, hapus mutex, dan bebaskan seluruh thread dan argument yang diberikan. Diakhir keluarkan "Pesan telah berhasil terkirim (enter)".


```
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
```
Kode tersebut berfungsi untuk membaca isi pesan dari suatu file. Pertama fungsi akan mengambil inputan berupa nama file (char filename[256]) ketika fungsi dipanggil. Lalu fungsi akan membuka file tersebut (membuka read) apabila bisa maka fungsi lanjut, bila tidak maka pesan error akan keluar dan fungsi berhenti. Bila bisa dibuka, buat suatu string untuk mengambil setiap baris dari file yakni buffer. Selama terdapat baris di file, maka tampilkan baris tersebut. Setelah selesai, file ditutup.

```
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
```
Untuk fungsi utama, diperlukan sebuah integer dan sebuah array of strings untuk argumen inputan ketika program dijalankan. Integer argc berperan sebagai penampung jumlah argumen, dan array of strings argv berperan untuk menampung argumen-argumen yang dimasukkan. Sesuai permintaan soal, apabila ingin mengirim pesan maka jalankan program dengan format "./chat write kalimat yang ingin dimasukkan filetujuan(bisa juga user tujuan)", apabila ingin membaca pesan maka jalankan program dengan format "./chat read". Maka diperlukan pengondisian untuk mengecek apakah user ingin mengirim pesan atau membaca pesan sekaligus mencegah hal yang tidak diinginkan terjadi bila argumennya tidak sesuai. 

```
  if ((strcmp(argv[1], "write") == 0)) {
        if(argc>3){
            write_message(&argv[2], argc - 3, argv[argc - 1]);
        }else{
            printf("Argument kurang!\n");
            return EXIT_FAILURE;
        }
        
    }
```
Blok kode tersebut untuk mengecek apabila argumen yang dimasukkan user adalah "write". Argumen "write" pasti diletakkan di urutan kedua berarti pada argv terletak pada index ke 1. Lalu pengondisian lagi apabila jumlah argumen lebih dari tiga, jalankan program untuk mengirim pesan dengan pemanggilan fungsi write_message. Inputan yang dimasukkan ke write_message adalah seluruh kata dari index ke 2 di argv dan seterusnya, lalu jumlah dari kata pada kalimat (argc dikurangi 3, 3 disini adalah ./chat write dan file tujuan), dan terakhir adalah file tujuan yakni argumen pada urutan terakhir (argv[argc-1]). Apabila argumen yang dimasukkan kurang dari tiga maka akan dikeluarkan pesan "Argument kurang" dan program keluar secara failure.

```
else if(strcmp(argv[1], "read") == 0){
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
    }
```
Apabila tidak masuk ke dalam pengondisian write, program akan mengecek lagi apakah user memasukkan argumen read. Apabila ternyata user menginputkan read maka tuliskan "Pesan yang terkirim:(enter)" sesuai dengan permintaan soal. Lalu untuk membaca seluruh pesan, program perlu tahu docker container atau user siapa sekarang. Hal tersebut dapat dilakukan dengan mengambil enviroment dari container dengan "getenv("NAME")" dan dijadikan nilai dari string container_name. Lalu inisialisasikan string directory_path  untuk menyimpan lokasi dari file container. Lalu buat lokasi file container dengan snprintf dengan menggabungkan "/app/data/container_name". Setelah dibuat directory_path, buka directory_path. Apabila tidak bhisa dibuga maka program akan mengeluarkan pesan error dan keluar. Karena disini konsepnya pesan yang dibaca berada didalam semua file dengan format txt di dalam folder container tersebut. Maka dibuatlah sistem untuk membaca isi setiap file menggunakan stat. Program lalu akan memanggil fungsi untuk membaca (read_message) dan memberikan inputan lokasi (file_path) sesuai dengan nama-nama file.txt yang ada di direktori. Setiap file.txt pada direktori tersebut kemudian dibaca hingga seluruh file berformat txt telah terbaca. Diakhir program akan menutup direktori dari folder yang berisikan pesan tersebut.

```
else {
        fprintf(stderr, "Salah input.\n");
        return EXIT_FAILURE;
    }

    return 0;
}
```
Jika user tidak memasukkan "write" atau "read" pada saat menjalankan program, maka program akan mengeluarkan "Salah input. (enter)" lalu keluar dengan status EXIT_FAILURE. Bila program berjalan normal, maka pengondisian ini tidak akan dijalankan dan program dikeluarkan secara normal (return 0). 

- b. Membuat docker image yang dapat menjalankan chat.c dengan nama dockermessage
Full Code Dockerfile:
```
FROM alpine:latest

RUN apk add --no-cache gcc musl-dev

WORKDIR /app

COPY chat.c /app

RUN gcc -o chat chat.c -pthread
```
Pertama gunakan dockerimage dari "alpine" dengan tag "latest" (sesuai permintaan soal dan supaya ringan). Lalu supaya dapat mengompile file C diperlukan "gcc" maka download dan install "gcc". Kemudian buat/inisialisasikan suatu direktori yakni /app pada kontainer untuk dijadikan direktori bekerja (work directory). Kemudian copy chat.c ke dalam work directory. Setelah dicopy, compile chat.c beserta -pthread (supaya bisa threading) dengan output "chat" sehingga apabila ingin dijalankan, command pada container adalah "./chat" (sesuai soal). Pengompilean dapat dilakukan dengan "RUN gcc -o chat chat.c -pthread".


- c. Membuat pengaturan.c yang dapat menjalankan chat.c melalui docker image dockermessage
Full Code pengaturan.c:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_container(const char *container_name) {
    char command[256];
    snprintf(command, sizeof(command), "docker run -d --name %s -e NAME=%s -v our_volume:/app/data -t dockermessage:v2 tail -f /dev/null", container_name, container_name);
    system(command);
    snprintf(command, sizeof(command), "docker exec %s mkdir /app/data/%s", container_name, container_name);
    system(command);

    char filename[256];
    printf("Container %s created.\n", container_name);
}

void delete_container(const char *container_name) {
    char command[256];

     snprintf(command, sizeof(command), "docker exec %s rm -rf /app/data/%s", container_name, container_name);
    system(command);

    snprintf(command, sizeof(command), "docker rm -f %s", container_name);
    system(command);
   
    
    printf("Container %s deleted.\n", container_name);
}

void list_containers() {
    system("docker ps -a");
}

void login_container(const char *container_name) {
    char command[256];
    snprintf(command, sizeof(command), "docker exec -it %s /bin/sh", container_name);
    system(command);
}

void connect_containers(const char *container1, const char *container2) {
    char command[256];  


    snprintf(command, sizeof(command), "docker exec %s mkdir -p /app/data/connection", container1);
    system(command);
  
    snprintf(command, sizeof(command), "docker exec %s mkdir -p /app/data/connection", container2);
    system(command);
    
    snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/connection/%s%s.txt", container1, container1, container2);
    system(command);

      snprintf(command, sizeof(command), "docker exec  %s touch /app/data/connection/%s%s.txt", container2, container2, container1);
    system(command);

     snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/%s/%s.txt", container1, container1, container2);
    system(command);

    snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/%s/%s.txt", container2, container2, container1);
    system(command);
  
}

void disconnect_containers(const char *container1, const char *container2) {
    char command[256];

    snprintf(command, sizeof(command), "docker exec %s rm -f /app/data/connection/%s%s.txt", container1, container1, container2);
    system(command);

    snprintf(command, sizeof(command), "docker exec %s rm -f /app/data/connection/%s%s.txt", container2, container2, container1);
    system(command);


}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [container_name]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "create") == 0 && argc == 3) {
        create_container(argv[2]);
    } else if (strcmp(argv[1], "delete") == 0 && argc == 3) {
        delete_container(argv[2]);
    } else if (strcmp(argv[1], "list") == 0) {
        list_containers();
    } else if (strcmp(argv[1], "login") == 0 && argc == 3) {
        login_container(argv[2]);
    }else if (strcmp(argv[1], "connect") == 0 && argc == 4) {
        connect_containers(argv[2], argv[3]);
    }else if (strcmp(argv[1], "unconnect") == 0 && argc == 4) {
         disconnect_containers(argv[2], argv[3]);
    }else {
        fprintf(stderr, "Invalid command or arguments\n");
        return 1;
    }

    return 0;
}
```
Penjelasan:
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
```
Blok kode untuk mengimpor seluruh library yang diperlukan dalam program

```
void create_container(const char *container_name) {
    char command[256];
    snprintf(command, sizeof(command), "docker run -d --name %s -e NAME=%s -v our_volume:/app/data -t dockermessage:v2 tail -f /dev/null", container_name, container_name);
    system(command);
    snprintf(command, sizeof(command), "docker exec %s mkdir /app/data/%s", container_name, container_name);
    system(command);
    printf("Container %s created.\n", container_name);
}
```
Merupakan fungsi untuk membuat container dengan menginisialisasi sebuah string sebagai inputan untuk menampung nama container yang akan dibuat. Lalu dengan membuat suatu string command dan disusun melalui snprintf (suatu code/perintah/command untuk membuat rangkaian string dan dimasukkan ke dalam suatu variable string). String yang dibuat menggunakan snprintf adalah serangkaian perintah docker untuk membuat container dengan nama container tersebut. Perintah dalam string tersebut adalah membuat suatu container dengan nama container sesuai inputan, lalu memberikan env juga sesuai nama container inputan dan sekaligus membuat volume "our_volume" pada directory /app/data. Container dibuat dengan image bernama dockermessage dengan tag v2 dan container dijalankan terus menerus (menggunakan tail -f /dev/null). Lalu perintah dalam string tersebut (command) dijalankan dengan menggunakan system. Selain membuat container fungsi ini juga akan membuat folder dengan nama container tersebut didalam /app/data yang dimana nantinya akan berperan sebagai tempat penampungan pesan yang masuk ke container tersebut. Pembuatan perintah kedua untuk membuat folder, juga menggunakan snprintf dan system. Diakhir fungsi, program akan mengeluarkan kalimat "Container (nama container) created (enter)" untuk menunjukkan bahwa container tersebut telah dibuat. 

```
void delete_container(const char *container_name) {
    char command[256];

     snprintf(command, sizeof(command), "docker exec %s rm -rf /app/data/%s", container_name, container_name);
    system(command);

    snprintf(command, sizeof(command), "docker rm -f %s", container_name);
    system(command);
   
    
    printf("Container %s deleted.\n", container_name);
}
```
Fungsi tersebut berguna untuk menghapus container beserta messageboxnya. Inputan fungsi dimasukkan ke dalam string container_name. Pertama fungsi akan menginisialisasi string command. Lalu seperti sebelumnya, gunakan snprintf untuk membuat serangkaian string perintah untuk menghapus folder messagebox pada container yang akan dihapus (/app/data/nama container). Perintah tersebut dapat menggunakan "docker exec (nama container) rm -rf /app/data/(nama container)" dan dimasukkan ke dalam string command. Lalu string command tersebut di jalankan dengan system. Untuk menghapus containernya dapat menggunakan "docker rm -f (nama container)" dan jalankan command menggunakan system. Jadi ketika container dihapus, messageboxnya akan dihapus terlebih dahulu baru docker container. 

```
void list_containers() {
    system("docker ps -a");
}
```
Blok kode tersebut merupakan fungsi untuk melihat seluruh container yang tersedia. Ketika fungsi dipanggil maka lakukan "docker ps -a" menggunakan system.

```
void login_container(const char *container_name) {
    char command[256];
    snprintf(command, sizeof(command), "docker exec -it %s /bin/sh", container_name);
    system(command);
}

```
Kode tersebut adalah fungsi untuk login atau masuk ke sebuah container. Fungsi akan mengambil inputan berupa nama container dan dimasukkan ke string container_name. Fungsi juga akan menginisialisasikan string command untuk serangkaian perintah yang akan dijalankan. Seperti fungsi sebelumnya, menggunakan snprintf dibuatlah suatu rangkaian perintah untuk login ke dalam suatu container dengan nama container_name. Perintah dapat dilakukan dengan "docker exec -it (nama container) /bin/sh" lalu masukkan perintah tersebut ke dalam command dan jalankan command dengan system.

```
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [container_name]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "create") == 0 && argc == 3) {
        create_container(argv[2]);
    } else if (strcmp(argv[1], "delete") == 0 && argc == 3) {
        delete_container(argv[2]);
    } else if (strcmp(argv[1], "list") == 0) {
        list_containers();
    } else if (strcmp(argv[1], "login") == 0 && argc == 3) {
        login_container(argv[2]);
    ...
    }else {
        fprintf(stderr, "Invalid command or arguments\n");
        return 1;
    }

    return 0;
}
```
Blok tersebut merupakan blok kode terakhir yang berperan sebagai fungsi utama. Fungsi utama harus mengambil argumen maka dibuatlah int argc untuk menampung jumlah argumen yang akan dimasukkan user dan array of strings argv untuk menampung argumen yang dimasukkan user. Mulanya fungsi akan mengecek apakah jumlah argumen kurang dari dua (invalid), bila iya maka keluarkan "Usage: (./pengaturan) <command> [container_name] (enter)" lalu keluar dari program. Apabila valid maka lanjut ke pengecekan berikutnya. Perlu diketahui bahwa format inputan user adalah sebagai berikut:
"./pengaturan (perintah) (nama container)" -> dengan nama container bisa lebih dari satu sesuai perintah. 

Maka dicek pada array of strings argv pada index ke 1 apakah sesuai dengan suatu perintah dan dengan jumlah argumen sesuai atau tidak dengan kebutuhan perintah. Apabila iya maka panggil fungsi sesuai argumen. Misalkan user memasukkan "./pengaturan create container1" maka pengecekan sesuai dan program akan memanggil fungsi create_containers dan memasukkan "container1" sebagai inputan dari fungsi. Jika ternyata inputan user tidak sesuah dengan pengecekan yang ada, maka program akan mengeluarkan "Invalid command or arguments (enter)" sebagai keluaran error dan keluar dari program (program berhenti). Bila ternyata program berjalan normal maka tidak akan masuk ke pengondisian tersebut (apabila inputan user tidak sesuai) dan keluar program dengan normal (return 0).

- d. Mengupdate chat.c dan pengaturan.c supaya dapat saling memberikan pesan dan tambahkan sistem connect dan disconnect (apabila terkoneksi maka user satu bisa mengirim ke user lainnya, sebaliknya apabila tidak terkoneksi maka dua user tersebut tidak bisa saling berkomunikasi).
(penjelasan chat.c sudah ada di penjelasan subtask A, karena pada subtask A, chat.c sudah merupakan versi terakhir yang dapat mengirimkan pesan dari container atau user lain dan membaca semua pesan yang telah dikirimkan)

Tambahan pada pengaturan.c:
```
void connect_containers(const char *container1, const char *container2) {
    char command[256];  


    snprintf(command, sizeof(command), "docker exec %s mkdir -p /app/data/connection", container1);
    system(command);
  
    snprintf(command, sizeof(command), "docker exec %s mkdir -p /app/data/connection", container2);
    system(command);
    
    snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/connection/%s%s.txt", container1, container1, container2);
    system(command);

      snprintf(command, sizeof(command), "docker exec  %s touch /app/data/connection/%s%s.txt", container2, container2, container1);
    system(command);

     snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/%s/%s.txt", container1, container1, container2);
    system(command);

    snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/%s/%s.txt", container2, container2, container1);
    system(command);
  
}

void disconnect_containers(const char *container1, const char *container2) {
    char command[256];

    snprintf(command, sizeof(command), "docker exec %s rm -f /app/data/connection/%s%s.txt", container1, container1, container2);
    system(command);

    snprintf(command, sizeof(command), "docker exec %s rm -f /app/data/connection/%s%s.txt", container2, container2, container1);
    system(command);


}

```

Penjelasan:
Penyambungan koneksi container:
```
void connect_containers(const char *container1, const char *container2) {
    char command[256];  


    snprintf(command, sizeof(command), "docker exec %s mkdir -p /app/data/connection", container1);
    system(command);
  
    snprintf(command, sizeof(command), "docker exec %s mkdir -p /app/data/connection", container2);
    system(command);
    
    snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/connection/%s%s.txt", container1, container1, container2);
    system(command);

      snprintf(command, sizeof(command), "docker exec  %s touch /app/data/connection/%s%s.txt", container2, container2, container1);
    system(command);

     snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/%s/%s.txt", container1, container1, container2);
    system(command);

    snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/%s/%s.txt", container2, container2, container1);
    system(command);
  
}
```
Serangkaian blok kode tersebut adalah fungsi untuk menyambungkan container (connect) dengan inputan dua string berupa nama container pertama dan nama container kedua. Pertama fungsi seperti biasa akan menginisialisasi string command untuk menyimpan rangkaian perintah. Untuk menyambung container, dibuatlah suatu folder dengan nama connection di dalam container pertama dan container kedua dengan path directory /app/data/connection (di dalam volume). Perintah tersebut dibuat menggunakan snprintf dan dimasukkan ke string command, lalu dijalankan menggunakan system. Setelah membuat folder connection, buat pula file dengan format txt dengan nama containerpertamacontainerkedua.txt dan containerkeduacontainerpertama.txt di dalam kedua container tersebut (karena diisi dalam volume maka sebetulnya keduanya dibuat dalam folder yang sama dan dapat diakses oleh kedua container) dan jalankan tiap perintahnya dengan system. Kemudian diperlukan pula file txt sebagai pesan masuk ke suatu container dengan nama file tersebut merupakan nama pengirim pesan. Maka dibuatlah suatu file txt dalam folder container penerima dengan nama containerpengirim.txt (buat di dua container) lalu jalankan dengan system. Untuk pembuatan folder dapat dilakukan dengan perintah "docker exec (nama container) mkdir -p /app/data/connection". Pembuatan file "docker exec (nama container) touch /app/data/(nama container)/container1container2.txt (atau sebaliknya)". Kemudian untuk pembuatan file messagebox (pengirim.txt) dapat dilakukan dengan menyusun perintah "docker exec (nama container penerima) touch /app/data/(nama container penerima)/(containerpengirim).txt.

Pemutusan koneksi container:
```
void disconnect_containers(const char *container1, const char *container2) {
    char command[256];

    snprintf(command, sizeof(command), "docker exec %s rm -f /app/data/connection/%s%s.txt", container1, container1, container2);
    system(command);

    snprintf(command, sizeof(command), "docker exec %s rm -f /app/data/connection/%s%s.txt", container2, container2, container1);
    system(command);


}
```
Fungsi tersebut merupakan fungsi untuk memutus sambungan antar container (disconnect/unconnect). Fungsi membutuhkan dua string yakni string nama container pertama dan string nama container kedua (nama container 1 dimasukkan ke container1, dan nama container 2 dimasukkan ke container2). Untuk memutus sambungan container, perlu hapus file dengan nama container1container2.txt dan container2container1.txt di dalam folder connection. 
Pertama dibuatlah string command untuk menampung serangkaian perintah. Perintah yang disusun adalah "docker exec (nama container penerima) rm -f /app/data/connection/(nama container penerima)(nama container pengirim).txt. Pembuatan perintah menggunakan snprintf dan dimasukkan ke dalam string command. Lalu jalankan command dengan system. Lakukan perintah dua kali karena dalam folder connection terdapat dua file yang menggambarkan hubungan.

Untuk pemanggilan fungsi penyambungan dan pemutusan koneksi dalam fungsi utama dapat dilakukan seperti berikut:
```
else if (strcmp(argv[1], "connect") == 0 && argc == 4) {
        connect_containers(argv[2], argv[3]);
    }else if (strcmp(argv[1], "unconnect") == 0 && argc == 4) {
         disconnect_containers(argv[2], argv[3]);
```
Jadi seperti subtask sebelumnya, fungsi utama akan mengecek argumen yang dimasukkan user. Apabila user memasukkan connect dan argumennya ada 4 (valid) maka panggil fungsi connect_containers dengan memasukkan argumen index ke 2 (container1) dan argumen index ke 3 (container 2). Begitu juga dengan argumen unconnect yang akan memanggil fungsi disconnect_containers. 

Note:
Format inputan user adalah "./pengaturan connect container1 container2" (connect dapat diganti dengan unconnect)

Bukti Screenshot:
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/1d7965de-7b7c-456c-9953-cd64fbfaf9ab)
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/b39a7aad-3c28-4c41-937c-6630e30f2b29)
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/48cc2eb5-ba7d-4c9e-ae0a-e8a26d01a648)
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/92079448-1a0e-4ef9-bb7b-197a2e9d087a)
![image](https://github.com/sisop-its-s24/praktikum-modul-3-b24/assets/162027180/f754930e-f6d7-40bf-b8ff-65cfe7c7b521)




- [Task 4 - WOTA Competition](/task-4/) by Fernando
  
#### Soal
**WOTA Competition**

Pada suatu hari, ada seorang anak bernama Shani Nadira Natio, atau kerap dipanggil Shaniz696969, yang mengikuti sebuah lomba yang diadakan oleh WOTA Indonesia.

Dia akan membuat aplikasi yang sangat bermanfaat untuk para disabilitas di luar sana. Namun, suatu hari panitia memberikan sebuah ketentuan, bahwa setiap aplikasi yang dibuat haruslah dapat dijalankan di local setiap juri sehingga juri juga memiliki keleluasaan dalam melakukan analisis (baik dinamis maupun statis) terhadap aplikasi yang dibuat.

Shaniz696969 pun berpikir sejenak karena hal ini sangatlah tricky, jika ia hanya membuat sebuah dokumentasi atau mungkin runner, mungkin saja environment atau mungkin machine yang dimiliki oleh juri tidak sama dengan apa yang dimiliki oleh tim Shaniz696969. Namun, untung diuntung ternyata Shaniz696969 memiliki seorang teman yang sangat jago dalam sistem operasi, yaitu kamu! 🫵🫵🫵🫵🫵🫵🫵🫵🫵🫵🫵🫵🫵🫵

Singkat cerita, kamu memperkenalkan containerization dalam arsitektur Docker seperti ini.

Karena itu, dia memintamu untuk ikut dalam timnya. Bantu _arek’e_, ya!

- a. Akses URL berikut untuk mendapatkan resource yang dibutuhkan, yaitu sebuah aplikasi yang sedang dibuat oleh Shaniz696969!

  https://drive.google.com/file/d/1iMCSmUzlQ1OqIU72o9IGbPMeghFbj_zK/view?usp=sharing

  _It’s kinda legit, right?_ Aneh juga ya dia tidak pakai GitHub atau semacamnya tapi malah pakai Google Drive (kocak). Tapi _gapapa_ lah ya. Nah, tugas kamu adalah untuk membuat image dari aplikasi tersebut!

- b. Selanjutnya, karena Shaniz696969 mengatakan bahwa aplikasi ini menggunakan relational database berupa MySQL, mau tidak mau kamu harus membuat container yang menjalankan image dari MySQL! Pastikan container berjalan sebagai daemon.

  **Psst, ingat, kamu bisa mendapatkan image MySQL dari Docker Hub.**

- c. Kamu ingat bahwa sudah membuat image dari aplikasi Shaniz696969? Nah, sekarang, jalankan image tersebut sebagai container. Pastikan container berjalan sebagai daemon.

- d. Apa lagi yang kurang? Yup, kamu harus melakukan koneksi antara aplikasi backend tersebut dengan database yang telah dijalankan. Tentunya, dalam container yang telah kita jalankan di poin-poin sebelumnya, kan? Nah, lakukan koneksi antara dua container tersebut dalam satu network bernama **Shani-net**!

  **Hint: [belum tersedia] [ada, tapi nanti]**

- e. Terakhir, kamu harus lakukan setup pada aplikasi backend tersebut dengan menggunakan dokumentasi Laravel sebagai panduan kalian! Aplikasi harus bisa diakses melalui http://localhost:9090. Agar lebih legit, kalian bisa melakukan hit pada endpoint `/api/province`.

Pada akhirnya, juri terkesima dengan inisiatif Shaniz696969 untuk melakukan containerization pada ‘sebagian’ sistem miliknya sehingga dapat dijalankan sebagai container pada machine setiap juri dengan baik! Shaniz696969 pun menjadi juara! 👑👑👑



#### Penjelasan
Full Code Dockerfile:
```
FROM php:8.2-fpm

RUN apt-get update
RUN apt-get install -y wget
RUN apt-get install -y zip
RUN apt-get install -y unzip
RUN apt-get install -y libpng-dev
RUN apt-get install -y libonig-dev
RUN apt-get install -y libxml2-dev
RUN apt-get clean && rm -rf /var/lib/apt/lists/*
RUN docker-php-ext-install pdo pdo_mysql mbstring exif pcntl bcmath gd

WORKDIR /var/www/html
COPY . .
RUN wget --no-check-certificate "https://drive.usercontent.google.com/u/0/uc?id=1iMCSmUzlQ1OqIU72o9IGbPMeghFbj_zK&export=download" -O laraveltemplate
RUN unzip laraveltemplate
RUN mv laravel-onion-template/* /var/www/html
RUN rm laraveltemplate
RUN rm -rf laravel-onion-template

RUN echo "APP_NAME=Laravel" > .env
RUN echo "APP_ENV=local" >> .env
RUN echo "APP_KEY=base64:IkpV7upEVtsaFPo7yUeE7M2cknBww9N8vglFG5J2NdU=" >> .env
RUN echo "APP_DEBUG=true" >> .env
RUN echo "APP_URL=http://localhost" >> .env
RUN echo "LOG_CHANNEL=stack" >> .env
RUN echo "LOG_DEPRECATIONS_CHANNEL=null" >> .env
RUN echo "LOG_LEVEL=debug" >> .env
RUN echo "DB_CONNECTION=mysql" >> .env
RUN echo "DB_HOST=mysql" >> .env
RUN echo "DB_PORT=3306" >> .env
RUN echo "DB_DATABASE=keyisa" >> .env
RUN echo "DB_USERNAME=root" >> .env
RUN echo "DB_PASSWORD=secret" >> .env
RUN echo "BROADCAST_DRIVER=log" >> .env
RUN echo "CACHE_DRIVER=file" >> .env
RUN echo "FILESYSTEM_DISK=local" >> .env
RUN echo "QUEUE_CONNECTION=sync" >> .env
RUN echo "SESSION_DRIVER=file" >> .env
RUN echo "SESSION_LIFETIME=120" >> .env
RUN echo "MEMCACHED_HOST=127.0.0.1" >> .env
RUN echo "REDIS_HOST=127.0.0.1" >> .env
RUN echo "REDIS_PASSWORD=null" >> .env
RUN echo "REDIS_PORT=6379" >> .env
RUN echo "MAIL_MAILER=smtp" >> .env
RUN echo "MAIL_HOST=smtp.mailtrap.io" >> .env
RUN echo "MAIL_PORT=2525" >> .env
RUN echo "MAIL_USERNAME=7d009e488ee2ca" >> .env
RUN echo "MAIL_PASSWORD=bb9becb07b369a" >> .env
RUN echo "MAIL_ENCRYPTION=tls" >> .env
RUN echo "AWS_ACCESS_KEY_ID=" >> .env
RUN echo "AWS_SECRET_ACCESS_KEY=" >> .env
RUN echo "AWS_DEFAULT_REGION=us-east-1" >> .env
RUN echo "AWS_BUCKET=" >> .env
RUN echo "AWS_USE_PATH_STYLE_ENDPOINT=false" >> .env
RUN echo "PUSHER_APP_ID=" >> .env
RUN echo "PUSHER_APP_KEY=" >> .env
RUN echo "PUSHER_APP_SECRET=" >> .env
RUN echo "PUSHER_APP_CLUSTER=mt1" >> .env
RUN echo "MIX_PUSHER_APP_KEY="\${PUSHER_APP_KEY}"" >> .env
RUN echo "MIX_PUSHER_APP_CLUSTER="\${PUSHER_APP_CLUSTER}"" >> .env

COPY --from=composer:latest /usr/bin/composer /usr/bin/composer
RUN composer install
COPY --chown=www-data:www-data . /var/www/html
EXPOSE 80
```

Pertama, update dan install dependency yang dibutuhkan untuk membuat image laravel app
```
FROM php:8.2-fpm

RUN apt-get update
RUN apt-get install -y wget
RUN apt-get install -y zip
RUN apt-get install -y unzip
RUN apt-get install -y libpng-dev
RUN apt-get install -y libonig-dev
RUN apt-get install -y libxml2-dev
RUN apt-get clean && rm -rf /var/lib/apt/lists/*
RUN docker-php-ext-install pdo pdo_mysql mbstring exif pcntl bcmath gd
```

Selanjutnya, download template laravel yang diberikan ke direktori /var/www/html di containernya. 
Step pertama adalah pindahkan working directory ke /var/www/html untuk memudahkan pemindahan
COPY . . untuk mengduplikat file Dockerfile dan docker-compose ke /var/www/htmlnya.
RUN wget adalah untuk mendownload template tersebut menjadi file laraveltemplate, setelah selesai didownload, akan diunzip di folder yang sama, lalu pindahkan semua isinya ke /var/www/html dan hapus folder dan zipnya untuk menghilangkan clutter
```
WORKDIR /var/www/html
COPY . .
RUN wget --no-check-certificate "https://drive.usercontent.google.com/u/0/uc?id=1iMCSmUzlQ1OqIU72o9IGbPMeghFbj_zK&export=download" -O laraveltemplate
RUN unzip laraveltemplate
RUN mv laravel-onion-template/* /var/www/html
RUN rm laraveltemplate
RUN rm -rf laravel-onion-template
```

Blok kode ini berfungsi untuk mengubah isi .env yang telah diunzip laraveltemplatenya.
```
RUN echo "APP_NAME=Laravel" > .env
RUN echo "APP_ENV=local" >> .env
RUN echo "APP_KEY=base64:IkpV7upEVtsaFPo7yUeE7M2cknBww9N8vglFG5J2NdU=" >> .env
RUN echo "APP_DEBUG=true" >> .env
RUN echo "APP_URL=http://localhost" >> .env
RUN echo "LOG_CHANNEL=stack" >> .env
RUN echo "LOG_DEPRECATIONS_CHANNEL=null" >> .env
RUN echo "LOG_LEVEL=debug" >> .env
RUN echo "DB_CONNECTION=mysql" >> .env
RUN echo "DB_HOST=mysql" >> .env
RUN echo "DB_PORT=3306" >> .env
RUN echo "DB_DATABASE=keyisa" >> .env
RUN echo "DB_USERNAME=root" >> .env
RUN echo "DB_PASSWORD=secret" >> .env
RUN echo "BROADCAST_DRIVER=log" >> .env
RUN echo "CACHE_DRIVER=file" >> .env
RUN echo "FILESYSTEM_DISK=local" >> .env
RUN echo "QUEUE_CONNECTION=sync" >> .env
RUN echo "SESSION_DRIVER=file" >> .env
RUN echo "SESSION_LIFETIME=120" >> .env
RUN echo "MEMCACHED_HOST=127.0.0.1" >> .env
RUN echo "REDIS_HOST=127.0.0.1" >> .env
RUN echo "REDIS_PASSWORD=null" >> .env
RUN echo "REDIS_PORT=6379" >> .env
RUN echo "MAIL_MAILER=smtp" >> .env
RUN echo "MAIL_HOST=smtp.mailtrap.io" >> .env
RUN echo "MAIL_PORT=2525" >> .env
RUN echo "MAIL_USERNAME=7d009e488ee2ca" >> .env
RUN echo "MAIL_PASSWORD=bb9becb07b369a" >> .env
RUN echo "MAIL_ENCRYPTION=tls" >> .env
RUN echo "AWS_ACCESS_KEY_ID=" >> .env
RUN echo "AWS_SECRET_ACCESS_KEY=" >> .env
RUN echo "AWS_DEFAULT_REGION=us-east-1" >> .env
RUN echo "AWS_BUCKET=" >> .env
RUN echo "AWS_USE_PATH_STYLE_ENDPOINT=false" >> .env
RUN echo "PUSHER_APP_ID=" >> .env
RUN echo "PUSHER_APP_KEY=" >> .env
RUN echo "PUSHER_APP_SECRET=" >> .env
RUN echo "PUSHER_APP_CLUSTER=mt1" >> .env
RUN echo "MIX_PUSHER_APP_KEY="\${PUSHER_APP_KEY}"" >> .env
RUN echo "MIX_PUSHER_APP_CLUSTER="\${PUSHER_APP_CLUSTER}"" >> .env
```

Akhirnya, diinstall composer dan expose port 80 agar dapat diup nantinya sebagai website.
```
COPY --from=composer:latest /usr/bin/composer /usr/bin/composer
RUN composer install
COPY --chown=www-data:www-data . /var/www/html
EXPOSE 80
```

Full code docker-compose.yml:
```
version: "3.8"

networks:
  Shani-net:
    driver: bridge

services:
  laravel:
    build:
      context: .
      dockerfile: Dockerfile
    command: bash -c "php artisan migrate --seed && php artisan serve --host=0.0.0.0 --port=80"
    container_name: laravel
    restart: always
    ports:
      - "9090:80"
    networks:
      - Shani-net
    depends_on:
      - mysql

  mysql:
    image: mysql:5.7
    container_name: mysql
    restart: unless-stopped
    tty: true
    ports:
      - "3306:3306"
    environment:
      - MYSQL_DATABASE=keyisa
      - MYSQL_ROOT_PASSWORD=secret
    networks:
      - Shani-net
```

Pertama, dibuat network bernama Shani-net yang menggunakan driver default/bridge
```
networks:
  Shani-net:
    driver: bridge
```

Dibuat service laravel untuk membangun website laravel yang diberikan. Image yang digunakan berasal dari Dockerfile yang telah dibuat sebelumnya. Laravel ini juga akan dibuat setelah mysql telah dibuat. Laravel ini menggunakan network Shani-net agar bisa disambungkan dengan mysql nantinya. Setelah containernya selesai dibuild dan diup, akan dijalankan command "bash -c "php artisan migrate --seed && php artisan serve --host=0.0.0.0 --port=80"", yang berfungsi untuk mengambil data dari database mysql dan menjalankan website tersebut di localhost dengan port 80.
```
services:
  laravel:
    build:
      context: .
      dockerfile: Dockerfile
    command: bash -c "php artisan migrate --seed && php artisan serve --host=0.0.0.0 --port=80"
    container_name: laravel
    restart: always
    ports:
      - "9090:80"
    networks:
      - Shani-net
    depends_on:
      - mysql
```

Akhirnya, dibuat service mysql untuk menyimpan data-data laravel. mysql juga memiliki pseudo terminal dengan tty: true. Database yang digunakan adalah keyisa dengan network yang digunakan adalah Shani-net agar tersambung dengan laravelnya.
```
  mysql:
    image: mysql:5.7
    container_name: mysql
    restart: unless-stopped
    tty: true
    ports:
      - "3306:3306"
    environment:
      - MYSQL_DATABASE=keyisa
      - MYSQL_ROOT_PASSWORD=secret
    networks:
      - Shani-net
```
