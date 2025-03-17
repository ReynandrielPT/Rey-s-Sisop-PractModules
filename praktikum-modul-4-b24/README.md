[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/J5rciaaA)
<div align=center>

|    NRP     |      Name      |
| :--------: | :------------: |
| 5025231091 |  Fernando |
| 5025231113 | Reynandriel Pramas Thandya |

# Praktikum Modul 4 _(Module 4 Lab Work)_

</div>

### Daftar Soal _(Task List)_

- [Task 1 - TrashOps](/task-1/) by Rey

- [Task 2 - Fu53enC0d3D==](/task-2/) by Rey

- [Task 3 - MengOS](/task-3/) by Fernando

### Laporan Resmi Praktikum Modul 4 _(Module 4 Lab Work Report)_

- [Task 1 - TrashOps](/task-1/) by Rey
#### Soal
TrashOps

Pada kebanyakan sistem operasi Linux, perintah rm akan menghapus index yang mengarah pada entri file. Sehingga akan sulit untuk memulihkan file tersebut, atau dapat dikatakan telah terhapus permanen. Menariknya, anda akan membuat filesystem yang memodifikasi proses penghapusan dan pemulihan file dan direktori. Sebelum itu, silakan gunakan resource berikut sebagai direktori target: [link](https://drive.google.com/file/d/1WuFRDFHW3yYJpZkbgkON70P-9gFkicIR/view).

- a. Kode untuk membuat filesystem diletakkan pada [trash.c](./trash.c). Anda harus menggunakan fungsi getattr, readdir, read, unlink, rmdir, rename, mkdir, chown, dan chmod pada struct fuse_operations. Dibebaskan jika ingin menambahkan fungsi lain selain 9 fungsi diatas.

- b. Ketika menggunakan perintah rm atau rmdir untuk file atau direktori yang berada diluar direktori trash, maka file atau direktori tersebut akan dipindahkan ke direktori trash dan menjadi tidak dapat diread, write, dan execute baik oleh pemilik, grup, maupun user lainnya.

  ![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/18bd1c11-3eb0-47cc-9c95-e2f8096282a9)

  Barulah ketika perintah rm atau rmdir digunakan untuk file atau direktori yang berada di dalam direktori trash, maka file atau direktori tersebut akan dihapus permanen.

  ![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/78c4175a-e064-407f-a644-d326bba02daa)

- c. File atau direktori yang berada pada direktori trash tidak dapat diubah permission dan kepemilikannya, serta tidak dapat direname.

  ![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/1e0460cc-b490-4f5a-adb0-476ef78ea578)

- d. Untuk memulihkan file atau direktori dari direktori trash, anda harus menggunakan perintah mv dengan format mv [path_file_dalam_trash] [arg]. Opsi pertama untuk arg adalah dengan path biasa sehingga file atau direktori akan dipindahkan dari direktori trash ke path tersebut. Opsi kedua untuk arg adalah restore sehingga file atau direktori akan kembali ke path asal sebelum ia dipindah ke trash. Permission untuk file atau direktori yang dipulihkan harus kembali seperti sebelum dimasukkan ke trash. Khusus untuk arg restore anda harus membuat path yang sesuai apabila path asal tidak ada (terhapus/dipindah).

  ![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/3a052576-806e-4c7a-bc23-821959e4e1a9)

- e. Direktori trash tidak dapat dihapus, dipindah, direname. Anda juga tidak dapat membuat direktori dengan nama trash atau restore.

- f. Catatlah log pada file trash.log. Format untuk tiap baris log adalah YYMMDD-HH:MM:SS KETERANGAN. Format keterangan akan berdasarkan aksi sebagai berikut:

| Aksi                                                                                                              | Keterangan                                           |
| ----------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------- |
| Memindahkan file atau direktori ke direktori trash                                                                | `MOVED [path_awal] TO TRASH`                         |
| Menghapus file atau direktori secara permanen                                                                     | `PERMANENTLY REMOVED [path_di_trash]`                |
| Memulihkan file atau direktori dari direktori trash                                                               | `RESTORED [path_di_trash] FROM TRASH TO [path_baru]` |
| Percobaan mengubah permission, mengubah kepemilikan, atau merename file atau direktori yang berada di dalam trash | `FAILED TO [CHMOD/CHOWN/RENAME][path_di_trash]`      |
| Percobaan menghapus, memindah, merename direktori trash                                                           | `FAILED TO [REMOVE/MOVE/RENAME] TRASH DIRECTORY`     |
| Percobaan membuat direktori dengan nama 'trash' atau 'restore'                                                    | `FAILED TO CREATE [TRASH/RESTORE] DIRECTORY`         |

![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/bfedad43-1a15-46a1-9c5b-09aad7cb06e1)

Catatan:

- Direktori trash sudah terdapat pada resource yang akan anda gunakan.
- Anggap saja tidak ada kasus ketika menghapus file atau direktori dengan nama yang sama kedalam trash.
- Dilarang menggunakan fungsi system().


##### Jawaban
Full Code:
```
#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>
#include <stdarg.h>
#include <sys/xattr.h> 
#include <errno.h>     

static const char *dirpath = "/home/rey/praktikum-modul-4-b24/task-1/Storage";
static const char *trash_path = "/home/rey/praktikum-modul-4-b24/task-1/Storage/trash";

static void fullpath(char fpath[PATH_MAX], const char *path) {
    snprintf(fpath, PATH_MAX, "%s%s", dirpath, path);
}

static void current_time_string(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y%m%d-%H:%M:%S", t);
}


static FILE *log_file;


static void write_log(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
    fflush(log_file);
}

static void log_action(const char *description) {
    char timestamp[20];
    current_time_string(timestamp, sizeof(timestamp));
    write_log("%s %s\n", timestamp, description);
}


static int is_in_trash(const char *path) {
    return strncmp(path, "/trash", 6) == 0;
}

static int set_permissions(const char *path, mode_t mode) {
    if (chmod(path, mode) == -1)
        return -errno;
    return 0;
}

static int save_original_permissions(const char *path) {
      struct stat st;
    if (stat(path, &st) == -1)
        return -errno;
    
    mode_t original_mode = st.st_mode & 0777;
     if (chmod(path, 0600) == -1) {
        return -errno;
    }
    
    char attr_name[] = "user.orig_perms";
    if (setxattr(path, attr_name, &st.st_mode, sizeof(st.st_mode), 0) == -1)
        return -errno;
 

    if (chmod(path, original_mode) == -1) {
        return -errno;
    }

    return 0;
}

static int get_original_permissions(const char *path, mode_t *mode) {
    char attr_name[] = "user.orig_perms";
    ssize_t size = getxattr(path, attr_name, mode, sizeof(mode_t));
    if (size == -1)
        return -errno;
    return 0;
}

static int set_original_permissions(const char *path) {
    mode_t mode;
    int res = get_original_permissions(path, &mode);
    if (res != 0)
        return res;
    return chmod(path, mode);
}

static int save_original_location(const char *path, const char *original_location) {
    char orioriori[PATH_MAX];
    memset(orioriori, 0, PATH_MAX);
    strncpy(orioriori, original_location, PATH_MAX - 1); 
    orioriori[PATH_MAX - 1] = '\0'; 


    struct stat st;
    if (stat(path, &st) == -1)
        return -errno;

    mode_t original_mode = st.st_mode & 0777;
    if (chmod(path, 0600) == -1) {
        return -errno;
    }
    
    char attr_name[] = "user.orig_location";
    if (removexattr(path, attr_name) == -1 && errno != ENODATA) {
        return -errno;
    }

    if (setxattr(path, attr_name, orioriori, strlen(orioriori) + 1, 0) == -1)
        return -errno;

    if (chmod(path, original_mode) == -1) {
        return -errno;
    }
   
    return 0;
}



static int get_original_location(const char *path, char *original_location, size_t size) {
    char attr_name[] = "user.orig_location";
    ssize_t len = getxattr(path, attr_name, original_location, size);
    if (len == -1)
        return -errno;
    return 0;
}

static int move_from_trash(const char *src, const char *dest) {
    char src_path[PATH_MAX], dest_path[PATH_MAX];
    char desc[PATH_MAX];
    fullpath(dest_path, dest);
    fullpath(src_path, src);
        if (set_permissions(src_path, 0777) != 0)
        return -errno;

    char *dir_path = strdup(dest_path);
    char *last_slash = strrchr(dir_path, '/');
    if (last_slash != NULL) {
        *last_slash = '\0';
        mkdir(dir_path, 0755);
    }
    free(dir_path);

        if (rename(src_path, dest_path) == -1)
            return -errno;
    

    if (set_original_permissions(dest_path) != 0)
        return -errno;
    
      snprintf(desc, PATH_MAX, "RESTORED %s FROM TRASH TO %s", src_path, dest_path);
        log_action(desc);

    return 0;
}

static int restore_from_trash(const char *path) {
    char trash_fpath[PATH_MAX], fullori[PATH_MAX];
    char original_location[PATH_MAX];
    char desc[PATH_MAX];
     size_t size = sizeof(original_location);
    fullpath(trash_fpath, path);
     if (set_permissions(trash_fpath, 0777) != 0)
        return -errno;
   
    
    get_original_location(trash_fpath, original_location, size);
    fullpath(fullori, original_location);
    if (rename(trash_fpath, fullori) == -1) {
        return -errno;
    }

    if (set_original_permissions(fullori) != 0)
    return -errno;

     snprintf(desc, PATH_MAX, "RESTORED %s FROM TRASH TO %s", trash_fpath, fullori);
        log_action(desc);

    return 0;
}

static int trash_getattr(const char *path, struct stat *stbuf) {
    int res;
    char fpath[PATH_MAX];
    fullpath(fpath, path);


    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}

static int trash_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    char fpath[PATH_MAX];
    fullpath(fpath, path);

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
            continue;
        }

        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}

static int trash_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    int fd;
    int res;
    char fpath[PATH_MAX];
    fullpath(fpath, path);

    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}


static int trash_unlink(const char *path) {
    char fpath[PATH_MAX], trash_fpath[PATH_MAX];
    char original[PATH_MAX];
    char desc[PATH_MAX];
    fullpath(fpath, path);

    if (is_in_trash(path)) {
        if (unlink(fpath) == -1)
            return -errno;
        snprintf(desc, PATH_MAX, "PERMANENTLY REMOVED %s", fpath);
        log_action(desc);
        return 0;
    }

    snprintf(trash_fpath, PATH_MAX, "%s", trash_path);
    mkdir(trash_fpath, 0755);

    snprintf(trash_fpath, PATH_MAX, "%s/%s", trash_path, strrchr(path, '/') + 1);

    

      if (save_original_permissions(fpath) != 0)
       return -errno;
    
    if (save_original_location(fpath, path) != 0)
        return -errno;
    if(get_original_location(fpath, original, sizeof(original))!=0){
        return -errno;
    }

  
    if (set_permissions(fpath, 0777) != 0)
        return -errno;
    
    if (rename(fpath, trash_fpath) == -1)
        return -errno;
    snprintf(desc, PATH_MAX, "MOVED %s TO TRASH", fpath);
    log_action(desc);
    
    

    if (set_permissions(trash_fpath, 0000) != 0)
        return -errno;

    return 0;
}

static int remove_directory_recursive(const char *path) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL)
        return -errno;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[PATH_MAX];
        snprintf(full_path, PATH_MAX, "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (remove_directory_recursive(full_path) != 0)
                return -errno;
        } else {
            if (unlink(full_path) == -1)
                return -errno;
        }
    }

    closedir(dir);

    if (rmdir(path) == -1)
        return -errno;

    return 0;
}

static int trash_rmdir(const char *path) {
    char fpath[PATH_MAX], trash_fpath[PATH_MAX];
    char desc[PATH_MAX];
    fullpath(fpath, path);
     if(strcmp(path, "/trash")==0){
        snprintf(desc, PATH_MAX, "FAILED TO REMOVE TRASH DIRECTORY");
            log_action(desc);
        return -EPERM;
    }
    if (is_in_trash(path)) {
        if (set_permissions(fpath, 0777) != 0)
            return -errno;

        if (remove_directory_recursive(fpath) != 0)
            return -errno;
         snprintf(desc, PATH_MAX, "PERMANENTLY REMOVED %s", fpath);
        log_action(desc);
        return 0;
    }

     snprintf(trash_fpath, PATH_MAX, "%s", trash_path);
    mkdir(trash_fpath, 0755);

    snprintf(trash_fpath, PATH_MAX, "%s/%s", trash_path, strrchr(path, '/') + 1);
 
     if (save_original_permissions(fpath) != 0)
       return -errno;
    if (save_original_location(fpath, path) != 0)
        return -errno;
     if (set_permissions(fpath, 0777) != 0)
        return -errno;

    if (rename(fpath, trash_fpath) == -1)
        return -errno;
 
    if (set_permissions(trash_fpath, 0000) != 0)
        return -errno;
    
     snprintf(desc, PATH_MAX, "MOVED %s TO TRASH", fpath);
    log_action(desc);
   
    return 0;
}


static int trash_rename(const char *from, const char *to) {
    char from_fpath[PATH_MAX], to_fpath[PATH_MAX];
    char desc [PATH_MAX];
    fullpath(from_fpath, from);
    if(strcmp(from, "/trash")==0){
            snprintf(desc, PATH_MAX, "FAILED TO RENAME/MOVE TRASH DIRECTORY");
        log_action(desc);
        return -EPERM;
        }
        
    if (is_in_trash(from)) {
        if (strcmp(to, "/trash/restore") == 0) {
            return restore_from_trash(from);
        } else if (strncmp(to, "/trash/", 7) == 0) {
             snprintf(desc, PATH_MAX, "FAILED TO RENAME %s", from_fpath);
            log_action(desc);
            return -EPERM;
        } else {
            return move_from_trash(from, to);
        }
    }
    fullpath(to_fpath, to);
    if (rename(from_fpath, to_fpath) == -1) {
        return -errno;
    }

    return 0;
}

static int trash_mkdir(const char *path, mode_t mode) {
    char fpath[PATH_MAX];
    char desc[PATH_MAX];
    fullpath(fpath, path);

     if (strstr(fpath, "/trash") != NULL){
       
        snprintf(desc, PATH_MAX, "FAILED TO CREATE TRASH DIRECTORY");
        log_action(desc);
        return -EPERM;
       
    }
    if (strstr(fpath, "restore") != NULL){
         snprintf(desc, PATH_MAX, "FAILED TO CREATE RESTORE DIRECTORY");
        log_action(desc);
        return -EPERM;
    }
     if (mkdir(fpath, mode) == -1) {
       
        if (errno == EEXIST && strstr(fpath, "/trash") != NULL ) {
            snprintf(desc, PATH_MAX, "FAILED TO CREATE TRASH DIRECTORY");
            log_action(desc);
        }
       return -errno;
     }

    return 0;
}

static int trash_chown(const char *path, uid_t uid, gid_t gid) {
    char fpath[PATH_MAX];
    char desc[PATH_MAX];
    fullpath(fpath, path);
    if (is_in_trash(path)) {
        snprintf(desc, PATH_MAX, "FAILED TO CHOWN %s", fpath);
            log_action(desc);
        return -EPERM;
    }
  
    if (lchown(fpath, uid, gid) == -1)
        return -errno;
    return 0;
}

static int trash_chmod(const char *path, mode_t mode) {
     char fpath[PATH_MAX];
     char desc[PATH_MAX];
    fullpath(fpath, path);
    if (is_in_trash(path)) {
        snprintf(desc, PATH_MAX, "FAILED TO CHMOD %s", fpath);
            log_action(desc);
        return -EPERM;
    }
   
    if (chmod(fpath, mode) == -1)
        return -errno;
    return 0;
}

static struct fuse_operations trash_oper = {
    .getattr = trash_getattr,
    .readdir = trash_readdir,
    .read = trash_read,
    .unlink = trash_unlink,
    .rmdir = trash_rmdir,
    .rename = trash_rename,
    .mkdir = trash_mkdir,
    .chown = trash_chown,
    .chmod = trash_chmod,
};

int main(int argc, char *argv[]) {
     log_file = fopen("/home/rey/praktikum-modul-4-b24/task-1/Storage/trash.log", "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        return -1;
    }
    return fuse_main(argc, argv, &trash_oper, NULL);
}

```
Penjelasan full code:
```
#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>
#include <stdarg.h>
#include <sys/xattr.h> 
#include <errno.h>  
```
Blok kode diatas berfungsi untuk mengimpor seluruh library yang diperlukan dan mendefinisikan versi FUSE yang digunakan yakni 30

```
static const char *dirpath = "/home/rey/praktikum-modul-4-b24/task-1/Storage";
static const char *trash_path = "/home/rey/praktikum-modul-4-b24/task-1/Storage/trash";
```
Supaya mempermudah kedepannya, dibuatlah suatu string untuk menyimpan alamat base directory dan alamat dari folder trash

```
static void fullpath(char fpath[PATH_MAX], const char *path) {
    snprintf(fpath, PATH_MAX, "%s%s", dirpath, path);
}

```
Merupakan fungsi fullpath yang digunakan untuk membuat alamat lengkap dari suatu direktori (misalkan folder test, maka akan dibuat alamat lengkap dari home menuju folder test tersebut). Pembuatan alamat dilakukan dengan menggunakan snprintf untuk menggabungkan string. 

```
static void current_time_string(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y%m%d-%H:%M:%S", t);
}

```
Blok kode tersebut merupakan fungsi untuk mengambil waktu sekarang dan dijadikan suatu string dengan format "Tahunbulamhari-jam:menit:detik"

```
static FILE *log_file;
```
Sebuah line code untuk mendefinisikan file untuk logging

```
static void write_log(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
    fflush(log_file);
}

static void log_action(const char *description) {
    char timestamp[20];
    current_time_string(timestamp, sizeof(timestamp));
    write_log("%s %s\n", timestamp, description);
}

```
Dua blok kode tersebut merupakan blok kode untuk menulis pesan log ke dalam file logging. Fungsi pertama untuk menulis ke dalam log file, fungsi kedua untuk menuliskan pesan sekaligus waktu dari log tersebut. Waktu diambil menggunakan fungsi 'current_time_string'.

```

static int is_in_trash(const char *path) {
    return strncmp(path, "/trash", 6) == 0;
}
```
Merupakan fungsi untuk mengecek apakah suatu file berada dalam folder trash atau tidak.

```
static int set_permissions(const char *path, mode_t mode) {
    if (chmod(path, mode) == -1)
        return -errno;
    return 0;
}
```
Terkadang suatu folder atau file perlu diubah permissionnya, maka dibuatlah fungsi tersebut untuk mengubah permission suatu file ataupun folder.

```
static int save_original_permissions(const char *path) {
      struct stat st;
    if (stat(path, &st) == -1)
        return -errno;
    
    mode_t original_mode = st.st_mode & 0777;
     if (chmod(path, 0600) == -1) {
        return -errno;
    }
    
    char attr_name[] = "user.orig_perms";
    if (setxattr(path, attr_name, &st.st_mode, sizeof(st.st_mode), 0) == -1)
        return -errno;
 

    if (chmod(path, original_mode) == -1) {
        return -errno;
    }

    return 0;
}

static int get_original_permissions(const char *path, mode_t *mode) {
    char attr_name[] = "user.orig_perms";
    ssize_t size = getxattr(path, attr_name, mode, sizeof(mode_t));
    if (size == -1)
        return -errno;
    return 0;
}

static int set_original_permissions(const char *path) {
    mode_t mode;
    int res = get_original_permissions(path, &mode);
    if (res != 0)
        return res;
    return chmod(path, mode);
}
```
Misalkan terdapat suatu kasus dimana file.txt dihapus dari folder test. Ketika dihapus dengan command 'rm' maka sebetulnya file.txt akan dipindahkan ke folder trash. Selain dipindahkan, file.txt juga harus diubah permissionnya menjadi '0000' (sesuai permintaan soal), maka digunakanlah fungsi sebelumnya (set_permission). Ketika file.txt di trash, maka file.txt masih bisa di restore dengan cara 'mv file.txt restore' (bisa juga dipindahkan ke folder lain). Ketika dipindahkan keluar dari trash, file.txt perlu dikembalikan permission sebelumnya. Maka pada code ini diperlukan beberapa fungsi untuk menyimpan, mengambil, dan memberikan permission awal suatu file. Untuk menyimpan permission suatu file, maka disini dibuat fungsi yang dimana akan mencantumkan original permission ke file tersebut dengan suatu attribut dengan nama "user.orig_perms". Begitu juga ketika ingin diset ke semula, maka dicarilah attribut tersebut (dengan fungsi get_original_permissions) dan dipasangkan permissionnya ke file tersebut (dengan fungsi set_original_permissions). Untuk penyimpanan attribut, perlu dilakukan sebelum file tersebut dihapus (dipindah ke trash). 

```
static int save_original_location(const char *path, const char *original_location) {
    char orioriori[PATH_MAX];
    memset(orioriori, 0, PATH_MAX);
    strncpy(orioriori, original_location, PATH_MAX - 1); 
    orioriori[PATH_MAX - 1] = '\0'; 


    struct stat st;
    if (stat(path, &st) == -1)
        return -errno;

    mode_t original_mode = st.st_mode & 0777;
    if (chmod(path, 0600) == -1) {
        return -errno;
    }
    
    char attr_name[] = "user.orig_location";
    if (removexattr(path, attr_name) == -1 && errno != ENODATA) {
        return -errno;
    }

    if (setxattr(path, attr_name, orioriori, strlen(orioriori) + 1, 0) == -1)
        return -errno;

    if (chmod(path, original_mode) == -1) {
        return -errno;
    }
   
    return 0;
}



static int get_original_location(const char *path, char *original_location, size_t size) {
    char attr_name[] = "user.orig_location";
    ssize_t len = getxattr(path, attr_name, original_location, size);
    if (len == -1)
        return -errno;
    return 0;
}

```
Suatu file harus dapat dikembalikan ke lokasi direktori semula. Dengan begitu diperlukan fungsi-fungsi yang dapat menyimpan dan mengambil lokasi semula tersebut. Lokasi semula suatu file dapat disimpan dengan menambahkan attribut (seperti pada fungsi save_original_location) bernama "user.orig_location" dengan value lokasi file sekarang. Kemudian untuk mengetahuinya dapat menggunakan fungsi get_original_location untuk mengambil attribut "user.orig_location" dari suatu file, dan lokasi semula tersebut dipass ke pemanggil fungsi. Menyimpan suatu lokasi file juga harus dilakukan sebelum penghapusan file. Untuk pengambilan lokasi semula, diperlukan ketika perintah 'mv file restore' dijalankan, berarti fungsi 'get_original_location' akan dipanggil pada fungsi 'restore_from_trash' ('mv' akan masuk ke fungsi 'rename' dan ketika restore akan masuk ke 'restore_from_trash').

```
static int move_from_trash(const char *src, const char *dest) {
    char src_path[PATH_MAX], dest_path[PATH_MAX];
    char desc[PATH_MAX];
    fullpath(dest_path, dest);
    fullpath(src_path, src);
        if (set_permissions(src_path, 0777) != 0)
        return -errno;

    char *dir_path = strdup(dest_path);
    char *last_slash = strrchr(dir_path, '/');
    if (last_slash != NULL) {
        *last_slash = '\0';
        mkdir(dir_path, 0755);
    }
    free(dir_path);

        if (rename(src_path, dest_path) == -1)
            return -errno;
    

    if (set_original_permissions(dest_path) != 0)
        return -errno;
    
      snprintf(desc, PATH_MAX, "RESTORED %s FROM TRASH TO %s", src_path, dest_path);
        log_action(desc);

    return 0;
}

static int restore_from_trash(const char *path) {
    char trash_fpath[PATH_MAX], fullori[PATH_MAX];
    char original_location[PATH_MAX];
    char desc[PATH_MAX];
     size_t size = sizeof(original_location);
    fullpath(trash_fpath, path);
     if (set_permissions(trash_fpath, 0777) != 0)
        return -errno;
   
    
    get_original_location(trash_fpath, original_location, size);
    fullpath(fullori, original_location);
    if (rename(trash_fpath, fullori) == -1) {
        return -errno;
    }

    if (set_original_permissions(fullori) != 0)
    return -errno;

     snprintf(desc, PATH_MAX, "RESTORED %s FROM TRASH TO %s", trash_fpath, fullori);
        log_action(desc);

    return 0;
}

```
Seperti penjelasan sebelumnya, suatu file yang di 'rm' diluar folder trash, akan dipindahkan ke folder trash dan diberikan permissionnya '0000'. Lalu file tersebut masih bisa dikembalikan dengan cara 'mv file restore' yang akan memindahkan filenya ke lokasi awalnya, ataupun dengan 'mv file ../folderlain' yang akan memindahkan file tersebut ke folder yang dituju. Maka dibuatlah dua fungsi tersebut untuk memenuhi kebutuhan tersebut. Fungsi-fungsi tersebut akan dipanggil oleh fungsi lainnya yakni fungsi 'rename'. Kedua fungsi tersebut akan melakukan logging dengan memanggil fungsi "log_action" dengan pesan ""RESTORED {lokasi file di trash} FROM TRASH TO {lokasi pemindahan/restorasi}" setelah melakukan pemindahan/restorasi. 

```

static int trash_getattr(const char *path, struct stat *stbuf) {
    int res;
    char fpath[PATH_MAX];
    fullpath(fpath, path);


    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}
```
Fungsi tersebut merupakan fungsi untuk mengambil attribut suatu file.

```
static int trash_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    char fpath[PATH_MAX];
    fullpath(fpath, path);

    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL) {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
            continue;
        }

        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}
```
Serangakaian kode tersebut merupakan fungsi untuk membaca isi dari suatu direktori.

```
static int trash_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    int fd;
    int res;
    char fpath[PATH_MAX];
    fullpath(fpath, path);

    fd = open(fpath, O_RDONLY);
    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);
    return res;
}

```
Kode tersebut merupakan fungsi untuk membaca suatu file.

```
static int trash_unlink(const char *path) {
    char fpath[PATH_MAX], trash_fpath[PATH_MAX];
    char original[PATH_MAX];
    char desc[PATH_MAX];
    fullpath(fpath, path);

    if (is_in_trash(path)) {
        if (unlink(fpath) == -1)
            return -errno;
        snprintf(desc, PATH_MAX, "PERMANENTLY REMOVED %s", fpath);
        log_action(desc);
        return 0;
    }

    snprintf(trash_fpath, PATH_MAX, "%s", trash_path);
    mkdir(trash_fpath, 0755);

    snprintf(trash_fpath, PATH_MAX, "%s/%s", trash_path, strrchr(path, '/') + 1);

    

      if (save_original_permissions(fpath) != 0)
       return -errno;
    
    if (save_original_location(fpath, path) != 0)
        return -errno;
    if(get_original_location(fpath, original, sizeof(original))!=0){
        return -errno;
    }

  
    if (set_permissions(fpath, 0777) != 0)
        return -errno;
    
    if (rename(fpath, trash_fpath) == -1)
        return -errno;
    snprintf(desc, PATH_MAX, "MOVED %s TO TRASH", fpath);
    log_action(desc);
    
    

    if (set_permissions(trash_fpath, 0000) != 0)
        return -errno;

    return 0;
}
```
Fungsi tersebut merupakan fungsi untuk menghapus suatu file. Seperti yang sudah dijelaskan sebelumnya, apabila file berada di trash maka akan dihapus secara permanent (unlink). Setelah penghapusan permanen dilakukan maka fungsi akan memanggil "log_action" untuk logging dan memberikan pesan dari string 'desc' dengan nilai "PERMANENTLY REMOVED %s" dengan '%s' merupakan lokasi file tersebut. Apabila diluar trash maka akan dipindahkan ke direktori trash. Akan tetapi sebelum memindahkan file, fungsi tersebut akan menyimpan lokasi awal dan permission awal file tersebut. Setelah dipindahkan, file dalam trash akan di set permissionnya menjadi tidak bisa di execute, dibaca, dan diread (0000). Kemudian fungsi tersebut juga akan memanggil "log_action" untuk logging dengan string 'desc' sebagai pesan log yang berisi ""MOVED %s TO TRASH", dengan '%s' adalah lokasi dari file tersebut.

```
static int remove_directory_recursive(const char *path) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(path)) == NULL)
        return -errno;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[PATH_MAX];
        snprintf(full_path, PATH_MAX, "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) {
            if (remove_directory_recursive(full_path) != 0)
                return -errno;
        } else {
            if (unlink(full_path) == -1)
                return -errno;
        }
    }

    closedir(dir);

    if (rmdir(path) == -1)
        return -errno;

    return 0;
}

static int trash_rmdir(const char *path) {
    char fpath[PATH_MAX], trash_fpath[PATH_MAX];
    char desc[PATH_MAX];
    fullpath(fpath, path);
     if(strcmp(path, "/trash")==0){
        snprintf(desc, PATH_MAX, "FAILED TO REMOVE TRASH DIRECTORY");
            log_action(desc);
        return -EPERM;
    }
    if (is_in_trash(path)) {
        if (set_permissions(fpath, 0777) != 0)
            return -errno;

        if (remove_directory_recursive(fpath) != 0)
            return -errno;
         snprintf(desc, PATH_MAX, "PERMANENTLY REMOVED %s", fpath);
        log_action(desc);
        return 0;
    }

     snprintf(trash_fpath, PATH_MAX, "%s", trash_path);
    mkdir(trash_fpath, 0755);

    snprintf(trash_fpath, PATH_MAX, "%s/%s", trash_path, strrchr(path, '/') + 1);
 
     if (save_original_permissions(fpath) != 0)
       return -errno;
    if (save_original_location(fpath, path) != 0)
        return -errno;
     if (set_permissions(fpath, 0777) != 0)
        return -errno;

    if (rename(fpath, trash_fpath) == -1)
        return -errno;
 
    if (set_permissions(trash_fpath, 0000) != 0)
        return -errno;
    
     snprintf(desc, PATH_MAX, "MOVED %s TO TRASH", fpath);
    log_action(desc);
   
    return 0;
}
```
Sama seperti sebelumnya, dua fungsi tersebut berguna untuk menghapus hanya saja kali ini menghapus suatu direktori. Terdapat dua fungsi karena apabila folder memiliki isi, perlu suatu fungsi lain untuk menghapus pula isi-isi dari direktori tersebut. Maka dibuatlah fungsi 'remove_directory_recursive' untuk menghapus isi direktori/folder secara rekursif. Seperti sebelumnya, apabila direktori berada diluar trash, maka direktori tersebut akan dipindahkan ke trash dan diset permissionnya menjadi '0000'. Fungsi juga akan memanggil "log_action" dan logging dengan pesan ""MOVED %s TO TRASH", dengan '%s' adalah lokasi dari direktori tersebut. Jika sudah berada di trash maka direktori akan dihapus beserta isi-isinya, dan fungsi akan memanggil "log_action" untuk melakukan logging dengan pesan "PERMANENTLY REMOVED " dengan diikuti alamat dari direktori tersebut. Terdapat pula pengondisian apabila folder yang ingin dicoba untuk dihapus adalah folder "trash" maka perintah tidak bisa dijalankan dan fungsi akan memanggil "log_action" dan logging dengan pesan "FAILED TO REMOVE TRASH DIRECTORY". 

```
static int trash_rename(const char *from, const char *to) {
    char from_fpath[PATH_MAX], to_fpath[PATH_MAX];
    char desc [PATH_MAX];
    fullpath(from_fpath, from);
    if(strcmp(from, "/trash")==0){
            snprintf(desc, PATH_MAX, "FAILED TO RENAME/MOVE TRASH DIRECTORY");
        log_action(desc);
        return -EPERM;
        }
        
    if (is_in_trash(from)) {
        if (strcmp(to, "/trash/restore") == 0) {
            return restore_from_trash(from);
        } else if (strncmp(to, "/trash/", 7) == 0) {
             snprintf(desc, PATH_MAX, "FAILED TO RENAME %s", from_fpath);
            log_action(desc);
            return -EPERM;
        } else {
            return move_from_trash(from, to);
        }
    }
    fullpath(to_fpath, to);
    if (rename(from_fpath, to_fpath) == -1) {
        return -errno;
    }

    return 0;
}
```
Blok kode tersebut merupakan fungsi untuk mengubah nama dan mengubah lokasi (memindahkan) suatu file. Jika folder trash berusaha untuk dipindahkan (atau diganti namanya) dengan 'mv' maka perintah tidak bisa dijalankan dan fungsi akan memanggil fungsi "log_action" untuk logging dengan pesan "FAILED TO RENAME/MOVE TRASH DIRECTORY". Apabila file atau direktori berada dalam direktori trash maka terdapat 3 kondisi. Jika terdapat argumen "restore" maka panggil fungsi 'restore_from_trash' untuk me-"restore" file tersebut ke lokasi semula. Jika ternyata argumen tambahan berada dalam folder trash juga maka perintah tidak bisa dijalankan (contoh 'mv dummy.pdf dum.pdf' maka 'dum.pdf' sebetulnya '/trash/dum.pdf' yang berarti berada dalam folder trash). Selain pengondisian tersebut maka file atau direktori akan dipindahkan ke lokasi sesuai argumen tambahan tersebut.  Jika file atau direktori berada dalam folder trash, fungsi akan memanggil fungsi "log_action" dengan memberikan string 'desc' dengan value "FAILED TO RENAME " dan diikuti dengan lokasi dari file atau direktori tersebut.
```
static int trash_mkdir(const char *path, mode_t mode) {
    char fpath[PATH_MAX];
    char desc[PATH_MAX];
    fullpath(fpath, path);

     if (strstr(fpath, "/trash") != NULL){
       
        snprintf(desc, PATH_MAX, "FAILED TO CREATE TRASH DIRECTORY");
        log_action(desc);
        return -EPERM;
       
    }
    if (strstr(fpath, "restore") != NULL){
         snprintf(desc, PATH_MAX, "FAILED TO CREATE RESTORE DIRECTORY");
        log_action(desc);
        return -EPERM;
    }
     if (mkdir(fpath, mode) == -1) {
       
        if (errno == EEXIST && strstr(fpath, "/trash") != NULL ) {
            snprintf(desc, PATH_MAX, "FAILED TO CREATE TRASH DIRECTORY");
            log_action(desc);
        }
       return -errno;
     }

    return 0;
}
```
Merupakan fungsi untuk membuat suatu direktori. Pada fungsi tersebut terdapat dua pengondisian yang dimana jika nama direktori yang dibuat adalah "restore" ataupun "trasH" maka perintah tidak dapat dijalankan, selain itu suatu direktori dapat dibuat dengan fungsi tersebut. Jika nama direktori yang dibuat adalah "restore", fungsi juga akan memanggil fungsi "log_action" dengan memberikan string 'desc' dengan value "FAILED TO CREATE RESTORE DIRECTORY", begitu juga dengan "trash" dengan pesan log "FAILED TO CREATE TRASH DIRECTORY".

```
static int trash_chown(const char *path, uid_t uid, gid_t gid) {
    char fpath[PATH_MAX];
    char desc[PATH_MAX];
    fullpath(fpath, path);
    if (is_in_trash(path)) {
        snprintf(desc, PATH_MAX, "FAILED TO CHOWN %s", fpath);
            log_action(desc);
        return -EPERM;
    }
  
    if (lchown(fpath, uid, gid) == -1)
        return -errno;
    return 0;
}
```
Bagian tersebut adalah fungsi untuk menjalankan perintah 'chown' untuk mengubah kepemilikan dari suatu file ataupun direktori. Terdapat pengondisian apabila file atau direktori berada dalam folder trazh maka perintah tidak dapat dijalankan, selain itu perintah dapat dijalankan dengan normal. Apabila berada dalam trash, fungsi juga akan memanggil fungsi "log_action" dengan memberikan string 'desc' dengan value "FAILED TO CHOWN " diikuti dengan alamat lokasi file trsebut, sehingga pencatatan (logging) dapat dilakukan.

```
static int trash_chmod(const char *path, mode_t mode) {
     char fpath[PATH_MAX];
     char desc[PATH_MAX];
    fullpath(fpath, path);
    if (is_in_trash(path)) {
        snprintf(desc, PATH_MAX, "FAILED TO CHMOD %s", fpath);
            log_action(desc);
        return -EPERM;
    }
   
    if (chmod(fpath, mode) == -1)
        return -errno;
    return 0;
}
```
Bagian tersebut adalah fungsi untuk menjalankan perintah 'chmod' untuk mengubah permission dari suatu file atau direktori. Perintah dapat dijalankan dengan normal kecuali jika file atau direktori berada dalam folder trash. Apabila file atau direktori berada dalam folder trash maka perintah tidak bisa dijalankan. Fungsi juga akan melakukan logging ketika file atau direktori yang ingin di ubah permissionnya berada di dalam folder trash. Logging dilakukan dengan memanggil fungsi "log_action" dan memberikan string 'desc' sebagai pesan log yang berisi "FAILED TO CHMOD " yang diikuti dengan alamat lokasi file atau direktori tersebut.

```
static struct fuse_operations trash_oper = {
    .getattr = trash_getattr,
    .readdir = trash_readdir,
    .read = trash_read,
    .unlink = trash_unlink,
    .rmdir = trash_rmdir,
    .rename = trash_rename,
    .mkdir = trash_mkdir,
    .chown = trash_chown,
    .chmod = trash_chmod,
};
```
Merupakan blok kode untuk "memanggil" (menghubungkan) fungsi-fungsi yang sudah dibuat sebelumnya ke suatu struct "fuse_operations". Sehingga ketika program dijalankan, seluruh perintah akan dijalankan sesuai fungsi-fungsi tersebut (memodifikasi perintah default menjadi sesuai fungsi).

```

int main(int argc, char *argv[]) {
     log_file = fopen("/home/rey/praktikum-modul-4-b24/task-1/Storage/trash.log", "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        return -1;
    }
    return fuse_main(argc, argv, &trash_oper, NULL);
}
```
Merupakan fungsi utama dari program. Mulanya terdapat pemberian value kepada "log_file" yang merupakan hasil dari operasi "fopen" (membuka) suatu lokasi yang menuju ke "trash.log". Fungsi 'fopen' dilakukan dengan 'a' untuk 'append' sehingga setiap pesan log yang masuk tidak menjadi __overwrite_. Kemudian perintah utama akan mengembalikan ke "fuse_main" seluruh argumen dan perintah yang dimasukkan.


Bukti Screenshot:
![1](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/66d6335b-2d08-403d-a699-4171ae9859d7)
![2](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/260479e2-b116-46be-b334-a02d74f43a62)
![3](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/adca9084-94f4-406d-b4ff-0838b19f0c45)
![4,5](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/98632cca-10f4-47e6-b2b9-6b814b424d2d)
![4](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/47ac1542-fa88-43e5-aab2-c1f605577738)


Kendala yang dialami:
- Ketidaktahuan saya bahwa mount point dengan base directory tidak bisa sama sehingga banyak waktu yang terbuang hanya untuk mencoba memount trash.c pada base directorynya.
- Pada soal terdapat permintaan untuk logging ketika perintah 'mv' dilakukan terhadap folder trash. Format loggingnya adalah (pada soal) "FAILED TO REMOVE/MOVE/RENAME TRASH DIRECTORY". Hal ini membuat saya bingung karena saya pikir log untuk MOVE dan RENAME harus berbeda (jadi harus buat pengondisian). Akan tetapi setelah saya coba-coba lagi bahkan sampai mengubah algoritmanya, tetap saja tidak bisa sehingga membuat saya inisiatif untuk menggabungkan saya pesan lognya dan terpaksa harus mengubah (merevisi) sedikit program tersebut. (Kerancuan soal)
- Terkadang saat mount point tidak bisa di unmount, saya harus restart lagi OS saya. (buang-buang waktu)

- [Task 2 - Fu53enC0d3D==](/task-2/) by Rey
#### Soal
Fu53enC0d3D==

Keamanan digital adalah suatu aspek penting dalam dunia digital, salah satu topik yang sangat menarik adalah keamanan dari suatu file. Cara cara untuk mengamankan isi dari suatu file salah satunya dapat menggunakan encode pada isi file tersebut. Encode adalah sebuah proses yang dilakukan untuk mengamankan data dan informasi dan membuatnya menjadi tidak bisa dibaca dengan mudah jika tidak ada pengetahuan khusus. Adapun salah satu encode paling terkenal adalah encode Base64.

Tugas kalian adalah merancang suatu sistem keamanan pada file file user menggunakan FUSE pada file [fuse.c](./fuse.c), dengan penjabaran tugas sebagai berikut:

- a. Clone [direktori target](https://github.com/hqlco/target.git) dan buatlah 4 user pada linux dengan username sesuai isi folder pada direktori target (password dibebaskan).

  | User | Folder |
  | ---- | ------ |
  | andi | andi   |
  | budi | budi   |
  | cony | cony   |
  | deni | deni   |

  Tabel kepemilikan folder

- b. Ketika folder mount point dari fuse yang kalian buat diakses, akan langsung menuju ke dalam target folder yang telah di clone (tidak dimulai dari root)

  ![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/1383686d-9ba2-4493-9297-3cdfdb7dcdac)

- c. Buatlah agar tiap user tidak dapat menambah, mengubah, maupun menghapus folder maupun file dalam folder yang bukan miliknya.

- d. Buatlah agar user dapat menambah, mengubah, maupun menghapus folder maupun file dalam folder miliknya.

  command yang relevan untuk poin c and d:
  
  - mkdir
  - rmdir
  - touch
  - rm
  - cp
  - mv
  - etc.

- e. Semua isi file akan ter-encode jika diakses oleh selain user pemilik folder tersebut (menggunakan encoding Base64).

- f. Sebaliknya, file akan ter-decode ketika diakses oleh user pemilik folder tersebut.

Contoh:

![image](https://github.com/arsitektur-jaringan-komputer/Modul-Sisop/assets/54766683/932c2acd-2f85-4abf-9fc9-9c38737d1605)

Note:

- Nama folder mounting dibebaskan
- Agar fuse dapat diakses oleh semua user, perlu ditambahkan argumen “-o allow_other” ketika melakukan mount

#### Jawaban
Full Code:
```
#define FUSE_USE_VERSION 30



#include <ctype.h>

#include <fuse.h>

#include <sys/stat.h>

#include <dirent.h>

#include <limits.h>

#include <sys/ioctl.h>

#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <libgen.h>

#include <sys/xattr.h>

#include <pwd.h>

#include <string.h>

#include <errno.h>

#include <fcntl.h>

#include <unistd.h>



const char* basepath = "/home/rey/praktikum-modul-4-b24/task-2/target";



#define PATH_MAX 4096

#define INITIAL_SIZE 4096



static void fullpath(char fpath[PATH_MAX], const char *path) {

    snprintf(fpath, PATH_MAX, "%s%s", basepath, path);

}



static int fuse_getattr(const char *path, struct stat *stbuf) {

    char fpath[PATH_MAX];

    fullpath(fpath, path);



    int res = lstat(fpath, stbuf);

    if (res == -1) {

        return -errno;

    }



    if (S_ISREG(stbuf->st_mode)) {

        stbuf->st_size = INITIAL_SIZE;

    }



    return 0;

}



static int cek_owner(const char *path) {

    char fpath[PATH_MAX];

    char dir_path[PATH_MAX];



    fullpath(fpath, path);

    strncpy(dir_path, fpath, sizeof(dir_path));

    dir_path[sizeof(dir_path) - 1] = '\0';



    char *parent_dir = dirname(dir_path);

 



    struct stat st;

    if (lstat(parent_dir, &st) == -1)

        return 0;



    struct fuse_context *context = fuse_get_context();

    return st.st_uid == context->uid;

}



char* get_username_by_id(uid_t uid) {

    struct passwd *pw = getpwuid(uid);

    if (pw) {

        return pw->pw_name;

    }

    return NULL;

}



char* get_current_username() {

    struct fuse_context *context = fuse_get_context();

    if (!context) {

        return NULL;

    }

    return get_username_by_id(context->uid);

}



unsigned char* read_file(const char* filename, size_t* file_size) {



    FILE* file = fopen(filename, "rb");

    if (!file) {

        return NULL;

    }



    fseek(file, 0, SEEK_END);

    *file_size = ftell(file);

    fseek(file, 0, SEEK_SET);



    unsigned char* buffer = (unsigned char*)malloc(*file_size);

    if (!buffer) {

        fclose(file);

        return NULL;

    }



    size_t read_size = fread(buffer, 1, *file_size, file);

    fclose(file);



    if (read_size != *file_size) {

        free(buffer);

        return NULL;

    }

    return buffer;

}



char *base64_encode(const unsigned char *data, size_t input_length) {

    static const char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    static const int mod_table[] = {0, 2, 1};

    size_t output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = (char *)malloc(output_length + 2);

    if (encoded_data == NULL) return NULL;

    for (size_t i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? data[i++] : 0;

        uint32_t octet_b = i < input_length ? data[i++] : 0;

        uint32_t octet_c = i < input_length ? data[i++] : 0;

        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];

        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];

        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];

        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];

    }

    for (size_t i = 0; i < mod_table[input_length % 3]; i++) {

        encoded_data[output_length - 1 - i] = '=';

    }

    encoded_data[output_length] = '\n';

    encoded_data[output_length + 1] = '\0';

    return encoded_data;

}



static int fuse_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {

    char fpath[PATH_MAX];

   fullpath(fpath, path);



    struct stat st;

    if (stat(fpath, &st) == -1) {

        return -errno;

    }

    uid_t file_owner = st.st_uid;



    struct fuse_context *context = fuse_get_context();

    uid_t current_user = context->uid;



    size_t file_size;

    unsigned char *file_content = read_file(fpath, &file_size);

    if (!file_content) {

        return -errno;

    }



    const char *content_to_read;

    char *encoded_content = NULL;

    size_t content_size;



    if (current_user != file_owner) {

        encoded_content = base64_encode(file_content, file_size);

        if (!encoded_content) {

            free(file_content);

            return -ENOMEM;

        }

        content_to_read = encoded_content;

        content_size = strlen(encoded_content);

    } else {

        content_to_read = (const char *)file_content;

        content_size = file_size;

    }



    size_t bytes_to_copy = 0;

    if (offset < content_size) {

        bytes_to_copy = (size < (content_size - offset)) ? size : (content_size - offset);

        memcpy(buf, content_to_read + offset, bytes_to_copy);

    }



    free(file_content);

    if (encoded_content) {

        free(encoded_content);

    }

    return bytes_to_copy;

}



static int fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {

    DIR *dp;

    struct dirent *de;



    char fpath[PATH_MAX];

    fullpath(fpath, path);



    dp = opendir(fpath);

    if (dp == NULL)

        return -errno;



    while ((de = readdir(dp)) != NULL) {

        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;

        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0))

            break;

    }



    closedir(dp);

    return 0;

}



static int fuse_mkdir(const char *path, mode_t mode) {

    mode = 0777;



    if (!cek_owner(path))

        return -EACCES;



    char fpath[PATH_MAX];

   

   fullpath(fpath, path);

     

    char *username = get_current_username();

    int res = mkdir(fpath, mode);

    

    if (res == -1)

        return -errno;

    

    char cmd[255];

    snprintf(cmd, sizeof(cmd), "sudo chown %s:%s %s", username, username, fpath);

    system(cmd);



    return 0;

}



static int fuse_rename(const char *from, const char *to) {

    



    if (!cek_owner(from) || !cek_owner(to))

        return -EACCES;

    

   

    char full_from[PATH_MAX];

    char full_to[PATH_MAX];

    fullpath(full_from, from);

    fullpath(full_to, to);



        char *username = get_current_username();

    int res = rename(full_from, full_to);

    if (res == -1)

        return -errno;

    char cmd[255];

    snprintf(cmd, sizeof(cmd), "sudo chown %s:%s %s", username, username, full_to);

    system(cmd);

    

    return 0;

}



static int fuse_rmdir(const char *path) {





    if (!cek_owner(path))

        return -EACCES;

    



    char fpath[PATH_MAX];

    fullpath(fpath, path);



    int res = rmdir(fpath);

    if (res == -1)

        return -errno;

   

    return 0;

}



static int fuse_mknod(const char *path, mode_t mode, dev_t dev) {

   

    mode = 0777;

    if (!cek_owner(path))

        return -EACCES;



    char fpath[PATH_MAX];

    fullpath(fpath, path);

         char *username = get_current_username();

    int res;

    if (S_ISREG(mode)) {

        res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);

        if (res >= 0)

            res = close(res);

    } else if (S_ISFIFO(mode)) {

        res = mkfifo(fpath, mode);

    } else {

        res = mknod(fpath, mode, dev);

    }



    if (res == -1)

        return -errno;

    char cmd[255];

    snprintf(cmd, sizeof(cmd), "sudo chown %s:%s %s", username, username, fpath);

    system(cmd);



    return 0;

}



static int fuse_create(const char *path, mode_t mode, struct fuse_file_info *fi) {

  

    mode = 0777;

    if (!cek_owner(path))

        return -EACCES;

     char *username = get_current_username();

    char fpath[PATH_MAX];

    fullpath(fpath, path);



    int res = open(fpath, O_CREAT | O_WRONLY | O_TRUNC, mode);

    if (res == -1)

        return -errno;



    close(res);

    char cmd[255];

    snprintf(cmd, sizeof(cmd), "sudo chown %s:%s %s", username, username, fpath);

    system(cmd);

    

    return 0;

}



static int fuse_unlink(const char *path) {

   



    if (!cek_owner(path))

        return -EACCES;



    char fpath[PATH_MAX];

    snprintf(fpath, sizeof(fpath), "%s%s", basepath, path);



    int res = unlink(fpath);

    if (res == -1)

        return -errno;

   

    return 0;

}



#define fuse_IOCTL_COPY _IOWR('m', 1, char[2][PATH_MAX])



static int fuse_cp(const char *src, const char *dest) {



    if (!cek_owner(src) || !cek_owner(dest))

        return -EACCES;

     

    char full_src[PATH_MAX];

    char full_dest[PATH_MAX];



    

    snprintf(full_src, sizeof(full_src), "%s%s", basepath, src);

    snprintf(full_dest, sizeof(full_dest), "%s%s", basepath, dest);



    int src_fd = open(full_src, O_RDONLY);

    if (src_fd == -1) {

        return -errno;

    }



    int dest_fd = open(full_dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (dest_fd == -1) {

        close(src_fd);

        return -errno;

    }

    

    char buf[4096];

    ssize_t bytes_read, bytes_written;

    while ((bytes_read = read(src_fd, buf, sizeof(buf))) > 0) {

        bytes_written = write(dest_fd, buf, bytes_read);

        if (bytes_written != bytes_read) {

            close(src_fd);

            close(dest_fd);

            return -errno;

        }

    }



    close(src_fd);

    close(dest_fd);



    if (bytes_read == -1) {

        return -errno;

    }



    return 0;

}



static int fuse_ioctl(const char *path, int cmd, void *arg, struct fuse_file_info *fi, unsigned int flags, void *data) {

    if (cmd == fuse_IOCTL_COPY) {

        char (*paths)[PATH_MAX] = arg;

        return fuse_cp(paths[0], paths[1]);

    }

    return -ENOTTY;

}





static struct fuse_operations fuse_oper = {

    .getattr    = fuse_getattr,

    .read       = fuse_read,

    .readdir    = fuse_readdir,

    .mkdir      = fuse_mkdir,

    .rename     = fuse_rename,

    .rmdir      = fuse_rmdir,

    .mknod      = fuse_mknod,

    .create     = fuse_create,

    .unlink     = fuse_unlink,

    .ioctl      = fuse_ioctl,

};







void setup() {

    const char* dir[] = { "andi", "budi", "cony", "deni" };

    const char* users[] = { "andi", "budi", "cony", "deni" };

    char uid_cmd[128];

    char uid_str[16];

    char cmd[PATH_MAX];

    FILE *fp;



    for (int i = 0; i < 4; i++) {

        snprintf(uid_cmd, sizeof(uid_cmd), "id -u %s", users[i]);

        fp = popen(uid_cmd, "r");

        if (fp == NULL) {

            continue;

        }

        if (fgets(uid_str, sizeof(uid_str), fp) != NULL) {

            strtok(uid_str, "\n");

            snprintf(cmd, sizeof(cmd), "sudo chown -R %s:%s %s/%s", users[i], users[i], basepath, dir[i]);

            system(cmd);

            snprintf(cmd, sizeof(cmd), "sudo chmod -R 777 %s/%s", basepath, dir[i]);

            system(cmd);

        }

        pclose(fp);

    }

}



int main(int argc, char *argv[]) {

    umask(0);

    setup();

    return fuse_main(argc, argv, &fuse_oper, NULL);

}
```

Penjelasan per blok kode
```
#define FUSE_USE_VERSION 30
```
Baris ini berfungsi untuk menetapkan versi dari FUSE yang digunakan yaitu 3.0

```
#include <ctype.h>

#include <fuse.h>

#include <sys/stat.h>

#include <dirent.h>

#include <limits.h>

#include <sys/ioctl.h>

#include <stdio.h>

#include <stdlib.h>

#include <sys/types.h>

#include <libgen.h>

#include <sys/xattr.h>

#include <pwd.h>

#include <string.h>

#include <errno.h>

#include <fcntl.h>

#include <unistd.h>
```
Blok kode tersebut berfungsi untuk mengimpor atau memasukkan (include) seluruh library yang diperlukan dalam program ini.

```
const char* basepath = "/home/rey/praktikum-modul-4-b24/task-2/target";
```
- Baris ini mendeklarasikan sebuah variabel `basepath` yang bertipe `const char*`. Variabel ini adalah sebuah pointer ke string (teks) yang tidak bisa diubah nilainya, dan menunjukkan path dasar (lokasi dasar) pada sistem file, yaitu `/home/rey/praktikum-modul-4-b24/task-2/target`. Ini digunakan sebagai direktori dasar untuk operasi file dalam program ini.

```
#define PATH_MAX 4096
```
- Baris ini mendefinisikan sebuah makro `PATH_MAX` dengan nilai 4096. Makro ini digunakan untuk menentukan panjang maksimum dari sebuah path (lokasi file atau direktori). Jadi, path yang digunakan dalam program ini tidak boleh melebihi 4096 karakter.

```
#define INITIAL_SIZE 4096
```
- Baris ini mendefinisikan sebuah makro `INITIAL_SIZE` dengan nilai 4096. Makro ini digunakan untuk menetapkan ukuran awal dari sebuah file atau buffer. Misalnya, saat mengatur ukuran file baru atau buffer yang akan digunakan untuk operasi tertentu.

Jadi, secara keseluruhan, blok kode ini menetapkan path dasar untuk operasi file, serta mendefinisikan panjang maksimum path dan ukuran awal yang akan digunakan dalam program ini.

Berikut adalah penjelasan kode tersebut dalam bahasa Indonesia sederhana:

```
static void fullpath(char fpath[PATH_MAX], const char *path) {
    snprintf(fpath, PATH_MAX, "%s%s", basepath, path);
}
```

- Fungsi ini bernama `fullpath` dan dideklarasikan sebagai `static`, yang berarti fungsi ini hanya dapat diakses dalam file tempatnya dideklarasikan.

- Fungsi ini memiliki dua parameter:
  - `fpath`: Sebuah array karakter dengan ukuran `PATH_MAX` (4096), yang akan diisi dengan path lengkap.
  - `path`: Sebuah pointer ke string (teks) yang berisi path relatif (lokasi relatif file atau direktori).

- Di dalam fungsi, `snprintf` digunakan untuk membentuk path lengkap. Fungsi ini menggabungkan `basepath` dengan `path` dan menyimpannya ke dalam `fpath`. 

- `snprintf` adalah fungsi yang aman untuk menulis string, karena memastikan bahwa panjang string yang ditulis tidak melebihi ukuran yang ditentukan (`PATH_MAX` dalam hal ini).

- Secara sederhana, fungsi ini mengambil path dasar (`basepath`) dan path relatif (`path`), lalu menggabungkannya untuk membentuk path lengkap, yang kemudian disimpan di dalam `fpath`.

Contoh:
Jika `basepath` adalah "/home/rey/praktikum-modul-4-b24/task-2/target" dan `path` adalah "/file.txt", maka setelah pemanggilan fungsi ini, `fpath` akan berisi "/home/rey/praktikum-modul-4-b24/task-2/target/file.txt".

Berikut adalah penjelasan kode tersebut dalam bahasa Indonesia sederhana:

```
static int fuse_getattr(const char *path, struct stat *stbuf) {

    char fpath[PATH_MAX];

    fullpath(fpath, path);
```
- Fungsi `fuse_getattr` ini mengambil atribut file dari path yang diberikan.
- `path` adalah path relatif dari file atau direktori.
- `stbuf` adalah pointer ke struktur `stat` yang akan diisi dengan informasi atribut file.
- `fullpath` digunakan untuk menggabungkan `basepath` dan `path` menjadi `fpath`, yang merupakan path lengkap dari file.

```
    int res = lstat(fpath, stbuf);

    if (res == -1) {

        return -errno;

    }
```
- `lstat` digunakan untuk mendapatkan informasi atribut file dari `fpath` dan menyimpannya di `stbuf`.
- Jika `lstat` gagal (mengembalikan -1), fungsi ini mengembalikan nilai kesalahan (`-errno`).

```
    if (S_ISREG(stbuf->st_mode)) {

        stbuf->st_size = INITIAL_SIZE;

    }
```
- Jika file yang diperiksa adalah file reguler (bukan direktori, link, dll.), ukuran file (`st_size`) diatur ke nilai `INITIAL_SIZE` (4096).

```
    return 0;

}
```
- Jika berhasil, fungsi ini mengembalikan 0.

Secara sederhana, fungsi ini mendapatkan informasi tentang sebuah file atau direktori dari path yang diberikan, dan jika file tersebut adalah file reguler, ukurannya diatur menjadi 4096. Jika ada kesalahan saat mengambil informasi, fungsi ini mengembalikan kode kesalahan.

```
static int cek_owner(const char *path) {

    char fpath[PATH_MAX];

    char dir_path[PATH_MAX];



    fullpath(fpath, path);

    strncpy(dir_path, fpath, sizeof(dir_path));

    dir_path[sizeof(dir_path) - 1] = '\0';



    char *parent_dir = dirname(dir_path);

 



    struct stat st;

    if (lstat(parent_dir, &st) == -1)

        return 0;



    struct fuse_context *context = fuse_get_context();

    return st.st_uid == context->uid;

}
```
Blok kode ini berfungsi untuk memeriksa apakah pengguna saat ini adalah pemilik dari direktori yang berisi file yang ditentukan. Berikut adalah penjelasan sederhana dari setiap langkahnya:

1. Pertama, fungsi `cek_owner` menerima `path`, yang merupakan path relatif dari file yang ingin diperiksa.
2. Selanjutnya, `fullpath` digunakan untuk mendapatkan path lengkap dari file.
3. Kemudian, path lengkap tersebut disalin ke dalam `dir_path`.
4. `dirname` digunakan untuk mendapatkan path direktori induk dari `dir_path`.
5. Informasi atribut dari direktori induk diperoleh menggunakan `lstat`.
6. Setelah itu, informasi konteks FUSE saat ini (termasuk UID pengguna) diperoleh.
7. Akhirnya, fungsi ini mengembalikan nilai `1` jika UID dari direktori induk sama dengan UID pengguna saat ini, yang menunjukkan bahwa pengguna saat ini adalah pemilik dari direktori tersebut. Jika tidak, mengembalikan nilai `0`.

```
char* get_username_by_id(uid_t uid) {

    struct passwd *pw = getpwuid(uid);

    if (pw) {

        return pw->pw_name;

    }

    return NULL;

}
```
Blok kode tersebut merupakan sebuah fungsi yang bertujuan untuk mendapatkan nama pengguna (username) berdasarkan ID pengguna (UID). Berikut penjelasan sederhana dari fungsi tersebut:

1. Fungsi `get_username_by_id` menerima `uid` sebagai argumen, yang merupakan ID pengguna (UID) yang ingin dicari namanya.
2. Selanjutnya, fungsi menggunakan `getpwuid(uid)` untuk mendapatkan informasi pengguna berdasarkan UID yang diberikan.
3. Jika informasi pengguna berhasil ditemukan (tidak NULL), maka fungsi mengembalikan nama pengguna (`pw->pw_name`).
4. Jika tidak ada informasi pengguna yang ditemukan (NULL), maka fungsi mengembalikan NULL.

Jadi, secara keseluruhan, fungsi ini digunakan untuk mencari nama pengguna berdasarkan UID yang diberikan, dan mengembalikan nama pengguna tersebut jika ditemukan, atau NULL jika tidak ditemukan.
Jadi, secara keseluruhan, blok kode ini berfungsi untuk memeriksa kepemilikan direktori yang berisi file yang ditentukan oleh `path`.

```
char* get_current_username() {

    struct fuse_context *context = fuse_get_context();

    if (!context) {

        return NULL;

    }

    return get_username_by_id(context->uid);

}
```
Blok kode ini adalah sebuah fungsi yang bertujuan untuk mendapatkan nama pengguna (username) dari pengguna yang saat ini sedang menggunakan sistem file FUSE. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `get_current_username` tidak menerima argumen apa pun.
2. Pertama, fungsi mencoba untuk mendapatkan konteks FUSE saat ini menggunakan `fuse_get_context()`.
3. Jika konteks tidak berhasil ditemukan (konteks NULL), itu berarti tidak mungkin untuk mendapatkan informasi tentang pengguna saat ini. Oleh karena itu, fungsi mengembalikan nilai NULL.
4. Jika konteks FUSE berhasil ditemukan, maka fungsi menggunakan ID pengguna (UID) dari konteks tersebut untuk memanggil fungsi `get_username_by_id`.
5. Fungsi `get_username_by_id` akan mencari nama pengguna berdasarkan UID yang diberikan dan mengembalikan namanya.
6. Akhirnya, nama pengguna tersebut dikembalikan oleh fungsi `get_current_username`.

Jadi, secara keseluruhan, fungsi ini digunakan untuk mendapatkan nama pengguna dari pengguna yang saat ini sedang menggunakan sistem file FUSE. Namun, jika tidak mungkin mendapatkan informasi pengguna saat ini, fungsi akan mengembalikan nilai NULL.

```
unsigned char* read_file(const char* filename, size_t* file_size) {



    FILE* file = fopen(filename, "rb");

    if (!file) {

        return NULL;

    }



    fseek(file, 0, SEEK_END);

    *file_size = ftell(file);

    fseek(file, 0, SEEK_SET);



    unsigned char* buffer = (unsigned char*)malloc(*file_size);

    if (!buffer) {

        fclose(file);

        return NULL;

    }



    size_t read_size = fread(buffer, 1, *file_size, file);

    fclose(file);



    if (read_size != *file_size) {

        free(buffer);

        return NULL;

    }

    return buffer;

}
```
Fungsi ini bertujuan untuk membaca isi dari sebuah file dan mengembalikan isinya dalam bentuk array unsigned char, serta mengembalikan ukuran file melalui pointer `file_size`. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `read_file` menerima dua argumen: `filename`, yang merupakan nama file yang akan dibaca, dan `file_size`, yang merupakan pointer ke variabel untuk menyimpan ukuran file.

2. Pertama, fungsi mencoba membuka file dengan mode "rb" (read binary) menggunakan fungsi `fopen`.

3. Jika file tidak berhasil dibuka (file NULL), fungsi mengembalikan nilai NULL, menandakan bahwa file tidak dapat dibaca.

4. Kemudian, fungsi menghitung ukuran file dengan menggunakan `fseek` untuk mencapai akhir file, `ftell` untuk mendapatkan posisi akhir file (yang menunjukkan ukuran), lalu `fseek` kembali ke awal file.

5. Fungsi mengalokasikan memori untuk buffer sebesar ukuran file yang telah didapatkan. Jika alokasi memori gagal, file ditutup dan fungsi mengembalikan nilai NULL.

6. Selanjutnya, isi dari file dibaca ke dalam buffer menggunakan `fread`. Jika jumlah byte yang dibaca tidak sama dengan ukuran file yang diharapkan, hal ini menunjukkan adanya kesalahan dalam proses membaca file.

7. Jika pembacaan berhasil (jumlah byte yang dibaca sama dengan ukuran file), buffer yang berisi isi file dikembalikan oleh fungsi.

Jadi, fungsi ini secara sederhana membaca isi dari sebuah file, menyimpannya dalam buffer, dan mengembalikan buffer tersebut, serta ukuran file melalui pointer `file_size`.

```
char *base64_encode(const unsigned char *data, size_t input_length) {

    static const char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    static const int mod_table[] = {0, 2, 1};

    size_t output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = (char *)malloc(output_length + 2);

    if (encoded_data == NULL) return NULL;

    for (size_t i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? data[i++] : 0;

        uint32_t octet_b = i < input_length ? data[i++] : 0;

        uint32_t octet_c = i < input_length ? data[i++] : 0;

        uint32_t triple = (octet_a << 16) + (octet_b << 8) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];

        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];

        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];

        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];

    }

    for (size_t i = 0; i < mod_table[input_length % 3]; i++) {

        encoded_data[output_length - 1 - i] = '=';

    }

    encoded_data[output_length] = '\n';

    encoded_data[output_length + 1] = '\0';

    return encoded_data;

}
```

Fungsi ini bertujuan untuk melakukan enkode data dalam format base64. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `base64_encode` menerima dua argumen: `data`, yang merupakan array unsigned char yang akan diencode, dan `input_length`, yang merupakan panjang data yang akan diencode.

2. Fungsi menggunakan dua tabel konstanta: `encoding_table`, yang berisi karakter-karakter yang digunakan dalam encoding base64, dan `mod_table`, yang digunakan untuk menentukan jumlah padding yang diperlukan.

3. Ukuran output untuk data yang diencode dihitung berdasarkan panjang data input dan disimpan dalam `output_length`. Setiap tiga byte data input diencode menjadi empat karakter base64.

4. Memori dialokasikan untuk menyimpan hasil enkode dalam `encoded_data`, dengan ukuran sesuai dengan `output_length`.

5. Kemudian, dilakukan iterasi untuk setiap tiga byte data input. Tiap byte dibagi menjadi enam bit, lalu dikonversi menjadi karakter base64 menggunakan tabel encoding. Karakter-karakter ini disimpan dalam `encoded_data`.

6. Setelah semua data diencode, dilakukan penambahan padding jika diperlukan. Karakter padding (`=`) ditambahkan sesuai dengan nilai yang terdapat dalam `mod_table`.

7. Akhirnya, ditambahkan karakter null-terminator pada akhir string yang diencode, dan hasil enkode dikembalikan.

Jadi, secara sederhana, fungsi ini mengambil data dalam bentuk biner, mengonversinya menjadi karakter base64, dan mengembalikan string hasil enkode.

```
static int fuse_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {

    char fpath[PATH_MAX];

   fullpath(fpath, path);



    struct stat st;

    if (stat(fpath, &st) == -1) {

        return -errno;

    }

    uid_t file_owner = st.st_uid;



    struct fuse_context *context = fuse_get_context();

    uid_t current_user = context->uid;



    size_t file_size;

    unsigned char *file_content = read_file(fpath, &file_size);

    if (!file_content) {

        return -errno;

    }



    const char *content_to_read;

    char *encoded_content = NULL;

    size_t content_size;



    if (current_user != file_owner) {

        encoded_content = base64_encode(file_content, file_size);

        if (!encoded_content) {

            free(file_content);

            return -ENOMEM;

        }

        content_to_read = encoded_content;

        content_size = strlen(encoded_content);

    } else {

        content_to_read = (const char *)file_content;

        content_size = file_size;

    }



    size_t bytes_to_copy = 0;

    if (offset < content_size) {

        bytes_to_copy = (size < (content_size - offset)) ? size : (content_size - offset);

        memcpy(buf, content_to_read + offset, bytes_to_copy);

    }



    free(file_content);

    if (encoded_content) {

        free(encoded_content);

    }

    return bytes_to_copy;

}
```
Fungsi ini bertanggung jawab untuk membaca isi file dari path yang diberikan dan menyalinnya ke dalam buffer yang disediakan. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `fuse_read` menerima lima argumen: `path`, `buf`, `size`, `offset`, dan `fi`. 
   - `path` adalah path relatif dari file yang akan dibaca.
   - `buf` adalah buffer tempat data akan disalin.
   - `size` adalah ukuran buffer.
   - `offset` adalah posisi dalam file dari mana pembacaan akan dimulai.
   - `fi` adalah pointer ke struktur informasi file FUSE.

2. Path lengkap dari file diambil menggunakan fungsi `fullpath`, yang menyimpannya di dalam `fpath`.

3. Informasi atribut file (seperti pemilik file) diambil menggunakan fungsi `stat` dan disimpan dalam struktur `st`.

4. UID pemilik file dan UID pengguna saat ini diambil untuk membandingkannya nanti.

5. Isi file dibaca menggunakan fungsi `read_file`, dan ukuran file disimpan.

6. Jika tidak dapat membaca file, fungsi mengembalikan kode kesalahan.

7. Jika pengguna saat ini tidak sama dengan pemilik file, isi file diencode menjadi base64 menggunakan fungsi `base64_encode`. Hasilnya disimpan dalam `encoded_content`.

8. Jika ada kegagalan dalam mengalokasikan memori untuk encoding base64, fungsi mengembalikan kode kesalahan.

9. Jika pengguna saat ini adalah pemilik file, isi file disalin langsung ke dalam `buf`.

10. Jika pembacaan dimulai dari posisi yang lebih kecil dari ukuran file, data yang dibaca akan disalin ke dalam `buf`.

11. Memori yang dialokasikan untuk isi file dan encoding base64 kemudian dibebaskan.

12. Jumlah byte yang berhasil disalin ke dalam `buf` dikembalikan sebagai nilai pengembalian fungsi.

Jadi, secara sederhana, fungsi ini membaca isi file, mungkin mengkodekannya menjadi base64 tergantung pada siapa yang meminta bacaan, dan menyalinnya ke dalam buffer yang disediakan.

```
static int fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {

    DIR *dp;

    struct dirent *de;



    char fpath[PATH_MAX];

    fullpath(fpath, path);



    dp = opendir(fpath);

    if (dp == NULL)

        return -errno;



    while ((de = readdir(dp)) != NULL) {

        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;

        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0))

            break;

    }



    closedir(dp);

    return 0;

}
```
Fungsi ini bertujuan untuk membaca isi dari sebuah direktori dan mengisi buffer dengan nama-nama file yang terdapat di dalamnya. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `fuse_readdir` menerima lima argumen: `path`, `buf`, `filler`, `offset`, dan `fi`.
   - `path` adalah path dari direktori yang akan dibaca.
   - `buf` adalah pointer ke buffer tempat nama-nama file akan disimpan.
   - `filler` adalah fungsi yang digunakan untuk mengisi buffer dengan nama-nama file.
   - `offset` adalah posisi dalam direktori dari mana pembacaan akan dimulai.
   - `fi` adalah pointer ke struktur informasi file FUSE.

2. Path lengkap dari direktori diambil menggunakan fungsi `fullpath`, yang menyimpannya dalam `fpath`.

3. Direktori dibuka menggunakan fungsi `opendir`, dan handle direktori disimpan dalam variabel `dp`.

4. Jika tidak dapat membuka direktori, fungsi mengembalikan kode kesalahan.

5. Selama masih ada entri dalam direktori yang belum dibaca, fungsi membaca entri tersebut satu per satu menggunakan `readdir`.

6. Untuk setiap entri yang dibaca, informasi atribut file (seperti inode dan mode) diatur dalam struktur `st`, dan nama file disimpan dalam buffer menggunakan fungsi `filler`.

7. Jika buffer sudah penuh, fungsi `filler` akan mengembalikan nilai yang tidak nol, dan perulangan dihentikan.

8. Setelah selesai membaca semua entri, handle direktori ditutup menggunakan fungsi `closedir`.

9. Fungsi mengembalikan nilai 0 untuk menandakan bahwa proses pembacaan direktori telah selesai dengan sukses.

Jadi, secara sederhana, fungsi ini membuka direktori, membaca entri-entri di dalamnya, mengisi buffer dengan nama-nama file, dan kemudian menutup direktori setelah selesai.

```
static int fuse_mkdir(const char *path, mode_t mode) {

    mode = 0777;



    if (!cek_owner(path))

        return -EACCES;



    char fpath[PATH_MAX];

   

   fullpath(fpath, path);

     

    char *username = get_current_username();

    int res = mkdir(fpath, mode);

    

    if (res == -1)

        return -errno;

    

    char cmd[255];

    snprintf(cmd, sizeof(cmd), "sudo chown %s:%s %s", username, username, fpath);

    system(cmd);



    return 0;

}
```
Fungsi ini digunakan untuk membuat direktori baru pada path yang diberikan. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `fuse_mkdir` menerima dua argumen: `path`, yang merupakan path relatif dari direktori yang akan dibuat, dan `mode`, yang menentukan hak akses untuk direktori yang baru dibuat.

2. Hak akses untuk direktori yang akan dibuat diatur menjadi 0777.

3. Fungsi memeriksa apakah pengguna saat ini memiliki hak akses untuk membuat direktori pada path yang diberikan menggunakan fungsi `cek_owner`. Jika tidak, maka fungsi mengembalikan kode kesalahan `-EACCES`, menunjukkan bahwa akses ditolak.

4. Path lengkap dari direktori yang akan dibuat diambil menggunakan fungsi `fullpath`, dan disimpan dalam variabel `fpath`.

5. Nama pengguna saat ini diambil menggunakan fungsi `get_current_username`.

6. Direktori baru kemudian dibuat menggunakan fungsi `mkdir`, dengan hak akses yang sudah ditentukan.

7. Jika pembuatan direktori gagal, fungsi mengembalikan kode kesalahan.

8. Setelah berhasil membuat direktori, fungsi menggunakan perintah sistem untuk mengubah kepemilikan direktori ke pengguna saat ini menggunakan perintah `chown`.

9. Fungsi mengembalikan nilai 0 untuk menandakan bahwa pembuatan direktori telah berhasil.

Jadi, secara sederhana, fungsi ini bertanggung jawab untuk membuat direktori baru, mengatur hak akses, dan mengubah kepemilikan direktori tersebut ke pengguna yang memanggil fungsi.

```
static int fuse_rename(const char *from, const char *to) {

    



    if (!cek_owner(from) || !cek_owner(to))

        return -EACCES;

    

   

    char full_from[PATH_MAX];

    char full_to[PATH_MAX];

    fullpath(full_from, from);

    fullpath(full_to, to);



        char *username = get_current_username();

    int res = rename(full_from, full_to);

    if (res == -1)

        return -errno;

    char cmd[255];

    snprintf(cmd, sizeof(cmd), "sudo chown %s:%s %s", username, username, full_to);

    system(cmd);

    

    return 0;

}
```
Fungsi ini bertujuan untuk mengganti nama sebuah file atau direktori dari path "from" ke path "to". Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `fuse_rename` menerima dua argumen: `from`, yang merupakan path dari file atau direktori yang akan diubah namanya, dan `to`, yang merupakan path baru yang akan diberikan pada file atau direktori tersebut.

2. Fungsi memeriksa apakah pengguna saat ini memiliki hak akses untuk mengubah nama file atau direktori dari path "from" dan path "to" menggunakan fungsi `cek_owner`. Jika tidak, maka fungsi mengembalikan kode kesalahan `-EACCES`, menunjukkan bahwa akses ditolak.

3. Path lengkap dari path "from" dan path "to" diambil menggunakan fungsi `fullpath`, dan disimpan dalam variabel `full_from` dan `full_to`.

4. Nama pengguna saat ini diambil menggunakan fungsi `get_current_username`.

5. Kemudian, fungsi `rename` dipanggil untuk mengubah nama file atau direktori dari path "from" menjadi path "to".

6. Jika penggantian nama gagal, fungsi mengembalikan kode kesalahan.

7. Setelah berhasil mengubah nama file atau direktori, fungsi menggunakan perintah sistem untuk mengubah kepemilikan file atau direktori yang baru ke pengguna saat ini menggunakan perintah `chown`.

8. Fungsi mengembalikan nilai 0 untuk menandakan bahwa penggantian nama telah berhasil.

Jadi, secara sederhana, fungsi ini bertanggung jawab untuk mengubah nama file atau direktori dari path "from" ke path "to" dan mengatur kepemilikan file atau direktori yang baru kepada pengguna yang memanggil fungsi.

```
static int fuse_rmdir(const char *path) {

    if (!cek_owner(path))

        return -EACCES;


    char fpath[PATH_MAX];

    fullpath(fpath, path);

    int res = rmdir(fpath);

    if (res == -1)

        return -errno;


    return 0;

}
```
Fungsi ini digunakan untuk menghapus sebuah direktori. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `fuse_rmdir` menerima satu argumen: `path`, yang merupakan path dari direktori yang akan dihapus.

2. Fungsi memeriksa apakah pengguna saat ini memiliki hak akses untuk menghapus direktori tersebut menggunakan fungsi `cek_owner`. Jika tidak, maka fungsi mengembalikan kode kesalahan `-EACCES`, menunjukkan bahwa akses ditolak.

3. Path lengkap dari direktori yang akan dihapus diambil menggunakan fungsi `fullpath`, dan disimpan dalam variabel `fpath`.

4. Fungsi `rmdir` dipanggil untuk menghapus direktori tersebut.

5. Jika penghapusan gagal, fungsi mengembalikan kode kesalahan.

6. Setelah berhasil menghapus direktori, fungsi mengembalikan nilai 0 untuk menandakan bahwa penghapusan telah berhasil.

Jadi, secara sederhana, fungsi ini bertanggung jawab untuk menghapus sebuah direktori, dengan memastikan bahwa pengguna yang memanggil fungsi memiliki hak akses yang sesuai.

```
static int fuse_mknod(const char *path, mode_t mode, dev_t dev) {

   

    mode = 0777;

    if (!cek_owner(path))

        return -EACCES;



    char fpath[PATH_MAX];

    fullpath(fpath, path);

         char *username = get_current_username();

    int res;

    if (S_ISREG(mode)) {

        res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);

        if (res >= 0)

            res = close(res);

    } else if (S_ISFIFO(mode)) {

        res = mkfifo(fpath, mode);

    } else {

        res = mknod(fpath, mode, dev);

    }



    if (res == -1)

        return -errno;

    char cmd[255];

    snprintf(cmd, sizeof(cmd), "sudo chown %s:%s %s", username, username, fpath);

    system(cmd);



    return 0;

}
```
Fungsi ini bertujuan untuk membuat node khusus (mknod) atau file khusus (membuat fifo atau berkas biasa). Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `fuse_mknod` menerima tiga argumen: `path`, `mode`, dan `dev`.
   - `path` adalah path dari node atau file yang akan dibuat.
   - `mode` adalah mode hak akses untuk node atau file yang akan dibuat.
   - `dev` adalah nomor perangkat (device number) untuk node yang akan dibuat.

2. Hak akses untuk node atau file yang akan dibuat diatur menjadi 0777.

3. Fungsi memeriksa apakah pengguna saat ini memiliki hak akses untuk membuat node atau file pada path yang diberikan menggunakan fungsi `cek_owner`. Jika tidak, maka fungsi mengembalikan kode kesalahan `-EACCES`, menunjukkan bahwa akses ditolak.

4. Path lengkap dari node atau file yang akan dibuat diambil menggunakan fungsi `fullpath`, dan disimpan dalam variabel `fpath`.

5. Nama pengguna saat ini diambil menggunakan fungsi `get_current_username`.

6. Berdasarkan mode yang diberikan, fungsi memutuskan jenis file yang akan dibuat: berkas biasa, FIFO (First In, First Out), atau node khusus. Sesuai dengan itu, fungsi menggunakan fungsi sistem yang sesuai (`open`, `mkfifo`, atau `mknod`) untuk membuat file atau node tersebut.

7. Jika pembuatan file atau node gagal, fungsi mengembalikan kode kesalahan.

8. Setelah berhasil membuat file atau node, fungsi menggunakan perintah sistem untuk mengubah kepemilikan file atau node tersebut ke pengguna saat ini menggunakan perintah `chown`.

9. Fungsi mengembalikan nilai 0 untuk menandakan bahwa pembuatan file atau node telah berhasil.

Jadi, secara sederhana, fungsi ini bertanggung jawab untuk membuat file atau node khusus dengan hak akses yang sesuai, serta mengatur kepemilikan file atau node tersebut kepada pengguna yang memanggil fungsi.

```
static int fuse_create(const char *path, mode_t mode, struct fuse_file_info *fi) {

  

    mode = 0777;

    if (!cek_owner(path))

        return -EACCES;

     char *username = get_current_username();

    char fpath[PATH_MAX];

    fullpath(fpath, path);



    int res = open(fpath, O_CREAT | O_WRONLY | O_TRUNC, mode);

    if (res == -1)

        return -errno;



    close(res);

    char cmd[255];

    snprintf(cmd, sizeof(cmd), "sudo chown %s:%s %s", username, username, fpath);

    system(cmd);

    

    return 0;

}
```
Fungsi ini bertujuan untuk membuat sebuah berkas baru. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `fuse_create` menerima tiga argumen: `path`, `mode`, dan `fi`.
   - `path` adalah path dari berkas yang akan dibuat.
   - `mode` adalah mode hak akses untuk berkas yang akan dibuat.
   - `fi` adalah pointer ke struktur `fuse_file_info` yang berisi informasi tambahan tentang file yang akan dibuat.

2. Hak akses untuk berkas yang akan dibuat diatur menjadi 0777.

3. Fungsi memeriksa apakah pengguna saat ini memiliki hak akses untuk membuat berkas pada path yang diberikan menggunakan fungsi `cek_owner`. Jika tidak, maka fungsi mengembalikan kode kesalahan `-EACCES`, menunjukkan bahwa akses ditolak.

4. Path lengkap dari berkas yang akan dibuat diambil menggunakan fungsi `fullpath`, dan disimpan dalam variabel `fpath`.

5. Nama pengguna saat ini diambil menggunakan fungsi `get_current_username`.

6. Fungsi `open` dipanggil dengan mode `O_CREAT | O_WRONLY | O_TRUNC` untuk membuat berkas baru. Mode ini mengindikasikan bahwa berkas akan dibuat, bisa ditulis, dan kontennya akan dihapus jika sudah ada.

7. Jika pembuatan berkas gagal, fungsi mengembalikan kode kesalahan.

8. Setelah berhasil membuat berkas, fungsi menutup file descriptor yang terbuka dengan fungsi `close`.

9. Fungsi menggunakan perintah sistem untuk mengubah kepemilikan berkas tersebut ke pengguna saat ini menggunakan perintah `chown`.

10. Fungsi mengembalikan nilai 0 untuk menandakan bahwa pembuatan berkas telah berhasil.

Jadi, secara sederhana, fungsi ini bertanggung jawab untuk membuat sebuah berkas baru dengan hak akses yang sesuai, serta mengatur kepemilikan berkas tersebut kepada pengguna yang memanggil fungsi.

```
static int fuse_unlink(const char *path) {

   



    if (!cek_owner(path))

        return -EACCES;



    char fpath[PATH_MAX];

    snprintf(fpath, sizeof(fpath), "%s%s", basepath, path);



    int res = unlink(fpath);

    if (res == -1)

        return -errno;

   

    return 0;

}
```
Fungsi ini bertujuan untuk menghapus sebuah berkas. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `fuse_unlink` menerima satu argumen: `path`, yang merupakan path dari berkas yang akan dihapus.

2. Fungsi memeriksa apakah pengguna saat ini memiliki hak akses untuk menghapus berkas tersebut menggunakan fungsi `cek_owner`. Jika tidak, maka fungsi mengembalikan kode kesalahan `-EACCES`, menunjukkan bahwa akses ditolak.

3. Path lengkap dari berkas yang akan dihapus dibuat menggunakan fungsi `snprintf`, dan disimpan dalam variabel `fpath`.

4. Fungsi `unlink` dipanggil dengan path lengkap dari berkas tersebut untuk menghapusnya.

5. Jika penghapusan berkas gagal, fungsi mengembalikan kode kesalahan.

6. Setelah berhasil menghapus berkas, fungsi mengembalikan nilai 0 untuk menandakan bahwa penghapusan telah berhasil.

Jadi, secara sederhana, fungsi ini bertanggung jawab untuk menghapus sebuah berkas, dengan memastikan bahwa pengguna yang memanggil fungsi memiliki hak akses yang sesuai.

```
#define fuse_IOCTL_COPY _IOWR('m', 1, char[2][PATH_MAX])



static int fuse_cp(const char *src, const char *dest) {



    if (!cek_owner(src) || !cek_owner(dest))

        return -EACCES;

     

    char full_src[PATH_MAX];

    char full_dest[PATH_MAX];



    

    snprintf(full_src, sizeof(full_src), "%s%s", basepath, src);

    snprintf(full_dest, sizeof(full_dest), "%s%s", basepath, dest);



    int src_fd = open(full_src, O_RDONLY);

    if (src_fd == -1) {

        return -errno;

    }



    int dest_fd = open(full_dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (dest_fd == -1) {

        close(src_fd);

        return -errno;

    }

    

    char buf[4096];

    ssize_t bytes_read, bytes_written;

    while ((bytes_read = read(src_fd, buf, sizeof(buf))) > 0) {

        bytes_written = write(dest_fd, buf, bytes_read);

        if (bytes_written != bytes_read) {

            close(src_fd);

            close(dest_fd);

            return -errno;

        }

    }



    close(src_fd);

    close(dest_fd);



    if (bytes_read == -1) {

        return -errno;

    }



    return 0;

}
```
Fungsi ini digunakan untuk menyalin isi dari sebuah berkas ke berkas lainnya. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi `fuse_cp` menerima dua argumen: `src` (path berkas sumber) dan `dest` (path berkas tujuan).

2. Fungsi memeriksa apakah pengguna saat ini memiliki hak akses untuk mengakses kedua berkas tersebut menggunakan fungsi `cek_owner`. Jika tidak, fungsi mengembalikan kode kesalahan `-EACCES`, menunjukkan bahwa akses ditolak.

3. Path lengkap dari berkas sumber dan berkas tujuan dibuat dengan menggabungkan path dasar (`basepath`) dengan path yang diberikan.

4. Fungsi membuka berkas sumber dengan mode baca (`O_RDONLY`) dan berkas tujuan dengan mode tulis dan buat (`O_WRONLY | O_CREAT | O_TRUNC`). Mode ini mengindikasikan bahwa berkas tujuan akan ditimpa jika sudah ada atau dibuat jika belum ada.

5. Fungsi membaca data dari berkas sumber dan menuliskannya ke berkas tujuan dalam potongan-potongan sebesar 4096 byte menggunakan buffer `buf`.

6. Fungsi menutup kedua berkas setelah selesai proses penyalinan.

7. Jika terjadi kesalahan selama proses penyalinan, fungsi mengembalikan kode kesalahan.

8. Jika penyalinan berhasil, fungsi mengembalikan nilai 0 untuk menandakan bahwa penyalinan telah berhasil.

Jadi, secara sederhana, fungsi ini bertanggung jawab untuk menyalin isi dari sebuah berkas ke berkas lainnya, dengan memastikan bahwa pengguna memiliki hak akses yang sesuai terhadap kedua berkas tersebut.

```
static int fuse_ioctl(const char *path, int cmd, void *arg, struct fuse_file_info *fi, unsigned int flags, void *data) {

    if (cmd == fuse_IOCTL_COPY) {

        char (*paths)[PATH_MAX] = arg;

        return fuse_cp(paths[0], paths[1]);

    }

    return -ENOTTY;

}
```
Fungsi `fuse_ioctl` bertujuan untuk menangani operasi I/O khusus yang tidak tertutup oleh fungsi-fungsi standar dalam FUSE. Berikut adalah penjelasan sederhana dari fungsi tersebut:

1. Fungsi menerima beberapa argumen, di antaranya adalah `path` (path dari berkas yang terlibat dalam operasi I/O), `cmd` (perintah ioctl yang ingin dieksekusi), `arg` (argumen untuk perintah ioctl), dan lain-lain.

2. Fungsi memeriksa apakah perintah ioctl yang diterima adalah `fuse_IOCTL_COPY`, yang merupakan perintah khusus untuk menyalin berkas.

3. Jika perintah ioctl yang diterima adalah `fuse_IOCTL_COPY`, fungsi mengekstrak path berkas sumber dan path berkas tujuan dari argumen `arg`.

4. Fungsi memanggil fungsi `fuse_cp` untuk menyalin berkas dari path sumber ke path tujuan.

5. Jika operasi penyalinan berhasil, fungsi mengembalikan nilai 0 untuk menandakan keberhasilan. Jika gagal, fungsi mengembalikan kode kesalahan yang sesuai.

6. Jika perintah ioctl yang diterima bukanlah `fuse_IOCTL_COPY`, fungsi mengembalikan kode kesalahan `-ENOTTY`, menunjukkan bahwa perintah ioctl tersebut tidak didukung.

Jadi, secara sederhana, fungsi ini bertanggung jawab untuk menangani operasi I/O khusus dalam FUSE, dan jika perintahnya adalah menyalin berkas, maka fungsi akan memanggil fungsi khusus `fuse_cp` untuk menyalin berkas tersebut.

```
static struct fuse_operations fuse_oper = {

    .getattr    = fuse_getattr,

    .read       = fuse_read,

    .readdir    = fuse_readdir,

    .mkdir      = fuse_mkdir,

    .rename     = fuse_rename,

    .rmdir      = fuse_rmdir,

    .mknod      = fuse_mknod,

    .create     = fuse_create,

    .unlink     = fuse_unlink,

    .ioctl      = fuse_ioctl,

};
```
Ini adalah deklarasi struktur `fuse_operations` yang mendefinisikan operasi-operasi file system yang akan diimplementasikan dalam FUSE. Setiap anggota dari struktur ini merupakan pointer ke fungsi yang akan menangani operasi tersebut. Berikut adalah penjelasan sederhana dari setiap anggota:

1. `.getattr`: Fungsi untuk mendapatkan atribut (metadata) dari sebuah berkas atau direktori.
2. `.read`: Fungsi untuk membaca konten dari sebuah berkas.
3. `.readdir`: Fungsi untuk membaca isi dari sebuah direktori.
4. `.mkdir`: Fungsi untuk membuat direktori baru.
5. `.rename`: Fungsi untuk mengubah nama atau memindahkan sebuah berkas atau direktori.
6. `.rmdir`: Fungsi untuk menghapus sebuah direktori kosong.
7. `.mknod`: Fungsi untuk membuat sebuah berkas atau named pipe.
8. `.create`: Fungsi untuk membuat sebuah berkas baru.
9. `.unlink`: Fungsi untuk menghapus sebuah berkas.
10. `.ioctl`: Fungsi untuk menangani operasi I/O khusus.

Dengan mendefinisikan operasi-operasi tersebut, FUSE dapat memanggil fungsi-fungsi yang sesuai ketika ada permintaan operasi tertentu terhadap sistem file yang di-mount.

```
void setup() {

    const char* dir[] = { "andi", "budi", "cony", "deni" };

    const char* users[] = { "andi", "budi", "cony", "deni" };

    char uid_cmd[128];

    char uid_str[16];

    char cmd[PATH_MAX];

    FILE *fp;



    for (int i = 0; i < 4; i++) {

        snprintf(uid_cmd, sizeof(uid_cmd), "id -u %s", users[i]);

        fp = popen(uid_cmd, "r");

        if (fp == NULL) {

            continue;

        }

        if (fgets(uid_str, sizeof(uid_str), fp) != NULL) {

            strtok(uid_str, "\n");

            snprintf(cmd, sizeof(cmd), "sudo chown -R %s:%s %s/%s", users[i], users[i], basepath, dir[i]);

            system(cmd);

            snprintf(cmd, sizeof(cmd), "sudo chmod -R 777 %s/%s", basepath, dir[i]);

            system(cmd);

        }

        pclose(fp);

    }

}
```
Fungsi `setup` digunakan untuk menyiapkan lingkungan kerja. Di dalamnya, dilakukan pengaturan hak akses (`chmod`) dan kepemilikan (`chown`) pada beberapa direktori di jalur kerja yang ditentukan sebelumnya. Langkah-langkahnya adalah sebagai berikut:

1. Sebuah array `dir` dan `users` diinisialisasi dengan nama direktori dan nama pengguna yang akan digunakan untuk menetapkan hak akses dan kepemilikan.
2. Dilakukan iterasi melalui array `users`.
3. Untuk setiap pengguna, eksekusi perintah `id -u [nama_pengguna]` melalui `popen` untuk mendapatkan ID pengguna.
4. Jika perintah berhasil dieksekusi, baca keluaran perintah tersebut dengan `fgets` dan hapus karakter newline (`\n`) dengan `strtok`.
5. Gunakan informasi ID pengguna dan nama pengguna untuk menetapkan hak akses dan kepemilikan pada direktori terkait dengan menggunakan `chown` dan `chmod` melalui `system`.
6. Tutup file pointer dengan `pclose`.

Dengan demikian, fungsi ini bertanggung jawab untuk menyiapkan lingkungan kerja dengan hak akses dan kepemilikan yang sesuai pada direktori-direktori yang telah ditentukan.

```
int main(int argc, char *argv[]) {

    umask(0);

    setup();

    return fuse_main(argc, argv, &fuse_oper, NULL);

}
```
Fungsi `main` merupakan titik masuk utama program. Di dalamnya, dilakukan beberapa langkah:

1. `umask(0)`: Mengatur nilai `umask` ke 0, yang berarti izin (permission) yang diberikan secara default untuk berkas yang baru dibuat akan maksimal.
2. `setup()`: Memanggil fungsi `setup()` untuk menyiapkan lingkungan kerja.
3. `return fuse_main(argc, argv, &fuse_oper, NULL)`: Memanggil fungsi `fuse_main` yang merupakan fungsi utama dari FUSE. Fungsi ini akan memulai file system FUSE dengan menggunakan operasi yang telah ditentukan sebelumnya dalam `fuse_oper`. Parameter `argc` dan `argv` digunakan untuk meneruskan argumen dari command line, sedangkan `NULL` digunakan untuk meneruskan pointer yang tidak digunakan.

Dengan demikian, fungsi `main` bertanggung jawab untuk menginisialisasi file system FUSE dengan operasi-operasi yang telah ditentukan sebelumnya dan memulai eksekusi program.

Bukti Screenshot:

![212](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/5cc8711a-f8b3-41e3-89e4-fd41b947b541)
![222](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/43910243-9731-4b46-ae56-289242a57905)
![nv](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/cb0613f7-3aa1-48df-a265-fe6d209ff175)
![2225](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/a08d1372-1a03-46d8-8de5-9a70cea7c4c3)
![22224](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/d76c5357-b3a1-448e-a2ce-81deecd0f070)
![22](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/3c691f20-d80d-4676-be64-642f468d209e)
![21](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/162027180/2187cc39-a249-467d-8207-35dd0f22e123)


Kendala yang dialami:
- Kesulitan dalam membuat versi yang tidak memerlukan permission 777 ke semua directory (saya lebih suka jika bisa tidak pakai 777)
- Kesulitan dalam menerapkan fungsi chown (error Operation not permitted padahal sudah pakai permission 777)
  
- [Task 3 - MengOS](/task-3/) by Fernando
#### Soal
MengOS

Nori adalah seorang mahasiswa yang sedang belajar membuat sistem operasi. Ia ingin membuat sebuah sistem operasi sederhana yang dapat menampilkan dan membaca string dari pengguna. Karena Nori sedang pada fase malas, dia meminta bantuan Anda untuk membuat program tersebut. Bantulah Nori untuk menyelesaikan tugasnya!

### Struktur Direktori

```
task-3/
├── bin/
├── include/
│   ├── kernel.h
│   ├── std_lib.h
│   └── std_type.h
├── src/
│   ├── bootloader.asm
│   ├── kernel.asm
│   ├── kernel.c
│   └── std_lib.c
├── bochsrc.txt
└── makefile
```

- [`bin/`](./bin/) : menyimpan file hasil kompilasi
- [`include/`](./include/) : berisi kode file header
- [`src/`](./src/) : berisi kode sumber
- [`bochsrc.txt`](./bochsrc.txt) : konfigurasi Bochs
- [`makefile`](./makefile) : file konfigurasi make

### Kernel

#### [`kernel.asm`](./src/kernel.asm)

```asm
global _putInMemory
global _interrupt

; void putInMemory(int segment, int address, char character)
_putInMemory:
	push bp
	mov bp,sp
	push ds
	mov ax,[bp+4]
	mov si,[bp+6]
	mov cl,[bp+8]
	mov ds,ax
	mov [si],cl
	pop ds
	pop bp
	ret

; int interrupt(int number, int AX, int BX, int CX, int DX)
_interrupt:
	push bp
	mov bp,sp
	mov ax,[bp+4]
	push ds
	mov bx,cs
	mov ds,bx
	mov si,intr
	mov [si+1],al
	pop ds
	mov ax,[bp+6]
	mov bx,[bp+8]
	mov cx,[bp+10]
	mov dx,[bp+12]

intr:	int 0x00

	mov ah,0
	pop bp
	ret
```

- `_putInMemory` : fungsi untuk menulis karakter ke memori (penjelasan ada di modul)

- `_interrupt` : fungsi untuk melakukan interrupt

  - `number` : nomor vektor interrupt
  - `AX`, `BX`, `CX`, `DX` : register yang akan diisi
  - Register `AX` merupakan gabungan dari `AH` dan `AL`
  - Register `BX` merupakan gabungan dari `BH` dan `BL`
  - Register `CX` merupakan gabungan dari `CH` dan `CL`
  - Register `DX` merupakan gabungan dari `DH` dan `DL`
  - Untuk menggabungkan dua regester tipe `H` dan `L` menjadi `X` dapat menggunakan salah satu dari cara berikut.
    ```c
    AX = AH << 8 | AL // cara pertama
    AX = AH * 256 + AL // cara kedua
    ```

#### [`kernel.c`](./src/kernel.c)

```c
#include "kernel.h"
#include "std_lib.h"

int main() {
  char buf[128];
  clearScreen();
  printString("Welcome to MengOS\n");

  while (true) {
    printString("$ ");
    readString(buf);
    printString(buf);
    printString("\n");
  }
}
```

- Kernel akan menunggu input dari pengguna dan menampilkan input tersebut kembali ke layar selama sistem operasi berjalan.

- Dilarang mengubah isi dari fungsi `main`.

### File Header

#### [`std_type.h`](./include/std_type.h)

Header ini berisi definisi tipe data yang akan sering digunakan dalam program.

```c
typedef unsigned char byte;

typedef char bool;
#define true 1
#define false 0
```

- `byte` : tipe data unsigned char, digunakan untuk menyimpan angka dengan rentang 0-255 (`0x00`-`0xFF`)

- `bool` : tipe data boolean, digunakan untuk menyimpan nilai `true` atau `false`

  Tipe data ini dibuat karena tidak ada tipe data boolean di bahasa C.

#### [`std_lib.h`](./include/std_lib.h)

Header ini berisi deklarasi fungsi yang akan sering digunakan dalam program.

```c
int div(int a, int b);
int mod(int a, int b);

void memcpy(byte* src, byte* dst, unsigned int size);
unsigned int strlen(char* str);
bool strcmp(char* str1, char* str2);
void strcpy(char* src, char* dst);
void clear(byte* buf, unsigned int size);
```

- `div` : fungsi pembagian

- `mod` : fungsi modulo

  Fungsi `div` dan `mod` perlu dibuat karena tidak ada operator pembagian dan modulo pada bahasa assembly. Buatlah fungsi tersebut dengan menggunakan operator yang tersedia, seperti `+`, `-`, `*`, `<<`, dan `>>`.

- `memcpy` : fungsi untuk menyalin memori

- `strlen` : fungsi untuk menghitung panjang string

- `strcmp` : fungsi untuk membandingkan dua string

- `strcpy` : fungsi untuk menyalin string

- `clear` : fungsi untuk mengisi memori dengan nol

#### [`kernel.h`](./include/kernel.h)

Header ini berisi deklarasi fungsi yang memiliki implementasi pada `kernel.asm` dan akan diimplementasikan pada `kernel.c`.

```c
extern void putInMemory(int segment, int address, char character);
extern int interrupt(int number, int AX, int BX, int CX, int DX);

void printString(char* str);
void readString(char* buf);
void clearScreen();
```

- `putInMemory` : implementasi fungsi `_putInMemory` pada `kernel.asm`
- `interrupt` : implementasi fungsi `_interrupt` pada `kernel.asm`
- `printString` : fungsi untuk menampilkan string
- `readString` : fungsi untuk membaca string
- `clearScreen` : fungsi untuk membersihkan layar

### Makefile

```makefile
prepare:

bootloader:

stdlib:

kernel:

link:

build: prepare bootloader stdlib kernel link

run:
```

- `prepare` : membuat disk image baru `floppy.img` pada direktori `bin/` dengan ukuran 1.44 MB.

- `bootloader` : mengkompilasi `bootloader.asm` menjadi `bootloader.bin` pada direktori `bin/`.

- `stdlib` : mengkompilasi `std_lib.c` menjadi `std_lib.o` pada direktori `bin/`.

- `kernel` : mengkompilasi `kernel.c` menjadi `kernel.o` pada direktori `bin/` dan mengkompilasi `kernel.asm` menjadi `kernel_asm.o` pada direktori `bin/`.

- `link` : menggabungkan `bootloader.bin`, `kernel.o`, `kernel_asm.o`, dan `std_lib.o` menjadi `floppy.img`.

- `build` : menjalankan perintah `prepare`, `bootloader`, `stdlib`, `kernel`, dan `link`.

- `run` : menjalankan Bochs dengan konfigurasi `bochsrc.txt`.

### Referensi Perintah

#### `dd`

```bash
dd if=<input_file> of=<output_file> bs=<block_size> count=<block_count> seek=<seek_count> conv=<conversion>
```

#### `nasm`

```bash
nasm -f <format> <input_file> -o <output_file>
```

#### `bcc`

```bash
bcc -ansi -c <input_file> -o <output_file>
```

#### `ld86`

```bash
ld86 -o <output_file> -d [file1.o file2.o ...]
```

### Deskripsi Tugas

- a. Implementasikan fungsi `printString` pada `kernel.c` yang akan menampilkan string ke layar.

  Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 10h` dengan parameter `AH = 0x0E` untuk _teletype output_. Karakter yang ditampilkan dapat dimasukkan pada register `AL`. Fungsi ini akan menampilkan string karakter ASCII (_null-terminated_) ke layar.

- b. Implementasikan fungsi `readString` pada `kernel.c` yang akan membaca string dari keyboard.

  Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 16h` dengan parameter `AH = 0x00` untuk _keyboard input_. Fungsi ini akan membaca karakter ASCII (_non-control_) yang dimasukkan oleh pengguna dan menyimpannya pada buffer hingga menekan tombol `Enter`. Handle tombol `Backspace` dibebaskan kepada praktikan.

- c. Implementasikan fungsi `clearScreen` pada `kernel.c` yang akan membersihkan layar.

  Ukuran layar adalah `80x25` karakter. Setelah layar dibersihkan, kursor akan kembali ke posisi awal yaitu `(0, 0)` dan buffer video untuk warna karakter diubah ke warna putih. Implementasi dapat menggunakan fungsi `interrupt` dengan service `int 10h` atau menggunakan fungsi `putInMemory` untuk mengisi memori video.

- d. Lengkapi implementasi fungsi yang terdapat pada [`std_lib.h`](./include/std_lib.h) pada [`std_lib.c`](./src/std_lib.c).

  Fungsi-fungsi di atas dapat digunakan untuk mempermudah implementasi fungsi `printString`, `readString`, dan `clearScreen`.

- e. Buat automasi untuk melakukan kompilasi dengan melengkapi file [`makefile`](./makefile).

  Untuk melakukan kompilasi program, akan digunakan perintah `make build`. Semua hasil kompilasi program akan disimpan pada direktori [`bin/`](./bin). Untuk menjalankan program, akan digunakan perintah `make run`.

##### Jawaban
File makefile
``` makefile
prepare:
	dd if=/dev/zero of=bin/floppy.img bs=512 count=2880

bootloader:
	nasm -f bin src/bootloader.asm -o bin/bootloader.bin

stdlib:
	bcc -ansi -c src/std_lib.c -o bin/std_lib.o -Iinclude

kernel:
	bcc -ansi -c src/kernel.c -o bin/kernel.o -Iinclude
	nasm -f as86 src/kernel.asm -o bin/kernel-asm.o

link:
	dd if=bin/bootloader.bin of=bin/floppy.img bs=512 count=1 conv=notrunc
	ld86 -o bin/kernel.bin -d bin/kernel.o bin/kernel-asm.o bin/std_lib.o
	dd if=bin/kernel.bin of=bin/floppy.img bs=512 seek=1 conv=notrunc

build: prepare bootloader stdlib kernel link

run:
	bochs -f bochsrc.txt
```
File makefile berfungsi untuk mempermudah proses compilingnya, agar setiap ingin mengcompile program yang baru saja diganti, hanya diperlukan untuk menjalankan `make build` dan `make run`. Penjelasan untuk segment-segment nya adalah sebagai berikut :

prepare: Membuat file floppy.img yang tidak berisi sebanyak 512 blok 2880 kali, yang sebesar 1.44 MB.
bootloader: Mengcompile file bootloader.asm dengan NASM menjadi file bin
stdlib: Mengcompile file std_lib.c dengan BCC sambil menggunakan include yang dibutuhkan menjadi file object
kernel: Mengcompile file kernel.c dengan BCC sambil menggunakan include yang dibutuhkan menjadi file object dan mengcompile file kernel.asm dengan NASM menjadi file object
link: Mengisi file floppy.img dengan bootloader.bin agar floppy.img dapat diload, lalu menggabungkan kedua file kernel.o dan kernel-asm.o karena kernel.o membutuhkan fungsi yang ada di kernel-asm.o, setelah itu file floppy.img diappend dengan file kernel.bin yang hasil gabungan dari kernel C dan Assembly.


File std_lib.c
``` c
#include "std_lib.h"

#define INT_MIN -2147483648
#define INT_MAX 2147483647

int div(int a, int b) {
    int sign, quo;
    if (b == 0) return INT_MAX;
    
    sign = ((a < 0) ^ (b < 0)) ? -1 : 1;

    a = (a < 0 ? -a : a);
    b = (b < 0 ? -b : b);

    quo = 0;
    while (a >= b) {
        int temp = b, cnt = 1;
        while (temp <= (b >> 1)) {
            temp <<= 1;
            cnt <<= 1;
        }
        a -= temp;
        quo += cnt;
    }

    return sign * quo;
}

int mod(int a, int b) {
    if (b == 0) return INT_MAX;

    a = (a < 0 ? -a : a);
    b = (b < 0 ? -b : b);

    while (a >= b) {
        int temp = b;
        while (temp <= (b >> 1)) {
            temp <<= 1;
        }
        a -= temp;
    }

    return a;
}

void memcpy(byte* src, byte* dst, unsigned int size) {
    int i;
    for (i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

unsigned int strlen(char *str) {
    unsigned int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

bool strcmp(char* str1, char* str2) {
    int i;
    unsigned int len1 = strlen(str1);
    unsigned int len2 = strlen(str2);
    if (len1 != len2) {
        return false;
    }
    for (i = 0; i < len1; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

void strcpy(char* src, char* dst) {
    unsigned int len = strlen(src);
    int i;
    for (i = 0; i < len; i++) {
        dst[i] = src[i];
    }
    dst[len] = '\0';
}

void clear(byte* buf, unsigned int size) {
    int i;
    for (i = 0; i < size; i++) {
        buf[i] = 0;
    }
}
```
File std_lib.c berfungsi sebagai library untuk membantu pengerjaan kernel.c

div: Membagi dua integer menggunakan metode bits manipulation, dan mengeluarkan hasil baginya.

mod: Modular dari dua integer menggunakan metode bits manipulation, dan mengeluarkan hasil sisa bagi dari kedua integer tersebut.

memcpy: Mengisi isi unsigned char* dst, dengan isi src.

strcpy: Mengisi isi char* dst, dengan isi src.

strcmp: Membandingkan dua char* dan mengeluarkan true jika sama, false jika beda.

strlen: Mengeluarkan panjang char* yang diberikan sebagai argumen.

clear: Mengosongkan isi unsigned char* buf sepanjang size.


File kernel.c
``` c
#include "kernel.h"
#include "std_lib.h"

int main() {
  char buf[128];
  clearScreen();
  printString("Welcome to MengOS\n");

  while (true) {
    printString("$ ");
    readString(buf);
    printString(buf);
    printString("\n");
  }
}

void printString(char* str) {
  interrupt(0x10, 0xE00 + '\r', 0, 0, 0);
  while (*str) {
    interrupt(0x10, 0xE00 + *str++, 0, 0, 0);
  }
}

void readString(char *buf) {
  int i = 0;
  int input;
  char ah, al;
  input = interrupt(0x16, 0, 0, 0, 0);
  ah = input >> 8;
  al = input & 0x00FF;
  while (al != 0xD && ah != 0x48 && ah != 0x50) {
    if (al != 0x8) {
      buf[i++] = al;
      interrupt(0x10, 0xE00 + al, 0, 0, 0);
    } else if (i > 0) {
      interrupt(0x10, 0xE00 + '\b', 0xF, 0, 0);
      interrupt(0x10, 0xE00 + ' ', 0xF, 0, 0);
      interrupt(0x10, 0xE00 + '\b', 0xF, 0, 0);
      buf[--i] = 0;
    }
    input = interrupt(0x16, 0, 0, 0, 0);
    ah = input >> 8;
    al = input & 0x00FF;
  }
  interrupt(0x10, 0xE00 + '\n', 0xF, 0, 0);
  interrupt(0x10, 0xE00 + '\r', 0xF, 0, 0);
  buf[i] = 0;
}

void clearScreen() {
  int i;
  interrupt(0x10, 0x02 << 8, 0, 0, 0);
  for (i = 0; i < 80 * 25; i++) {
    putInMemory(0xB000, 0x8000 + i * 2, ' ');
    putInMemory(0xB000, 0x8001 + i * 2, 0x0F);
  }
}
```
File kernel.c adalah *Bread and Butter* dari program ini, yang mengproses input dari user dan mengeluarkan hasilnya.

printString: Mengeluarkan isi char* ke `MengOS` menggunakan service `10h`.

readString: Membaca input menggunakan service `16h`, dan selama input bukan `enter`, akan menjadi satu string buffer. Fungsi ini juga memproses `backspace`, jadi jika ada kesalahan mengetik, dapat memperbaiki kesalahannya. Setelah input diterima, maka inputnya diprint ke `MengOS` agar user bisa melihat apa yang diinputkan.

clearScreen: Memindahkan kursor ke atas kiri menggunakan service `10h` lalu mengubah satu layar dengan resolusi 80x25 dengan mengubah memori di segment `0xB000` menjadi spasi, yang hasil akhirnya menjadi layar yang kosong.

#### Kendala saat mengerjakan
Masalah utama saat mengerjakan soal ini ialah bochsnya itu sendiri. Banyak teman-teman saya yang mengalami kendala hanya dikarenakan mereka menginstall bochs melalui `sudo apt install bochs`, yang versi bochs nya agak terkutuk, karena saat saya arahkan untuk menginstall menggunakan file .rpm, kode mereka langsung bisa.

Satu lagi, namun ini sebenarnya karena 

![image](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/79700258/33bd02b6-2e22-42a6-99d9-45416a4802ca)


Fungsi main di kernel.c **HARUS DI ATAS**, karena mengcompile C nya memakai BCC, berbeda dengan GCC yang biasa digunakan. Setelah diskusi dengan teman saya, rupanya BCC itu selalu mengasumsi fungsi yang paling di atas adalah sebagai fungsi `main`. Setelah saya letakkan di tempat yang sesuai, langsung jalan :DDD

#### Bukti Jawaban
![image](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/79700258/7a24d578-7038-4f37-9d80-140416262de1)
![image](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/79700258/63057675-a910-4590-8ede-081b10e5c519)
![image](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/79700258/46d105c8-391c-4b6b-a19d-7008e3d5475f)
![image](https://github.com/sisop-its-s24/praktikum-modul-4-b24/assets/79700258/39cf9fa9-c95c-4497-b07a-6c794cb9952a)


