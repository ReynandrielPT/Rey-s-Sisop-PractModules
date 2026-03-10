[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/V7fOtAk7)
|    NRP     |      Name      |
| :--------: | :------------: |
| 5025241068 | Riyan Fadli Amazzadin |
| 5025241100 | Liem, Alfred Haryanto |
| 5025221120 | Raditya Zhafran Pranuja |

# Praktikum Modul 4 _(Module 4 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - FUSecure](/task-1/)

- [Task 2 - LawakFS++](/task-2/)

- [Task 3 - Drama Troll](/task-3/)

- [Task 4 - LilHabOS](/task-4/)

### Laporan Resmi Praktikum Modul 4 _(Module 4 Lab Work Report)_
### Taks 1 - [FUSecure]
#### Setup Directory dan Mount Point
Menggunakan command ```useradd```, membuat user baru yakni ```yuadi``` dan ```irwandi```. Kemudian membuat directory baru yaitu ```/home/shared_files```, yang dengan struktur directory sebagai berikut
```
home/
└── shared_files/
    ├── private_yuadi/
    │   └── yuadi.txt
    ├── private_irwandi/
    │   └── irwandi.txt
    └── public/
        └── halo.txt
```
Selain itu, ```FUSE``` akan dimount dalam directory kosong bernama ```secure_fs```.

#### Menampilkan file dari ```/home/shared_files```  pada mount point (```secure_fs```)
```
define FUSE_USE_VERSION 28
#define BASE_PATH "/home/shared_files"
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <pwd.h>

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    char realpath[2000];
    strcpy(realpath, BASE_PATH);
    strcat(realpath, path);

    int res;
    res = lstat(realpath, stbuf);

    if (res == -1) return -errno;
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    char realpath[2000];
    strcpy(realpath, BASE_PATH);
    strcat(realpath, path);

    dp = opendir(realpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if(filler(buf, de->d_name, &st, 0)) break;
    }
    closedir(dp);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
    (void) fi; 

    const struct fuse_context *context = fuse_get_context();
    uid_t uid = context->uid;
    struct passwd *userpw = getpwuid(uid);
    
    char caller_name[200];
    strcpy(caller_name, userpw->pw_name);

    
    char realpath[2000];
    strcpy(realpath, BASE_PATH);
    strcat(realpath, path);

    if (strncmp(realpath, BASE_PATH "/private_yuadi/", strlen(BASE_PATH "/private_yuadi/")) == 0 && strcmp(caller_name, "yuadi") != 0) {
        return -EACCES;
    }
    if (strncmp(realpath, BASE_PATH "/private_irwandi/", strlen(BASE_PATH "/private_irwandi/")) == 0 && strcmp(caller_name, "irwandi") != 0) {
        return -EACCES;
    }
    
    fd = open(realpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}
```
#### Penjelasan
- Pada awal code, ```BASE_PATH``` didefinisikan sebagai ```/home/shared_files```
- Untuk mengarahkan ```FUSE``` menuju ke ```BASE_PATH```, tiap operasi yang melibatkan file akan membutuhkan variable ```realpath``` yang merupakan gabungan dari ```BASE_PATH``` dan file yang akan diakses
  <br>
  
   ```
    char realpath[2000];
    strcpy(realpath, BASE_PATH);
    strcat(realpath, path);
  ```
   sehingga ```FUSE``` akan mengarah pada path yang sesuai.

#### Read-only untuk semua user
```

static int xmp_rename(const char *from, const char *to) {
    printf("Permission denied: read only\n");
    return -EACCES;
}


static int xmp_mknod(const char *, mode_t, dev_t){
    printf("Permission denied: read only\n");
    return -EACCES;
}


static int xmp_mkdir (const char *, mode_t){
    printf("Permission denied: read only\n");
    return -EACCES;
}


static int xmp_unlink (const char *){
    printf("Permission denied: read only\n");
    return -EACCES;
}

static int xmp_rmdir (const char *){
    printf("Permission denied: read only\n");
    return -EACCES;
}


static int xmp_write (const char *, const char *, size_t, off_t, struct fuse_file_info *){
    printf("Permission denied: read only\n");
    return -EACCES;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .unlink = xmp_unlink,
    .write = xmp_write,
    .mkdir = xmp_mkdir,
    .mknod = xmp_mknod,
    .rmdir = xmp_rmdir,
    .rename = xmp_rename
};
```
#### Penjelasan
- Untuk memastikan access read-only, setiap fungsi yang dibutuhkan untuk melakukan operasi ```write``` akan me-return ```-EACCESS``` sehingga memunculkan notifikasi ```Permission denied```.
- Setelah itu, setiap fungsi yang diimplementasikan dihubungkan dalam sebuah struct operation.

#### Akses Public dan Private Folder
```
static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
    (void) fi; 

    const struct fuse_context *context = fuse_get_context();
    uid_t uid = context->uid;
    struct passwd *userpw = getpwuid(uid);
    
    char caller_name[200];
    strcpy(caller_name, userpw->pw_name);

    
    char realpath[2000];
    strcpy(realpath, BASE_PATH);
    strcat(realpath, path);

    if (strncmp(realpath, BASE_PATH "/private_yuadi/", strlen(BASE_PATH "/private_yuadi/")) == 0 && strcmp(caller_name, "yuadi") != 0) {
        return -EACCES;
    }
    if (strncmp(realpath, BASE_PATH "/private_irwandi/", strlen(BASE_PATH "/private_irwandi/")) == 0 && strcmp(caller_name, "irwandi") != 0) {
        return -EACCES;
    }
    
    fd = open(realpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}
```
#### Penjelasan
- Pada operasi read, dilakukan pengecekan untuk memastikan user mana yang sedang berusaha untuk melakukan operasi. Apabila nama user yang melakukan operasi tidak sesuai dengan pemilik folder, program akan mengembalikan -EACCESS.
- Untuk mengecek nama user

  ```
   const struct fuse_context *context = fuse_get_context();
    uid_t uid = context->uid;
    struct passwd *userpw = getpwuid(uid);
    
    char caller_name[200];
    strcpy(caller_name, userpw->pw_name);
  ```
- Untuk mengecek apabila user memiliki access pada folder tertentu (hanya untuk folder private_yuadi dan private_irwandi)
  ```
  char realpath[2000];
    strcpy(realpath, BASE_PATH);
    strcat(realpath, path);

    if (strncmp(realpath, BASE_PATH "/private_yuadi/", strlen(BASE_PATH "/private_yuadi/")) == 0 && strcmp(caller_name, "yuadi") != 0) {
        return -EACCES;
    }
    if (strncmp(realpath, BASE_PATH "/private_irwandi/", strlen(BASE_PATH "/private_irwandi/")) == 0 && strcmp(caller_name, "irwandi") != 0) {
        return -EACCES;
    }
  ```
  - Pada public directory, tidak dilakukan pengecekan sehingga semua user dapat mengakses isinya.
 
