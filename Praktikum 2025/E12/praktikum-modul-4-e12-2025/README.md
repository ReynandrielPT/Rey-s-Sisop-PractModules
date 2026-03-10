[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/V7fOtAk7)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241178 | Christian Mikaxelo |
| 5025241218 | Ida Bagus Gde Dimas Sutha Maha |
| 5025241227 | Safa Maulana Efendi |

# Praktikum Modul 4 _(Module 4 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - FUSecure](/task-1/)
### Task 1 - FUSecure

- **Setup Direktori, User, dan File**

1. Buat direktori source dan subfolder
```bash
sudo mkdir -p /home/shared_files/public
sudo mkdir -p /home/shared_files/private_yuadi
sudo mkdir -p /home/shared_files/private_irwandi
```
2. Buat user yuadi dan irwandi
```bash
sudo useradd -m yuadi
sudo useradd -m irwandi
sudo passwd yuadi  
sudo passwd irwandi 

```
1. Buat untuk pengujian
```bash
# Buat file di public
echo "File untuk semua user" | sudo tee /home/shared_files/public/materi_algoritma.txt

# File milik Yuadi
echo '#include <stdio.h>\nint main() {\n    printf("Ini jawaban praktikum 1 Yuadi\\n");\n    return 0;\n}' | sudo tee /home/shared_files/private_yuadi/jawaban_praktikum1.c

# File milik Irwandi
echo '#include <stdio.h>\nint main() {\n    printf("Ini tugas sistem operasi Irwandi\\n");\n    return 0;\n}' | sudo tee /home/shared_files/private_irwandi/tugas_sisop.c
```
- **Membuat File Fuse**

1. Setting config kernel
```bash
nano fusecure.c
```
2. Isi fusecure.c
```c
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>

static const char *source_dir = "/home/shared_files";

static void fullpath(char fpath[1024], const char *path) {
    sprintf(fpath, "%s%s", source_dir, path);
}

static int xmp_getattr(const char *path, struct stat *stbuf) {
    char fpath[1024];
    fullpath(fpath, path);
    int res = lstat(fpath, stbuf);
    return (res == -1) ? -errno : 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi) {
    DIR *dp;
    struct dirent *de;
    char fpath[1024];
    fullpath(fpath, path);
    dp = opendir(fpath);
    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0)) break;
    }

    closedir(dp);
    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi) {
    char fpath[1024];
    fullpath(fpath, path);

    if ((fi->flags & O_ACCMODE) != O_RDONLY)
        return -EACCES;

    uid_t uid = fuse_get_context()->uid;

    if (strncmp(path, "/private_yuadi/", 15) == 0 && uid != getpwnam("yuadi")->pw_uid)
        return -EACCES;
    if (strncmp(path, "/private_irwandi/", 17) == 0 && uid != getpwnam("irwandi")->pw_uid)
        return -EACCES;

    int fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno;
    close(fd);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi) {
    char fpath[1024];
    fullpath(fpath, path);
    int fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno;
    int res = pread(fd, buf, size, offset);
    close(fd);
    return (res == -1) ? -errno : res;
}

static int deny_all() {
    return -EACCES;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .open = xmp_open,
    .read = xmp_read,
    .write = (void *)deny_all,
    .mkdir = (void *)deny_all,
    .rmdir = (void *)deny_all,
    .unlink = (void *)deny_all,
    .rename = (void *)deny_all,
    .chmod = (void *)deny_all,
    .chown = (void *)deny_all,
    .truncate = (void *)deny_all,
    .create = (void *)deny_all,
};

int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
```
**Penjelasan**
- `source_dir` dan `fullpath()`
Program diawali dengan mendefinisikan direktori sumber (/home/shared_files) yang akan dipasang sebagai sistem file virtual. Fungsi fullpath() digunakan untuk menggabungkan path virtual (yang dilihat user di FUSE) dengan path asli di disk.
- `getattr()`
Fungsi ini digunakan untuk mengambil atribut file, seperti ukuran, tipe file, waktu modifikasi, dan sebagainya. Fungsi ini wajib ada dalam sistem FUSE agar sistem bisa mengenali file dan direktori.
- `readdir()`
Digunakan untuk membaca isi direktori. Ketika user menjalankan perintah seperti ls, fungsi ini akan dipanggil untuk menampilkan daftar file atau folder di dalam direktori tersebut.
- `open()`
Di sinilah sistem mengecek apakah pengguna boleh membuka file tertentu atau tidak. Pertama, sistem memastikan bahwa file dibuka dalam mode read-only. Setelah itu, UID dari user yang sedang mengakses file dicek. Jika user mencoba mengakses folder pribadi yang bukan miliknya, maka akses akan ditolak dengan error "Permission Denied".
- `read()`
Fungsi ini memungkinkan pengguna membaca isi file. Ia membuka file asli di disk, membaca isinya, lalu mengembalikannya ke sistem FUSE.
- `deny_all()`
Seluruh operasi tulis, seperti menulis file (write), membuat direktori (mkdir), menghapus file (unlink), mengganti nama file (rename), dan lain-lain, secara otomatis akan ditolak. Ini dilakukan dengan membuat satu fungsi deny_all() yang selalu mengembalikan EACCES (akses ditolak), lalu fungsi tersebut dipasang ke semua operasi tulis.
- `main()`
Terakhir, fungsi main() menjalankan sistem file dengan memanggil fuse_main() dan menyertakan semua operasi yang sudah didefinisikan sebelumnya.