#### Screenshots
- Untuk user ```yuadi```
  
  ![yuadi](https://cdn.discordapp.com/attachments/1386312888880201841/1386320304397226024/image.png?ex=685946e5&is=6857f565&hm=28f5a89542c6ce158365795e2deba15639293e9e285824adfc5b6a117508b006&)

- Untuk user ```irwandi```
  
  ![irwandi](https://cdn.discordapp.com/attachments/1386312888880201841/1386321798752370748/image.png?ex=68594849&is=6857f6c9&hm=74d1ec19061ac9bb4c61b12c474f56a357bc0430d7eaaee4bb3a1ab6a6ea11f1&)

#  Task 2 [_LawakFS++] - A Cursed Filesystem with Censorship and Strict Access Policies_
Teja adalah seorang penggemar sepak bola yang sangat bersemangat. Namun, akhir-akhir ini, tim kesayangannya selalu tampil kurang memuaskan di setiap pertandingan. Kekalahan demi kekalahan membuat Teja muak dan kesal. "Tim lawak!" begitu umpatnya setiap kali timnya gagal meraih kemenangan. Kekecewaan Teja yang mendalam ini menginspirasi sebuah ide: bagaimana jika ada sebuah filesystem yang bisa menyensor hal-hal "lawak" di dunia ini?

Untuk mengatasi hal tersebut, kami membuat filesystem terkutuk bernama **LawakFS++** yang mengimplementasikan kebijakan akses yang ketat, filtering konten dinamis, dan kontrol akses berbasis waktu untuk file tertentu. Filesystem ini dirancang sebagai read-only dan akan menerapkan perilaku khusus untuk akses file, termasuk logging dan manajemen konfigurasi.

### a. Ekstensi File Tersembunyi

Setelah beberapa hari menggunakan filesystem biasa, Teja menyadari bahwa ekstensi file selalu membuat orang-orang bisa mengetahui jenis file dengan mudah. "Ini terlalu mudah ditebak!" pikirnya. Dia ingin membuat sistem yang lebih misterius, di mana orang harus benar-benar membuka file untuk mengetahui isinya.

Semua file yang ditampilkan dalam FUSE mountpoint harus **ekstensinya disembunyikan**.

- **Contoh:** Jika file asli adalah `document.pdf`, perintah `ls` di dalam direktori FUSE hanya menampilkan `document`.
- **Perilaku:** Meskipun ekstensi disembunyikan, mengakses file (misalnya, `cat /mnt/your_mountpoint/document`) harus dipetakan dengan benar ke path dan nama aslinya (misalnya, `source_dir/document.pdf`).

#### Solusi
```c
static int complete_name(const char *dirname, const char *basename, char *realname) {
    DIR *dp = opendir(dirname);
    if (!dp) return -1;

    struct dirent *de;
    while ((de = readdir(dp)) != NULL) {
        if (strncmp(de->d_name, basename, strlen(basename)) == 0 &&
            de->d_name[strlen(basename)] == '.') {
            strcpy(realname, de->d_name);
            closedir(dp);
            return 0;
        }
    }

    closedir(dp);
    return -1;
}

static void fullpath(char fpath[PATH_MAX], const char *path) {
    snprintf(fpath, PATH_MAX, "%s%s", source_dir, path);
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) 
{
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    char fpath[PATH_MAX];
    fullpath(fpath, path);
    dp = opendir(fpath);
    if (dp == NULL) return -errno;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    while ((de = readdir(dp)) != NULL) {
        if (strstr(de->d_name, secret_key) != NULL && !is_work_hours()) {
            continue;
        }
        if (de->d_type == DT_REG) {
            char *dot = strrchr(de->d_name, '.');
            if (dot) {
                char name[256];
                strncpy(name, de->d_name, dot - de->d_name);
                name[dot - de->d_name] = '\0';
                filler(buf, name, NULL, 0);
            } else {
                filler(buf, de->d_name, NULL, 0);
            }
        } else {
            filler(buf, de->d_name, NULL, 0);
        }
    }

    closedir(dp);
    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi) 
{
    if (secret_basename(path) && !is_work_hours()) return -ENOENT;

    char fpath[PATH_MAX];
    char realname[256];
    char base[256];
    strcpy(base, path + 1);
    if (complete_name(source_dir, base, realname) == 0) {
        snprintf(fpath, PATH_MAX, "%s/%s", source_dir, realname);
    } else {
        return -ENOENT;
    }

    int fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno;

    fi->fh = fd;
    return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf) 
{
    if (secret_basename(path) && !is_work_hours()) return -ENOENT;

    int res;
    char fpath[PATH_MAX];

    if (strcmp(path, "/") == 0) {
        fullpath(fpath, path);
    } else {
        char realname[256];
        char base[256];
        strcpy(base, path + 1);
        if (complete_name(source_dir, base, realname) == 0) {
            snprintf(fpath, PATH_MAX, "%s/%s", source_dir, realname);
        } else {
            return -ENOENT;
        }
    }

    res = lstat(fpath, stbuf);
    return res == -1 ? -errno : 0;
}
```
#### Penjelasan
```c
static int complete_name(const char *dirname, const char *basename, char *realname)
```
- `opendir(dirname)` disini kita akan membuka direktori asal yang sudah di mount.
- Lalu kita jalankan `while` untuk mengecek tiap-tiap file yang ada di direktori tersebut.
- Disini kita compare `d_name` dan `basename` untuk mencari file yang cocok karena ketika kita menggunakan `cat`, kita hanya menuliskan basename dan tidak disertai ekstensi nya.
- Jika ada yang cocok akan meng-copy nama file sebenarnya ke dalam variabel `realname` dan mengembalikan nilai 0 dan diakhiri `closedir()` untuk menutup direktori yang dibuka tadi.

```c
static void fullpath(char fpath[PATH_MAX], const char *path)
```
- Fungsi ini untuk menghasilkan path sebenarnya menuju direktori yang di mount dengan mount point.
```c
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
```
- Untuk membaca direktori mount point kita menyusun direktori lengkap nya terlebih dahulu dengan fungsi `fullpath()` lalu kita buka dengan `opendir()`.
- `filler` untuk melengkapi direktori, `.` untuk direktori saat ini dan `..` untuk direktori sebelumnya.
- Variabel `dot` disini akan menyimpan nama file tanpa ekstensi dengan fungsi `strstr()` yang hanya mengambil karakter hingga karakter tertentu.
- Setelah didapat nama yang terfilter kita panggil `filler` untuk memunculkan nama-nama file tersebut di direktori mount point dan diakhiri `closedir()`.
```c
static int xmp_open(const char *path, struct fuse_file_info *fi);
static int xmp_getattr(const char *path, struct stat *stbuf);
```
- Kedua fungsi ini memiliki cara kerja yang sama yaitu membutuhkan nama file berekstensi sehingga akan memanggil fungsi `complete_name()` dan digabung ke path direktori asli.
- Untuk `xmp_open` akan menggunakan fungsi `open()` dan `xmp_getattr` akan menggunakan fungsi `lstat`.
#### Output
![image](https://github.com/rdtzaa/assets/blob/06accd89cc5f5b3174ab0da073d385049b12c202/Sistem%20Operasi/task2_a.png)

### b. Akses Berbasis Waktu untuk File Secret

Suatu hari, Teja menemukan koleksi foto-foto memalukan dari masa SMA-nya yang tersimpan dalam folder bernama "secret". Dia tidak ingin orang lain bisa mengakses file-file tersebut kapan saja, terutama saat dia sedang tidur atau tidak ada di rumah. "File rahasia hanya boleh dibuka saat jam kerja!" putusnya dengan tegas.

File yang nama dasarnya adalah **`secret`** (misalnya, `secret.txt`, `secret.zip`) hanya dapat diakses **antara pukul 08:00 (8 pagi) dan 18:00 (6 sore) waktu sistem**.

- **Pembatasan:** Di luar rentang waktu yang ditentukan, setiap percobaan untuk membuka, membaca, atau bahkan melakukan list file `secret` harus menghasilkan error `ENOENT` (No such file or directory).
- **Petunjuk:** Kamu perlu mengimplementasikan kontrol akses berbasis waktu ini dalam operasi FUSE `access()` dan/atau `getattr()` kamu.
#### Solusi
```c
int secret_basename(const char *path) {
    const char *base = strrchr(path, '/');
    base = base ? base + 1 : path;
    return strcasecmp(base, secret_key) == 0;
}

int is_work_hours() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int hour = t->tm_hour;
    return (hour >= access_start_hour && hour < access_end_hour);
}
```
#### Penjelasan
```c
int secret_basename(const char *path)
```
- Fungsi ini akan mengambil basename dari file yang dioperasikan dalam command terminal yang kemudian akan dicocokkan dengan `strcasecmp` yang akan menghasilkan 1 jika cocok dan 0 jika tidak.
- `strcasecmp` kita gunakan agar tidak terpengaruh dengan case-sensitive.
```c
int is_work_hours()
```
- Kita gunakan fungsi `time()` dan mengambil angka hour dari struct yang dihasilkan lalu kita cek apakah angka hour berada diantara jam akses yang ditentukan.

Fungsi-fungsi diatas akan dipanggil diawal di fungsi `xmp_getattr`, `xmp_access`, `xmp_open`, `xmp_readdir`, dan `xmp_read` yang mana fungsi-fungsi tersebut yang mengatasi ketika user akan membaca suatu file atau mengecek suatu file seperti `ls` dan `cat`.
#### Output
- Akses berhasil:
![image](https://github.com/rdtzaa/assets/blob/06accd89cc5f5b3174ab0da073d385049b12c202/Sistem%20Operasi/akses_acc.png)

- Akses gagal:
![image](https://github.com/rdtzaa/assets/blob/06accd89cc5f5b3174ab0da073d385049b12c202/Sistem%20Operasi/akses_acc.png)

### c. Filtering Konten Dinamis

Kekesalan Teja terhadap hal-hal "lawak" semakin memuncak ketika dia membaca artikel online yang penuh dengan kata-kata yang membuatnya kesal. Tidak hanya itu, gambar-gambar yang dia lihat juga sering kali tidak sesuai dengan ekspektasinya. "Semua konten yang masuk ke sistem saya harus difilter dulu!" serunya sambil mengepalkan tangan.

Ketika sebuah file dibuka dan dibaca, isinya harus **secara dinamis difilter atau diubah** berdasarkan tipe file yang terdeteksi:

| Tipe File      | Perlakuan                                                                                 |
| :------------- | :---------------------------------------------------------------------------------------- |
| **File Teks**  | Semua kata yang dianggap lawak (case-insensitive) harus diganti dengan kata `"lawak"`.    |
| **File Biner** | Konten biner mentah harus ditampilkan dalam **encoding Base64** alih-alih bentuk aslinya. |

> **Catatan:** Daftar "kata-kata lawak" untuk filtering file teks akan didefinisikan secara eksternal, seperti yang ditentukan dalam persyaratan **e. Konfigurasi**.
#### Solusi
```c
void filter_text(char *buf) {
    char *lower_buf = strdup(buf);
    if (!lower_buf) return;
    to_lower_str(lower_buf);

    for (int i = 0; i < filter_word_count; i++) {
        char *pos = lower_buf;
        size_t word_len = strlen(filter_words[i]);
        size_t replace_len = strlen("lawak");

        while ((pos = strstr(pos, filter_words[i])) != NULL) {
            size_t offset = pos - lower_buf;
            memcpy(buf + offset, "lawak", replace_len);
            if (replace_len < word_len) {
                memset(buf + offset + replace_len, ' ', word_len - replace_len);
            }
            memcpy(pos, "lawak", replace_len);

            pos += replace_len;
        }
    }
    free(lower_buf);
}

static const char base64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64_encode(const unsigned char *src, size_t len, char *out) {
    size_t i, j;
    for (i = 0, j = 0; i < len;) {
        uint32_t octet_a = i < len ? src[i++] : 0;
        uint32_t octet_b = i < len ? src[i++] : 0;
        uint32_t octet_c = i < len ? src[i++] : 0;

        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        out[j++] = base64_table[(triple >> 3 * 6) & 0x3F];
        out[j++] = base64_table[(triple >> 2 * 6) & 0x3F];
        out[j++] = base64_table[(triple >> 1 * 6) & 0x3F];
        out[j++] = base64_table[(triple >> 0 * 6) & 0x3F];
    }
    for (i = 0; i < ((3 - len % 3) % 3); i++)
        out[j - 1 - i] = '=';
    out[j] = '\0';
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) 
{
    if (secret_basename(path) && !is_work_hours()) return -ENOENT;
    off_t file_size = lseek(fi->fh, 0, SEEK_END);
    lseek(fi->fh, 0, SEEK_SET);

    unsigned char *file_buf = malloc(file_size + 1);
    if (!file_buf) return -ENOMEM;

    ssize_t read_bytes = read(fi->fh, file_buf, file_size);
    if (read_bytes < 0) {
        free(file_buf);
        return -errno;
    }
    file_buf[read_bytes] = 0;

    int is_text = 1;
    for (ssize_t i = 0; i < read_bytes && i < 1024; i++) {
        if ((file_buf[i] < 32 || file_buf[i] > 126) && file_buf[i] != '\n' && file_buf[i] != '\r' && file_buf[i] != '\t') {
            is_text = 0;
            break;
        }
    }

    int ret = 0;

    if (is_text) {
        filter_text((char *)file_buf);

        size_t len = strlen((char *)file_buf);
        if (offset < (off_t)len) {
            if (offset + size > len) size = len - offset;
            memcpy(buf, file_buf + offset, size);
            ret = size;
        } else {
            ret = 0;
        }
    } else {
        size_t encoded_len = 4 * ((read_bytes + 2) / 3);
        char *encoded_buf = malloc(encoded_len + 1);
        if (!encoded_buf) {
            free(file_buf);
            return -ENOMEM;
        }

        base64_encode(file_buf, read_bytes, encoded_buf);

        size_t enc_len = strlen(encoded_buf);
        if (offset < (off_t)enc_len) {
            if (offset + size > enc_len) size = enc_len - offset;
            memcpy(buf, encoded_buf + offset, size);
            ret = size;
        } else {
            ret = 0;
        }
        free(encoded_buf);
    }

    free(file_buf);

    if (ret >= 0) {
        log_access("READ", path);
    }

    return ret;
}
```
#### Penjelasan
```c
void filter_text(char *buf)
```
- `lower_buf` dibuat dari `buf` dengan semua huruf diubah menjadi huruf kecil dengan fungsi `to_lower_str`.
- Kita gunakan `for` loop untuk setiap kata dalam `filter_words` untuk mencari posisi tiap kata tersebut dalam `lower_buf` menggunakan `strstr()`. Jika cocok, `memcpy` mengganti isi asli di buf dengan "lawak". Jika kata aslinya lebih panjang dari "lawak", sisanya diisi dengan spasi.

```c
void base64_encode(const unsigned char *src, size_t len, char *out)
```
- Membaca tiap 3 byte (24-bit) lalu membaginya menjadi 4 grup 6-bit.
- Lalu, setiap grup digunakan untuk mengambil 1 karakter dari `base64_table` sesuai dengan index yang didapat.
- Jika ukuran file tidak kelipatan 3, ditambahkan karakter `=` sebagai pengisi agar menjadi kelipatan 3.
```c
static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
```
- Diawali dengan mengecek `secret` dan `access_hour`
- Lalu membuat variabel untuk ukuran buffer dengan `lseek`
- Gunakan `read` untuk mendapatkan ukuran byte dan mengcopy isi ke dalam buffer yang sudah dibuat.
- Selanjutnya gunakan looping kembali untuk mengecek apakah hasil buffer adalah sebuah text atau gambar
- Nah jika merupakan teks kita langsung panggil fungsi `filter_text()` dan `memcpy` ke `buf` untuk ditampilkan di terminal, sebaliknya jika itu adalah gambar maka kita panggil `base64_encode` dan hasil encode nya kita `memcpy` ke buf untuk ditampilkan.
#### Output
![image](https://github.com/rdtzaa/assets/blob/06accd89cc5f5b3174ab0da073d385049b12c202/Sistem%20Operasi/filter_text_gambar.png)

### d. Logging Akses

Sebagai seorang yang paranoid, Teja merasa perlu untuk mencatat setiap aktivitas yang terjadi di filesystemnya. "Siapa tahu ada yang mencoba mengakses file-file penting saya tanpa izin," gumamnya sambil menyiapkan sistem logging. Dia ingin setiap gerakan tercatat dengan detail, lengkap dengan waktu dan identitas pelakunya.

Semua operasi akses file yang dilakukan dalam LawakFS++ harus **dicatat** ke file yang terletak di **`/var/log/lawakfs.log`**.

Setiap entri log harus mematuhi format berikut:

```
[YYYY-MM-DD HH:MM:SS] [UID] [ACTION] [PATH]
```

Di mana:

- **`YYYY-MM-DD HH:MM:SS`**: Timestamp operasi.
- **`UID`**: User ID pengguna yang melakukan aksi.
- **`ACTION`**: Jenis operasi FUSE (misalnya, `READ`, `ACCESS`, `GETATTR`, `OPEN`, `READDIR`).
- **`PATH`**: Path ke file atau direktori dalam FUSE mountpoint (misalnya, `/secret`, `/images/photo.jpg`).

> **Persyaratan:** Kamu **hanya diwajibkan** untuk mencatat operasi `read` dan `access` yang berhasil. Logging operasi lain (misalnya, write yang gagal) bersifat opsional.
#### Solusi
```c
static void log_access(const char *action, const char *path) {
    FILE *logfile = fopen("/var/log/lawakfs.log", "a");
    if (!logfile) return;

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestr[20];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", tm_info);

    uid_t uid = fuse_get_context()->uid;

    fprintf(logfile, "[%s] [%d] [%s] [%s]\n", timestr, uid, action, path);
    fclose(logfile);
}
```
#### Penjelasan
- Membuka path file log dengan `fopen` dengan akses `a` yaitu jika belum ada akan membuat dan jika sudah ada akan membuka file yang sudah ada.
- Dapatkan catatan waktu dengan `time`.
- Dapatkan uid dengan `fuse_get_context` agar mendapatkan uid yang mengakses mount point tersebut
- `fprintf` untuk mencatat ke dalam file log dengan action READ atau ACCESS dan diakhiri `fclose` untuk menutup file.
- Fungsi ini akan dipanggil di akhir fungsi `xmp_access` dan `xmp_read`.
#### Output
![image](https://github.com/rdtzaa/assets/blob/06accd89cc5f5b3174ab0da073d385049b12c202/Sistem%20Operasi/log.png)

### e. Konfigurasi

Setelah menggunakan filesystemnya beberapa minggu, Teja menyadari bahwa kebutuhannya berubah-ubah. Kadang dia ingin menambah kata-kata baru ke daftar filter, kadang dia ingin mengubah jam akses file secret, atau bahkan mengubah nama file secret itu sendiri. "Saya tidak mau repot-repot kompilasi ulang setiap kali ingin mengubah pengaturan!" keluhnya. Akhirnya dia memutuskan untuk membuat sistem konfigurasi eksternal yang fleksibel.

Untuk memastikan fleksibilitas, parameter-parameter berikut **tidak boleh di-hardcode** dalam source code `lawak.c` kamu. Sebaliknya, mereka harus dapat dikonfigurasi melalui file konfigurasi eksternal (misalnya, `lawak.conf`):

- **Nama file dasar** dari file 'secret' (misalnya, `secret`).
- **Waktu mulai** untuk mengakses file 'secret'.
- **Waktu berakhir** untuk mengakses file 'secret'.
- **Daftar kata-kata yang dipisahkan koma** yang akan difilter dari file teks.

**Contoh konten `lawak.conf`:**

```
FILTER_WORDS=ducati,ferrari,mu,chelsea,prx,onic,sisop
SECRET_FILE_BASENAME=secret
ACCESS_START=08:00
ACCESS_END=18:00
```

FUSE kamu harus membaca dan mem-parse file konfigurasi ini saat inisialisasi.
#### Solusi
```c
int init_conf(const char *config_path) {
    FILE *f = fopen(config_path, "r");
    if (!f) return -1;

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (strncmp(line, "FILTER_WORDS=", 13) == 0) {
            char *p = line + 13;
            char *token = strtok(p, ",");
            while(token && filter_word_count < MAX_FILTER_WORDS) {
                filter_words[filter_word_count] = strdup(token);

                for (char *c = filter_words[filter_word_count]; *c; c++) {
                    *c = tolower(*c);
                }
                filter_word_count++;
                token = strtok(NULL, ",");
            }
        } else if (strncmp(line, "SECRET_FILE_BASENAME=", 21) == 0) {
            strcpy(secret_key, line + 21);
            trim(secret_key);
        } else if (strncmp(line, "ACCESS_START=", 13) == 0) {
            sscanf(line + 13, "%d:%*d", &access_start_hour);
        } else if (strncmp(line, "ACCESS_END=", 11) == 0) {
            sscanf(line + 11, "%d:%*d", &access_end_hour);
        }
    }
    fclose(f);
    return 0;
}

int main(int argc, char *argv[]) {
    umask(0);

    if (init_conf("/home/rdtzaaa/task-2/lawak.conf") != 0) {
        printf("Gagal membaca konfigurasi lawak.conf\n");
        return 1;
    }

    return fuse_main(argc, argv, &xmp_oper, NULL);
}
```
#### Penjelasan
- Fungsi ini akan dipanggil diawal ketika program berjalan.
- Gunakan `fopen` untuk membuka path file log yang dibuat yaitu `/var/log/lawakfs/log`
- Gunakan `trim()` untuk menghapus kemungkinan spasi dalam mengisi config
- Gunakan `while` loop untuk mengecek setiap baris dari config tersebut dan gunakan `strtok` untuk mengatasi kata kunci yang lebih dari satu sehingga membutuhkan tanda koma.
#### Output
![image](https://github.com/rdtzaa/assets/blob/06accd89cc5f5b3174ab0da073d385049b12c202/Sistem%20Operasi/lawakconf.png)
### Revisi
Mengganti fungsi `get_uid()` menjadi `fuse_get_context()` agar bisa mendapatkan uid user lain.

### Taks 3 - [Drama Troll]
#### Inisialisasi User dan Directory
Untuk membuat user, digunakan ```useradd``` untuk masing-masing user yang dibutuhkan. Kemudian, membuat directory baru yaitu ```/mnt/troll``` dengan struktur:
```
/mnt/
└── troll/
    ├── upload.txt
    └── very_spicy_info.txt

```
#### ```cat very_spicy_info.txt``` untuk ```DainTotas``` dan user selain ```DainTotas```
Untuk user selain DainTotas, ```very_spicy_info.txt``` akan berisi: 
```
DainTotas' personal secret!!.txt
```
karena itu, teks diatas dimasukkan pada file ```very_spicy_info.txt``` dengan perintah ```echo```.
```
 if(strcmp(caller_name, "DainTotas")==0){
        if(is_timpa_teks()){
            const char *ascii_art = timpa_teks();
            size_t art_len = strlen(ascii_art);

            if (offset < art_len) {
                size_t to_copy = (offset + size > art_len) ? art_len - offset : size;
                memcpy(buf, ascii_art + offset, to_copy);
                return to_copy;
            } 
            else  return 0;
        }
        else if(strcmp(path, "/very_spicy_info.txt")==0){
            const char *message = "Very spicy internal developer information: leaked roadmap.docx\n";
            size_t msg_len = strlen(message);

            if (offset < msg_len) {
                size_t to_copy = (offset + size > msg_len) ? msg_len - offset : size;
                memcpy(buf, message + offset, to_copy);
                return to_copy;
            } 
            else return 0;
        }
    }
```
#### Penjelasan
- Menggunakan cara seperti pada task 1, FUSE melakukan pengecekan terhadap nama user yang melakukan operasi. Apabila nama user adalah ```DainTotas``` dan jebakan belum aktif, akan mengeluarkan message sesuai yang diminta soal.
- Apabila user bukan ```DainTotas```, tampilkan file seperti file biasa.

#### Trap dan Trigger Trap 
Trap akan aktif ketika ```DainTotas``` telah melakukan ```echo upload > upload.txt```. Karena itu, dilakukan cek apakah file ```upload.txt``` telah memiliki isi atau masih kosong. 

- Fungsi untuk mengecek isi file ```upload.txt```:
  ```
      bool is_timpa_teks(){
        char buf[7];  
        struct fuse_file_info *fi;
        int fd;
        int res;
        (void) fi; 
    
        char realpath[2000];
        strcpy(realpath, BASE_PATH);
        strcat(realpath, "/upload.txt");
    
        fd = open(realpath, O_RDONLY);
    
        if (fd == -1) return false;
    
        res = pread(fd, buf, 6, 0);
    
        if (res == -1){
            close(fd);
            return false;
        }
    
        char tmp[res + 1];        
        memcpy(tmp, buf, res); 
        tmp[res] = '\0';         
    
        printf("%s %s\n", tmp, buf);
        if (strcmp(tmp, "upload") == 0) {
            close(fd);
            return true;
        }
    
        close(fd);
        return false;
  }
  ```
- Fungsi tersebut akan membuka ```upload.txt```, menyalin isinya pada buffer, dan mengembalikan true apabila ```upload.txt``` berisi ```upload``` 
- Apabila bernilai true, akan menampilkan ```ascii art``` apabila ```DainTotas``` mencoba melakukan cat pada file manapun. Diimplementasikan melalui program berikut:
```
const char* timpa_teks() {

    return "_____    _ _    __              _ _                       _                                        _ \n"
    "|  ___|__| | |  / _| ___  _ __  (_) |_    __ _  __ _  __ _(_)_ __    _ __ _____      ____ _ _ __ __| |\n"
    "| |_ / _ \\ | | | |_ / _ \\| '__| | | __|  / _` |/ _` |/ _` | | '_ \\  | '__/ _ \\ \\ /\\ / / _` | '__/ _` |\n"
    "|  _|  __/ | | |  _| (_) | |    | | |_  | (_| | (_| | (_| | | | | | | | |  __/\\ V  V / (_| | | | (_| |\n"
    "|_|  \\___|_|_| |_|  \\___/|_|    |_|\\__|  \\__,_|\\__, |\\__,_|_|_| |_| |_|  \\___| \\_/\\_/ \\__,_|_|  \\__ _|\n"
    "                                               |___/                                                  \n";
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    char realpath[2000];
    strcpy(realpath, BASE_PATH);
    strcat(realpath, path);

    int res = lstat(realpath, stbuf);
    if (res == -1) return -errno;

        const struct fuse_context *context = fuse_get_context();
        uid_t uid = context->uid;
        struct passwd *userpw = getpwuid(uid);
        
        char caller_name[200];
        strcpy(caller_name, userpw->pw_name);

    if (strcmp(caller_name, "DainTotas")==0) {
        if (is_timpa_teks()) {
            size_t art_len = strlen(timpa_teks());
            stbuf->st_size = art_len;
            return 0;
        }
        else if (strcmp(path, "/very_spicy_info.txt") == 0) {
            const char *msg = "Very spicy internal developer information: leaked roadmap.docx\n";
            stbuf->st_size = strlen(msg);
            return 0;
        }
    }
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
    (void) fi; 

    const struct fuse_context *context = fuse_get_context();
    uid_t uid = context->uid;
    struct passwd *userpw = getpwuid(uid);
    
    char caller_name[200];
    strcpy(caller_name, userpw->pw_name);

    char realpath[2000];
    strcpy(realpath, BASE_PATH);
    strcat(realpath, path);

    if(strcmp(caller_name, "DainTotas")==0){
        if(is_timpa_teks()){
            const char *ascii_art = timpa_teks();
            size_t art_len = strlen(ascii_art);

            if (offset < art_len) {
                size_t to_copy = (offset + size > art_len) ? art_len - offset : size;
                memcpy(buf, ascii_art + offset, to_copy);
                return to_copy;
            } 
            else  return 0;
        }
        else if(strcmp(path, "/very_spicy_info.txt")==0){
            const char *message = "Very spicy internal developer information: leaked roadmap.docx\n";
            size_t msg_len = strlen(message);

            if (offset < msg_len) {
                size_t to_copy = (offset + size > msg_len) ? msg_len - offset : size;
                memcpy(buf, message + offset, to_copy);
                return to_copy;
            } 
            else return 0;
        }
    }
    
    fd = open(realpath, O_RDONLY);
    if (fd == -1) return -errno;
    res = pread(fd, buf, size, offset);
    if (res == -1) res = -errno;
    close(fd);
    return res;
}
```
#### Penjelasan
- getattr sedikit dimodifikasi agar ```stbuf->size``` sesuai dengan ukuran string yang ingin ditampilkan
- ketika ```is_timpa_teks()``` bernilai ```true``` dan user adalah ```DainTotas```, tampilkan ```ascii_art``` melalui fungsi ```xmp_read()``` yang telah dimodifikasi, dengan cara meng-copy ```asci_art``` ke ```buf``` melalui ```memcpy```

#### Screenshots
- User lain mengakses ```very_spicy_info.txt``` sebelum jebakan aktif
  
  ![Image](https://cdn.discordapp.com/attachments/1386312888880201841/1386330088110424104/image.png?ex=68595001&is=6857fe81&hm=8a39a5619672fa9da0669100b22b6c228d628af7e1af98660df5d33278fc7bd9&)

- User lain setelah jebakan aktif
  
 ![Image](https://cdn.discordapp.com/attachments/1386312888880201841/1386329389414875269/image.png?ex=68594f5b&is=6857fddb&hm=783adebb9aa8197a4335b392d51472cf9c51d338f7e9e73626c2c746506be5f0&)


- ```DainTotas``` sebelum jebakan aktif
  
  ![Image](https://cdn.discordapp.com/attachments/1386312888880201841/1386330460942110821/image.png?ex=6859505a&is=6857feda&hm=3867756e463920adc6ffae9f430ed85f100ba743987cd3d98fdbec4960b26286&)

  
- ```DainTotas``` setelah jebakan aktif
  
  ![Image](https://cdn.discordapp.com/attachments/1386312888880201841/1386329132417417386/image.png?ex=68594f1d&is=6857fd9d&hm=bb9acdbb813d1d1cb7d51db7886fbc181e8d25462340bacf373c3e8ae295584b&)

# 4. LilHabOS

a. Implementasikan fungsi `printString`, `readString`, dan `clearScreen` di `kernel.c` yang akan menampilkan dan membaca string di layar.


Untuk melengkapi fungsi `printString()`, `readString()`, dan `printString()`, maka perlu untuk menggunakan interrupt. Untuk `printString()` dan `printString()`, maka dapat menggunakan `int 10h `yang merupakan video interrupt kepada bios, dan untuk menerima input dari keyboard dapat menggunakan `int 16h`, yakni interrupt untuk menerima input dari keyboard.

`printString()`

``` c 
void printString(char* str) {
    int i=0;
    while(str[i] != '\0'){
        if(strcmp(str[i],'\n')){endl();break;}
        interrupt(0x10,0xE00|str[i],0,0,0);i++;
    }
}
```
Untuk `printString()` sendiri akan menerima sebuah variabel string sehingga untuk menampilkan nya pada layar dapat menggunakan konfigurasi `(10,0xE00 | str[i],0,0,0)`. 10 berarti untuk video interrupt, `0xE00` sebagai teletype function yang akan melakukan print di layar, dan str[i] yang akan dipassing kepada `0xE00`. Fungsi ini akan terus berjalan sampai bertemu terminator `'\0'`. Bila bertemu dengan `\n`, maka akan memprint line baru.

`readstring()`

``` c
void readString(char* buf) {
    int i=0;char c;
    while(1){
        c = interrupt(0x16, 0x0, 0, 0, 0); 
        if(c==0x0D){  ~
            buf[i]='\0';
            interrupt(0x10,0xE00|0x0D,0,0,0);
            break;
        } else if(c == 0x8){ 
            if(i>0){
                interrupt(0x10,0xE00|0x8,0,0,0); 
                interrupt(0x10,0xE00|0x20,0,0,0); 
                interrupt(0x10,0xE00|0x8,0,0,0); 
                i--;
            }
        } else {
            buf[i]=c;
            interrupt(0x10,0xE00|c,0,0,0); 
            i++;
        }
    }
}
```

Untuk fungsi `readstring()`, karena input dari keyboard "tidak memiki batasan", maka menggunakan while(1) yang mana loop tersebut akan berlangsung terus menerus sampai user menekam tombol enter.
- Input dari interrupt `(0x16, 0x0, 0, 0, 0)` akan disimpan pada variabel `c`.
- Terdapat 3 kemungkinan pada input keyboard sederhana, yang pertama adalah `enter`, kedua adalah `backspace`, dan ketika input karakter/angka. 
- `enter` Ketika user menginput/menekan tombol `enter`, c akan bernilai `0x0D`, maka string tersebut akan langsung diberi `\0` pada indeks terakhir, menandakan akhir dari string tersebut, kemudian melakukan print sehingga kursor dimulai pada line baru, dan melakukan break pada while loop, menandakan fungsi `readstring()` telah berhenti dan agar program dapat berlanjut.
- `backspace` Ketika user menginput/menekan backspace, input yang didapat berupa `0x8`, untuk "menghapus" sebuah karakter pada line sendiri memiliki 3 mekanisme. Mekanisme pertama menggunakan interrupt dengan konfigurasi `(0x10,0xE00|0x8,0,0,0)`, yang berguna untuk memundurkan kursor sebanyak sekali kebelakang, kemudian interrupt dengan konfigurasi `(0x10,0xE00|0x20,0,0,0)` untuk menampilkan blank space (overwrite karakter), dan yang terakhir seperti pada mekanisme pertama yakni memundurkan kursor nya lagi. Selain mengubah tampilan, i (sebagai indexer pada variabel tempat menuliskan stringnya) akan di-decrement, sehingga akan secara tidak langsung "menghapus" karaker pada index yang memang ingin dihapus. 

`clearScreen()`

``` c 
void clearScreen(){
    interrupt(0x10,0x600,0x700,0x0,0x184F);
    rowcount=1;
    interrupt(0x10,0x200,0,rowcount++,0);
}
```

Function `clearScreen()` memiliki 2 mekanisme utama untuk menghapus seluruh layar dan menempatkan kursor pada bagian kiri atas. 
- Mekanisme pertama menggunakan interrupt `(0x10,0x600,0x700,0x0,0x184F)`, yang bertugas untuk mem-flush seluruh screen dengan melakukan scroll pada seluruh visible window, dan mengisinya dengan blank space. 
- Mekanisme kedua adalah dengan menggunakan interrupt `(0x10,0x200,0,rowcount++,0)` yang akan membawa kursor ke paling kiri setelah melakukan flush pada seluruh screen. 


b. Lengkapi implementasi fungsi-fungsi di `std_lib.h` dalam `std_lib.c.`
`std_lib.h`
``` c
#ifndef __STD_LIB_H__
#define __STD_LIB_H__

#include "std_type.h"

int div(int a, int b);
int mod(int a, int b);

void memcpy(byte* src, byte* dst, unsigned int size);
unsigned int strlen(char* str);
bool strcmp(char* str1, char* str2);
void strcpy(char* src, char* dst);
void clear(byte* buf, unsigned int size);

#endif // __STD_LIB_H__
```

`std_lib.c`
``` c
#include "std_lib.h"

int div(int a,int b){
    int res=0;
    while(a>=b){a-=b;res++;}
    return res;
}

int mod(int a,int b){
    while(a>=b)a-=b;
    return a;
}

void memcpy(byte* src, byte* dst, unsigned int size){
    int i=0;
    for(i;i<size;i++)dst[i]=src[i];
}

unsigned int strlen(char* str){
    int res=0,i=0;
    while(str[i]!='\0'){res++;i++;}
    return res;
}

bool strcmp(char* str1, char* str2){
    int i=0;
    while(str1[i]!='\0'&&str2[i]!='\0'){if(str1[i]!=str2[i])return false;i++;}
    return str1[i]==str2[i];
}

void strcpy(char* src, char* dst){
    int i=0;
    while(src[i]!='\0'){dst[i]=src[i];i++;}
    dst[i]='\0';
}

void clear(byte* buf, unsigned int size){
    int i=0;
    for(i;i<size;i++)buf[i]=0;
}
```

Dalam `std_lib.c` berisi implementasi dari fungsi-fungsi sederhana yang ada pada `std_lib.h`.
- `div()` Division/pembagian yaitu seberapa banyak suatu bilangan dapat dibagi oleh bilangan lain. Dalam kasus ini, dapat diimplementasikan dengan menghitung seberapa banyak bilangan `a` dapat dibagi oleh bilangan `b`, sehingga yang di return adalah jumlah hasil pengurangan nya (hasil pembagian).
- `mod()` Modulo singkatnya merupakan sisa pembagian dari suatu bilangan. Implementasinya hampir mirip dengan `div()`, namun karena yang dicari adalah sisa hasil pembagian, maka yang direturn adalah sisa dari variabel `a` yang sudah dibagi oleh variabel `b`. 
- `memcpy()` Memory Copy bertugas untuk "menduplikasi" isi dari suatu memori ke memori lainya. Karena diberikan variabel `src` (source), `dst` (destination), dan `size`, maka dapat menggunakan for loop untuk melakukan copy dari src[i] ke dst[i] sampai sejumlah size. 
- `strlen()` String Length berguna untuk menghitung panjang dari sebuah string. Implementasinya hanya membutuhkan sebuah variabel string, yang kemudian diakses dengan menggunakan while loop sampai akses ke variabel `str` mencapai terminator (`\0`). Setiap kali akses berhasil dan belum menjumpai terminator, variabel `res` (result) akan diincrement, menambah jumlah/panjang dari string tersebut. Nilai yang di return adalah panjang total dari variabel/string tersebut.
- `strcmp()` String Compare berfungsi untuk membandingkan 2 buah string. Implementasi dapat dengan membandingkan kedua string, selama `str1` dan `str2` bukan merupakan terminator, maka loop akan berjalan. Bila dijumpai str1[i] tidak sama dengan str2[i], maka akan langsung return false. Bila salah satu/keduanya adalah terminator, maka loop akan berhenti. Untuk benar-benar memastikan kedua string sama, return kembali membandingkan apakah index terakhir tetap sama (yakni terminator). Bila sama maka akan return true, bila tidak maka akan return false.
- `strcpy()` String Copy memiliki mekanisme yang sama dengan `memcpy()`, yakni mengcopy sebuah string ke variabel string lainya. While loop akan berjalan sampai bertemu terminator, bila sudah bertemu, loop akan berhenti dan pada variabel `dst` akan diberi terminator di index terakhirnya. 
- `clear()` Clear berguna untuk menghapus/mengosongkan sebuah variabel, dalam kasus ini untuk mengosongkan variabel `buf`. Karena diberikan size nya, maka dapat menggunakan for loop sebanyak size, dan setiap index pada `buf` akan diberi nilai 0 yang berarti kosong.

# Mekanisme `Pipe` dan `Command Check`
``` c
int pipe(char* input, char parts[][256]){
    int i=0,j=0,k=0;
    while(input[i]!='\0'){
        if(input[i]=='|'){
            parts[k][j]='\0';
            k++;j=0;i++;
            while(input[i]==' ')i++;
        } else parts[k][j++]=input[i++];
    }
    parts[k][j]='\0';
    return k + 1;
}
```
Karena input dapat berupa pipe, maka diperlukan pemisahan/pembagian pada setiap bagian sebelum dilakukan pipe. 
- `pipe` akan menyimpan hasil dari bagian yang sudah dipisahkan pada variabel `parts[][]`, sebuah array 2d. Index pertama akan menyimpan bagian dari piping, dan index kedua akan menyimpan seluruh string per bagianya, dengan jumlah maksimal dalam kasus ini 256. 
- Selama input[i] bukan terminator, input akan disimpan pada `parts`, bila input[i] merupakan `|`, maka bagian pertama akan diberi terminator pada index terakhir, dan `k` akan diincrement, menandakan partisi/bagian baru dari input. Bila sudah selesai semua, maka bagian terakhir akan diberikan terminator juga dan karena menggunakan indexin array yang dimulai dari 0, maka dalam return nilai harus ditambah dengan 1 agar sesuai dengan nilai aslinya. 

``` c
char vecho[]="echo ";
char vgrep[]="grep ";
char vc[]="wc";

int echoCheck(char* buf){
    int i=0;
    while (*buf == ' ') buf++;
    for(i;i<5;i++) if(buf[i]!=vecho[i]) return 0;
    return 1;
}

int grepCheck(char* buf){
    int i=0;
    while (*buf == ' ') buf++;
    for(i;i<5;i++) if(buf[i]!=vgrep[i]) return 0;
    return 1;
}

int wcCheck(char* buf){
    int i=0;
    while (*buf == ' ') buf++;
    for(i;i<2;i++) if(buf[i]!=vc[i]) return 0;
    return 1;
}  
``` 
Pengecekan menggunakan metode simple yaitu membandingkan keyword dengan 5 karakter (2 pada wc), bila sama maka akan return 1, bila tidak maka akan return 0. Blank space sangat krusial dan harus di cek karena bila command `echo` atau `grep` hanya boleh memiliki 4 karakter tersebut, tidak boleh ada karakter lain yang tergabung. Untuk `wc` sendiri, karena tidak perlu memiki argument, maka hanya perlu membandingkan untuk "wc" saja dan tidak perlu untuk blank space nya. 


c. Implementasikan perintah `echo`
```c
if (echoCheck(parted[0]) && count == 1) {
    char* echoed = parted[0] + 5;
    while (*echoed == ' ') echoed++;
    printString(echoed);
    endl();
}
```

Untuk implementasi dari `echo`, singkatnya langsung menggunakan `printString()`.
- Karena isi dari parted yang berbentuk misalnya `echo hello world`, 5 karakter pertama pasti berisi command nya, sehingga untuk menghindari hal tersebut dibuatlah variabel baru yaitu `echoed` yang sudah diberi ofset sebesar 5 index. Bila ternyata masih ada blank space pada `echoed`, maka akan di trace sampai ditemukan karakter sehingga isi dari `echoed` adalah argument dari command `echo`.
- Bila sudah didapatkan argumen dari `echo`, maka dapat di passing ke `printString()`.


d. Implementasikan perintah `grep`
``` c
int iniDiaGrep(char* ygecho, char* yggrep, char* grepres,int pr) {
    int i,wstart,outIndex,matched,wi,gi,gwi,m,n,wj,match;
    char word[64],grepword[64];
    i = wstart = outIndex = matched = 0;

    while (1) {
        wi = 0;
        while (ygecho[i] == ' ') i++;
        if (ygecho[i] == '\0') break;

        wstart = i;
        while (ygecho[i] != ' ' && ygecho[i] != '\0') word[wi++]= ygecho[i++];
        word[wi] = '\0';

        gi=0,match=0;
        while(1){
            gwi = 0;
            while(yggrep[gi] == ' ') gi++;
            if(yggrep[gi] == '\0') break;

            while(yggrep[gi] != ' ' && yggrep[gi] != '\0') grepword[gwi++]=yggrep[gi++];
            grepword[gwi] = '\0';

            m = 0;
            while(word[m] != '\0'){
                n = 0;
                while(word[m + n] == grepword[n] && grepword[n] != '\0') n++;
                if (grepword[n] == '\0'){match=1;break;}
                m++;
            }

            if (match) break;
        }

        if (match){
            wj = 0;
            while (word[wj] != '\0') grepres[outIndex++] = word[wj++];
            grepres[outIndex++] = ' ';
            matched = 1;
        }
    }

    if(matched){
        if (outIndex > 0) grepres[outIndex - 1] = '\0'; 
        else grepres[0] = '\0';
        if(pr==1)printString(grepres);
        return 1;
    } else {
        grepres[0] = '\0';
        if(pr==1)printString("NULL");
        return 0;
    }
}
```

`grep` disini akan mengambil sebuah kata dari argumen echo (bila argumen lebih dari 1 kata), kemudian membandingkanya dengan setiap argumen dari `grep`.

1. Kasus `echo <argument> | grep <argument>`
``` c
if (echoCheck(parted[0])&&wcCheck(parted[1])){
    echoed=parted[0]+5;
    while (*echoed == ' ') echoed++;
    iniDiaWc(echoed);
    endl();
}
```

2. Kasus `echo <argument> | grep <argument> | wc`
``` c
if(echoCheck(parted[0])&&grepCheck(parted[1])&&wcCheck(parted[2])){
    echoed=parted[0]+5;
    while (*echoed == ' ') echoed++;
    greped=parted[1]+5;
    while (*greped == ' ') greped++;
    if(iniDiaGrep(echoed,greped,grepres,0)){endl();iniDiaWc(grepres);}
    endl();
}
```


Pada while loop pertama, terdapat potongan baris code berikut yang bertugas untuk mengcopy kata pertama dari argumen echo kedalam variabel `word`.
``` c
while (ygecho[i] != ' ' && ygecho[i] != '\0') word[wi++]= ygecho[i++];
word[wi] = '\0';
```

While loop kedua juga bertugas untuk mengambil kata pertama dari argument grep, dan meletakkanya pada variabel `grepword`.
``` c
while(yggrep[gi] != ' ' && yggrep[gi] != '\0') grepword[gwi++]=yggrep[gi++];
grepword[gwi] = '\0';
``` 

Potongan code berikut akan membandingkan `word` dengan `grepword`
``` c
while(word[m] != '\0'){
    n = 0;
    while(word[m + n] == grepword[n] && grepword[n] != '\0') n++;
    if (grepword[n] == '\0'){match=1;break;}
    m++;
}
```
Bila `word` dan `grepword` sama, maka `grepword` akan berada pada terminator, yang akan mentrigger flag `match` dan melakukan break loop nya. 

Bila match bernilai 1 (true), maka word yang sesuai akan diletakkan pada variabel `grepres` (berguna untuk wc).
``` c
if (match){
    wj = 0;
    while (word[wj] != '\0') grepres[outIndex++] = word[wj++];
    grepres[outIndex++] = ' ';
    matched = 1;
}
```

Function grep juga akan menerima variabel untuk menandakan hasil grep ingin di print atau tidak (berguna untuk wc).
``` c
if(matched){
    if (outIndex > 0) grepres[outIndex - 1] = '\0'; 
    else grepres[0] = '\0';
    if(pr==1)printString(grepres);
    return 1;
} else {
    grepres[0] = '\0';
    if(pr==1)printString("NULL");
    return 0;
}?
```


e. Implementasikan perintah `wc`
``` c
void iniDiaWc(char* ygdikasi) {
    int i=0,words=0,chars=0,inWord=0;
    if(strcmp(ygdikasi,"NULL")==1){
    }else {
        while(ygdikasi[i]!='\0'){
            chars++;
            if(ygdikasi[i]!=' '&&inWord==0){
                words++;
                inWord = 1;
            } else if (ygdikasi[i]==' ') inWord = 0;
            i++;
        }

        if (chars > 0 && ygdikasi[chars - 1] == ' ') chars--;

        printInt(1);
        printString(" ");
        printInt(words);
        printString(" ");
        printInt(chars);
    }
}
```

1. Kasus `echo <argument> | wc`
```c
if (echoCheck(parted[0])&&wcCheck(parted[1])){
    echoed=parted[0]+5;
    while (*echoed == ' ') echoed++;
    iniDiaWc(echoed);
    endl();
}
```
2. Kasus `echo <argument> | grep <argument> | wc`
``` c
if(echoCheck(parted[0])&&grepCheck(parted[1])&&wcCheck(parted[2])){
    echoed=parted[0]+5;
    while (*echoed == ' ') echoed++;
    greped=parted[1]+5;
    while (*greped == ' ') greped++;
    if(iniDiaGrep(echoed,greped,grepres,0)){endl();iniDiaWc(grepres);}
    endl();
}
```

Bila hasil dari `grep` adalah NULL, maka wc tidak akan melakukan print apapun. 
```c
if(strcmp(ygdikasi,"NULL")==1){}
```

Ketika masuk kedalam sebuah kata, function akan menandai menggunakan sebuah flag bahwa string yang karakter yang diakses berada dalam sebuah word, dan variabel `word` sebagai penhitung jumlah kata akan diincrement. Hal ini berguna karena apabila bertemu blank space (spasi), akan ditandai flagnya menjadi 0 sehingga ketika bertemu sebuah karakter lagi, flag akan menjadi 1 lagi, dan jumlah word diincrement. Setiap loop yang terjadi (setiap index dalam variabel terakses), maka akan mengincrement variabel `chars`, penghitung jumlah karakter.
```c
while(ygdikasi[i]!='\0'){
    chars++;
    if(ygdikasi[i]!=' '&&inWord==0){
        words++;
        inWord = 1;
    } else if (ygdikasi[i]==' ') inWord = 0;
    i++;
}
```

Karena argumen dari `grep` pasti akan memiliki blank space diakhir, maka blank space tersebut harus dihapus dari perhitungan total jumlah karakter. 
``` c
if (chars > 0 && ygdikasi[chars - 1] == ' ') chars--;
```

Untuk melakukan print, dapat menggunakan function sebagai berikut. Karena line berjumlah 1, maka print pertama pasti 1. Dilanjutkan dengan print jumlah kata (`words`), dan jumlah karakter (`chars`).
```c
printInt(1);
printString(" ");
printInt(words);
printString(" ");
printInt(chars);
```

Karena tidak dapat melakukan print tipe data `integer` secara langsung, maka harus menggunakan fungsi `printInt()`.
```c
void printInt(int num){
    int i = 0, j, temp,a;
    char strr[100],tempStr[100];
    if (num == 0) {
        strr[i++] = '0';
        strr[i] = '\0';
        printString(strr);
        return;
    }
    while(num != 0){
        a=mod(num,10);
        tempStr[i++]=a+'0';
        num/=10;
    }
    tempStr[i]='\0';

    for(j=0;j<i;j++) strr[j]=tempStr[i - j - 1];
    strr[j]='\0';
    printString(strr);
}
```
Fungsi akan bekerja dengan cara mengambil angka satuan terlebih dahulu, mengubahnya menjadi karakter dengan menambahkan `'0'`, dan meletakkanya kedalam string. Untuk mengambil puluhan, maka variabel `num` dibagi dengan 10, kemudian proses yang sama diulang hingga habis. Karena yang diambil adalah satuan, kemudian puluhan dan seterusnya, maka hasil di `tempStr` harus dibalik dan diletakkan pada variabel baru, yakni `strr`. Baru pada akhirnya variabel `strr` akan di print, yang merupakan hasil konversid dari int ke string.


f. Buat otomatisasi untuk mengompilasi dengan melengkapi file `makefile`.
``` makefile
prepare: 
	dd if=/dev/zero of=bin/floppy.img bs=512 count=2880

bootloader: 
	nasm -f bin src/bootloader.asm -o bin/bootloader.bin

stdlib: 
	bcc -ansi -Iinclude -c src/std_lib.c -o bin/std_lib.o

kernel:
	bcc -ansi -Iinclude -c src/kernel.c -o bin/kernel.o
	nasm -f as86 src/kernel.asm -o bin/kernel-asm.o

link: 
	ld86 -o bin/kernel.bin -d bin/kernel.o bin/kernel-asm.o bin/std_lib.o

build: prepare bootloader stdlib kernel link
	dd if=bin/bootloader.bin of=bin/floppy.img bs=512 count=1 conv=notrunc
	dd if=bin/kernel.bin of=bin/floppy.img bs=512 seek=1 count=15 conv=notrunc

run: 
	bochs -f bochsrc.txt
```

Makefile berguna untuk mengotomasi compile sehingga pekerjaan dapat dilakukan dengan mudah. 


``` Makefile
prepare: 
	dd if=/dev/zero of=bin/floppy.img bs=512 count=2880
```
`prepare` berguna untuk membuat file floppy.img yang akan berguna untuk booting pada bochs.

``` Makefile
bootloader: 
	nasm -f bin src/bootloader.asm -o bin/bootloader.bin
```
`bootloader` berguna untuk mengcompile `bootloader.as` menjadi `bootloader.bin`.

``` Makefile
stdlib: 
	bcc -ansi -Iinclude -c src/std_lib.c -o bin/std_lib.o
```
`stdlib` berguna untuk mengcompile `std_lib.c` menjadi `std_lib.o`.

``` Makefile
kernel:
	bcc -ansi -Iinclude -c src/kernel.c -o bin/kernel.o
	nasm -f as86 src/kernel.asm -o bin/kernel-asm.o
```
`kernel` berguna untuk mengcompile `kernel.c` menjadi `kernel.o` dan `kernel.asm` menjadi `kernel-asm.o`.

``` Makefile
link: 
	ld86 -o bin/kernel.bin -d bin/kernel.o bin/kernel-asm.o bin/std_lib.o
```
`link` berguna untuk menggabungkan file-file kernel (`kernel-asm.o`, `kernel.o`, dan `std_lib.o`) kedalam suatu file `kernel.bin`.

``` Makefile
build: prepare bootloader stdlib kernel link
	dd if=bin/bootloader.bin of=bin/floppy.img bs=512 count=1 conv=notrunc
	dd if=bin/kernel.bin of=bin/floppy.img bs=512 seek=1 count=15 conv=notrunc
```
`build` akan memanggil semua function sebelumnya, kemudian memasukan `bootloader` dan `kernel` kedalam floppy.img. `count=1` pada `bootloader` berguna untuk menandai bahwa bootloader akan memakan sebanyak 1 block memory sebesar 512 bytes, dan `count=15` pada `kernel` akan memakan tempat sebanyak 15 block memory sebesar 512 bytes. `conv=notrunc` berguna untuk mencegah overwrite pada file yang sudah dimasukan pada floppy.img

``` Makefile
run: 
	bochs -f bochsrc.txt
```
`run` bekerja untuk menjalankan bocs dengan konfigurasi yang ada pada `bochsrc.txt`

## Kendala
![image](https://media.discordapp.net/attachments/1123949811020873759/1386347634314449066/WhatsApp_Image_2025-06-18_at_18.26.11_e619fad5.jpg?ex=68596059&is=68580ed9&hm=e2d1914d03bffcce89013e6bf9744e730bd87532d0a977f2cd2f4d6a6500588d&=&format=webp&width=2304&height=925)


Karena keterbatasan pada function dan menggunakan `bcc` sebagai compiler, terdapat beberapa kendala seperti tidak dapat menggunakan operas `%` (modulo), dan juga deklarasi variabel harus dilakukan setelah nama function (tidak dapat di tengah-tengah), dan juga tidak boleh didalam loop, sehingga dengan keterbatasan bahasa dan aturan yang cukup ketat, dibutuhkan sedikit pembiasaan untuk dapat menuliskan kode pada `kernel.c`

## Hasil 
![image](https://media.discordapp.net/attachments/1123949811020873759/1386348952844238869/D3E73E5A-C00A-4488-9049-AE4EFB939047.png?ex=68596193&is=68581013&hm=769883277ceec1122fc2ad2ea88b410cf31ab7a64682dd05888c82ec47d7a28a&=&format=webp&quality=lossless&width=2189&height=1481)

![image](https://media.discordapp.net/attachments/1123949811020873759/1386349365727334551/551DC77B-F459-411D-A171-8C4DF0D0286C.png?ex=685961f5&is=68581075&hm=b1215e03bbcb2f0952669e1efb0e1e4f4839c283d757583b1cc3ce2178c73c8a&=&format=webp&quality=lossless&width=2212&height=1481)

## Revisi
Revisi yang dilakukan yakni berfokus pada pengubahan pada cara kerja `\n` (merapihkan tampilan), dan fungsi grep.