- **Compile Program & Mount**
  
1. Compile
```bash
  gcc -Wall `pkg-config fuse --cflags` fusecure.c -o fusecure `pkg-config fuse --libs`
```
1. Mount
```bash
  sudo mkdir /mnt/secure_fs
```
1. Run Fuse
```bash
  sudo ./fusecure /mnt/secure_fs -o allow_others
```
- **Hasil Screenshoot**
![Image](https://github.com/user-attachments/assets/33d2f34d-a38d-49bb-9d0e-8e9f799a3f5b)
![Image](https://github.com/user-attachments/assets/e46d59af-a7bc-4b5b-91d1-f20183057f66)
![Image](https://github.com/user-attachments/assets/4bd3e666-a04d-4e5d-8814-22c2f1c08107)

- [Task 2 - LawakFS++](/task-2/)
# LawakFS++ - Sistem Berkas Terkutuk dengan Sensor dan Kebijakan Akses Ketat

**LawakFS++** adalah sistem berkas yang dibuat untuk memenuhi kebutuhan Teja dalam mengelola file dengan ketat, melakukan sensor otomatis terhadap konten "lawak", serta memastikan bahwa akses ke file rahasia hanya diperbolehkan pada waktu yang telah ditentukan.

## A. Akses Berdasarkan Waktu
File dengan nama dasar **`secret`** hanya dapat diakses antara pukul **08:00** hingga **18:00** waktu sistem. Jika diakses di luar rentang waktu tersebut, sistem akan mengembalikan error `ENOENT` (No such file or directory).

### Fitur:
- Pembatasan waktu akses untuk file dengan nama dasar "secret".
- Memastikan file rahasia tidak dapat diakses ketika di luar jam yang diizinkan.

### Contoh Kode (Bagian `getattr` untuk akses berbasis waktu):
```c
static int xmp_getattr(const char *path, struct stat *stbuf) {
    char fpath[4100];
    fullPth(fpath, path);
    
   
    if (strstr(path, SECRET_FILE_BASENAME)) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        if (tm.tm_hour < ACCESS_START_HOUR || tm.tm_hour >= ACCESS_END_HOUR) {
            return -ENOENT;  
        }
    }

    int res = lstat(fpath, stbuf);
    if (res == -1) return -errno; 
    return 0;
}
```

## B. Base64 Encoding untuk File Biner
```c
else if (strstr(path, ".bin")) {
    char encoded[2048];
    
    memcpy(buf, encoded, strlen(encoded));
    res = strlen(encoded);
}

```
## C. Censorship (Penyaringan Konten)
```c
void filter_text(char *buf, size_t size) {
    char *lawak_words[] = {"ducati", "ferrari", "mu", "chelsea", "prx", "onic", "sisop"};
    for (int i = 0; i < sizeof(lawak_words) / sizeof(lawak_words[0]); i++) {
        char *pos;
        while ((pos = strcasestr(buf, lawak_words[i])) != NULL) {
            memset(pos, 'l', strlen(lawak_words[i]));  
        }
    }
}

```

## D Daftar Kata "Lawak"
```conf
FILTER_WORDS=ducati,ferrari,mu,chelsea,prx,onic,sisop
SECRET_FILE_BASENAME=secret
ACCESS_START=08:00
ACCESS_END=18:00

```
## E. Ekstensi File Tersembunyi
```c
while ((de = readdir(dp)) != NULL) {
   
    char *dot = strrchr(de->d_name, '.');
    if (dot) *dot = '\0';  
    struct stat st;
    memset(&st, 0, sizeof(st));
    st.st_ino = de->d_ino;
    st.st_mode = de->d_type << 12;
    if (filler(buf, de->d_name, &st, 0)){
        break;
    }
}

```
## F. Fitur Read-Only
```c
static int gabole() {
    return -EROFS;
}
```

## G. Galeri Log Akses
```c
static void log_access(const char *path, const char *action) {
    FILE *log = fopen("/var/log/lawakfs.log", "a");
    if (log == NULL) return;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(log, "[%04d-%02d-%02d %02d:%02d:%02d] [UID %d] [%s] %s\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec,
            getuid(), action, path);
    fclose(log);
}

```
- [Task 3 - Drama Troll](/task-3/)

# **Drama Troll**

Dalam sebuah perusahaan penuh drama fandom, seorang karyawan bernama DainTontas telah menyinggung komunitas Gensh _ n, Z _ Z, dan Wut \* ering secara bersamaan. Akibatnya, dua rekan kerjanya, SunnyBolt dan Ryeku, merancang sebuah troll dengan gaya khas: membuat filesystem jebakan menggunakan FUSE.

Mereka membutuhkan bantuanmu, ahli Sistem Operasi, untuk menciptakan filesystem kustom yang bisa mengecoh DainTontas, langsung dari terminal yang dia cintai.

## **a. Pembuatan User**

Buat 3 user di sistem sebagai berikut yang merepresentasikan aktor-aktor yang terlibat dalam _trolling_ kali ini, yaitu:

- DainTontas
- SunnyBolt
- Ryeku

Gunakan `useradd` dan `passwd` untuk membuat akun lokal. User ini akan digunakan untuk mengakses filesystem FUSE yang kamu buat.

**Jawaban:**

`sudo useradd DainTontas`

`sudo useradd SunnyBolt`

`sudo useradd Ryeku`

## **b. Jebakan Troll**

Untuk menjebak DainTontas, kamu harus menciptakan sebuah filesystem FUSE yang dipasang di `/mnt/troll`. Di dalamnya, hanya akan ada dua file yang tampak sederhana:

- `very_spicy_info.txt` - umpan utama.
- `upload.txt` - tempat DainTontas akan "secara tidak sadar" memicu jebakan.

**Jawaban:**

`sudo mkdir -p /mnt/troll` membuat direktori untuk mount FUSE

`sudo chown $USER:$USER /mnt/troll` mengubah ownership menjadi user

## **c. Jebakan Troll (Berlanjut)**

Setelah membuat dua file tersebut, SunnyBolt dan Ryeku memintamu untuk membuat jebakan yang telah dirancang oleh mereka. SunnyBolt dan Ryeku yang merupakan kolega lama DainTontas dan tahu akan hal-hal memalukan yang dia pernah lakukan, akan menaruh aib-aib tersebut pada file `very_spicy_info.txt`. Apabila dibuka dengan menggunakan command `cat` oleh user selain DainTontas, akan memberikan output seperti berikut:

```
DainTontas' personal secret!!.txt
```

Untuk mengecoh DainTontas, apabila DainTontas membuka `very_spicy_info.txt`, isinya akan berupa seperti berikut:

```
Very spicy internal developer information: leaked roadmap.docx
```
**Jawaban:**

![Image](https://github.com/user-attachments/assets/16c32717-119c-4c86-9319-bbcdbd25fe4a)

dalam function `xmp_read` setelah mengambil path file dengan loop, statement pada gambar akan dilakukan ketika path nya adalah `very_spicy_info.txt`. setelah itu kita ambil uidnya dan jika nama dari uid nya adalah DainTontas maka content nya akan kita rubah menjadi konten untuk DianTontas `Very spicy internal developer information: leaked roadmap.docx`

**Hasil:**

- User Biasa

![Image](https://github.com/user-attachments/assets/08a88456-192c-4bb3-8d57-a52b4fc53c1d)

- DianTontas

![Image](https://github.com/user-attachments/assets/1e646faa-356c-46a4-ade8-5ed936309449)



## **d. Trap**

Suatu hari, DainTontas mengecek PC dia dan menemukan bahwa ada sebuah file baru, `very_spicy_info.txt`. Dia kemudian membukanya dan menemukan sebuah leak roadmap. Karena dia ingin menarik perhatian, dia akan mengupload file tersebut ke sosial media `Xitter` dimana _followernya_ dapat melihat dokumen yang bocor tersebut. Karena dia pernah melakukan hal ini sebelumnya, dia punya script bernama `upload.txt` yang dimana ketika dia melakukan `echo` berisi `upload` pada file tersebut, dia akan otomatis mengupload sebuah file ke akun `Xitter` miliknya.

Namun ini semua adalah rencana SunnyBolt dan Ryeku. Atas bantuanmu, mereka telah mengubah isi dari `upload.txt`. Apabila DainTontas melakukan upload dengan metode `echo` miliknya, maka dia dinyatakan telah "membocorkan" info leak tersebut. Setelah DainTontas membocorkan file tersebut, semua file berekstensi `.txt` akan selalu memunculkan sebuah ASCII Art dari kalimat

```
Fell for it again reward
```

Setelah seharian penuh mencoba untuk membetulkan PC dia, dia baru sadar bahwa file `very_spicy_info.txt` yang dia sebarkan melalui script uploadnya tersebut teryata berisikan aib-aib dia. Dia pun ditegur oleh SunnyBolt dan Ryeku, dan akhirnya dia pun tobat.

**Jawaban:**

![Image](https://github.com/user-attachments/assets/1125e690-3d85-4608-853f-07f46ab02d8a)

dalam function `xmp_write` ketika path nya sama dengan `upload.txt` dan trigger trapped nya belum aktif beserta buffer yang diberikan adalah "upload" ``(echo "upload")`` maka kita akan `write_status(1)` untuk mentrigger trapnya. 

**Hasil:**

- DainTontas melakukan `echo` 

![Image](https://github.com/user-attachments/assets/16e54afe-fb6a-43cd-a085-a66b51b94b45)

sampai saat ini, setiap DainTontas melakukan `echo`, selalu mendapat error message berupa : no such file or directory, permission denied. dan setelah beberapa kali percobaan untuk mengubah ownership dan permission, tetap saja belum berhasil.

- Ascii Art 

![Image](https://github.com/user-attachments/assets/06f1ccc4-b3cd-4baf-b0a5-26acf61a707a)

ketika status trapped true, maka content akan berubah menjadi ascii art yang ditulis melalui file external troll.txt

![Image](https://github.com/user-attachments/assets/16721cc3-edad-4f58-b5e7-0e4342ca1042)

ini function untuk mengambil isi dari troll.txt


## **NOTES**

- `upload.txt` hanyalah berupa file kosong yang dipakai untuk trigger _behavior change_ dari `cat` milik DainTontas. Deskripsi di soal hanyalah sebagai _storytelling_ belaka.
- Trigger dari Trap akan bersifat `persist` bahkan ketika FUSE telah di-unmount ataupun ketika mesin dimatikan/direstart

**Jawaban:**

agar trigger trap bersifat `persist` bahkan ketika FUSE telah di-unmount ataupun mesin dimatikan/direstart, maka setiap program dieksekusi, trap akan selalu ditulis kedalam suatu file yang menyimpan angka `1` untuk menghasilkan kondisi trapped yang true

![Image](https://github.com/user-attachments/assets/ab16cbd7-3718-4ce8-ac39-a74c923a7bf7)



---


- [Task 4 - LilHabOS](/task-4/)
# LilHabOS - Kernel Sederhana dengan Fitur Piping


- **`bin/`**: Menyimpan file hasil kompilasi
- **`include/`**: Menyimpan file header
- **`src/`**: Menyimpan file kode sumber
- **`bochsrc.txt`**: Konfigurasi Bochs
- **`makefile`**: File konfigurasi make untuk kompilasi dan linking

## Fungsi Kernel
##### kernel.c
```c
#include "std_lib.h"
#include "kernel.h"

void echo(char* input) {
    printString(input);
}

void grep(char* pattern, char* input) {
    if (strcmp(pattern, input) == 0) {
        printString(input);
    }
}

void wc(char* input) {
    int lines = 0, words = 0, chars = 0;
    bool inWord = false;

    while (*input) {
        chars++;

        if (*input == '\n') {
            lines++;
        }

        if (*input == ' ' || *input == '\n' || *input == '\t') {
            if (inWord) {
                words++;
                inWord = false;
            }
        } else {
            inWord = true;
        }

        input++;
    }

    if (inWord) {
        words++;
    }

    printString("Lines: ");
    printString(itoa(lines));
    printString(", Words: ");
    printString(itoa(words));
    printString(", Characters: ");
    printString(itoa(chars));
}

int main() {
    char buf[128];

    clearScreen();
    printString("LilHabOS - E12\n");

    while (true) {
        printString("$> ");
        readString(buf);
        printString("\n");

        if (strlen(buf) > 0) {
            if (strncmp(buf, "echo ", 5) == 0) {
                echo(buf + 5);
            }
            else if (strncmp(buf, "echo ", 5) == 0 && strchr(buf, '|') != NULL) {
                char* arg1 = buf + 5;
                char* arg2 = strchr(buf, '|') + 2;
                grep(arg2, arg1);
            }
            else if (strncmp(buf, "echo ", 5) == 0 && strchr(buf, '|') != NULL && strstr(buf, "wc") != NULL) {
                char* arg1 = buf + 5;
                wc(arg1);
            }
        }
    }
}

```
### 1. **`kernel.asm`** (Kode Assembly)

File assembly ini mendefinisikan dua fungsi penting:

- **`_putInMemory`**: Menulis karakter ke lokasi memori tertentu.
- **`_interrupt`**: Memicu interrupt dengan nilai register yang diberikan.

### 2. **`kernel.c`** (Kode C)

File ini berisi logika inti dari kernel, termasuk:

- **`main`**: Loop utama shell yang menunggu input pengguna, memproses perintah, dan memanggil fungsi berdasarkan input pengguna.
- Fungsi untuk menangani input dan output string, serta mengelola lingkungan shell.

### 3. **`std_lib.h`** (Header untuk Library Standar)

Berisi deklarasi fungsi utilitas seperti:

- **`div`, `mod`**: Implementasi pembagian dan modulo.
- **`memcpy`, `strlen`, `strcmp`, `strcpy`, `clear`**: Fungsi manipulasi string.

### 4. **`std_type.h`** (Tipe Data)

Mendefinisikan tipe data dasar yang digunakan di seluruh kernel:

- **`byte`**: `unsigned char` untuk operasi byte-level.
- **`bool`**: Tipe data boolean (`true` atau `false`).
- **`NULL`**: Konstanta pointer null.

## Fitur yang Diimplementasikan

### a. **`printString`, `readString`, dan `clearScreen`**

Fungsi-fungsi ini diimplementasikan untuk berinteraksi dengan pengguna melalui terminal:

- **`printString`**: Menampilkan string yang diakhiri null menggunakan interrupt `0x10`.
- **`readString`**: Membaca input dari pengguna, termasuk penanganan backspace dasar.
- **`clearScreen`**: Membersihkan layar dan mengatur kursor ke pojok kiri atas.

### b. **Perintah `echo`**

Perintah `echo` diimplementasikan untuk menerima input dari pengguna dan menampilkannya di shell.

Contoh penggunaan:
```bash
$> echo Hello World
Hello World


