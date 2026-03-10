#define FUSE_USE_VERSION 28
#define BASE_PATH "/mnt/troll"
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <pwd.h>
#include <stdbool.h>

const char* timpa_teks() {

    return "_____    _ _    __              _ _                       _                                        _ \n"
    "|  ___|__| | |  / _| ___  _ __  (_) |_    __ _  __ _  __ _(_)_ __    _ __ _____      ____ _ _ __ __| |\n"
    "| |_ / _ \\ | | | |_ / _ \\| '__| | | __|  / _` |/ _` |/ _` | | '_ \\  | '__/ _ \\ \\ /\\ / / _` | '__/ _` |\n"
    "|  _|  __/ | | |  _| (_) | |    | | |_  | (_| | (_| | (_| | | | | | | | |  __/\\ V  V / (_| | | | (_| |\n"
    "|_|  \\___|_|_| |_|  \\___/|_|    |_|\\__|  \\__,_|\\__, |\\__,_|_|_| |_| |_|  \\___| \\_/\\_/ \\__,_|_|  \\__ _|\n"
    "                                               |___/                                                  \n";
}

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
            else  return 0;
        }
    }
    
    fd = open(realpath, O_RDONLY);
    if (fd == -1) return -errno;
    res = pread(fd, buf, size, offset);
    if (res == -1) res = -errno;
    close(fd);
    return res;
}

static int xmp_write (const char *path, const char *buff, size_t size, off_t offset, struct fuse_file_info *fi){
    int fd;
    int res;
    (void) fi; 

    char realpath[2000];
    strcpy(realpath, BASE_PATH);
    strcat(realpath, path);

    fd = open(realpath, O_WRONLY);

    if (fd == -1) res = -errno;

    res = pwrite(fd, buff, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}

static int xmp_truncate(const char *path, off_t size)
{
    char realpath[2000];
    strcpy(realpath, BASE_PATH);
    strcat(realpath, path);

    int res = truncate(realpath, size);
    if (res == -1)
        return -errno;

    return 0;
}


static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .write = xmp_write,
    .truncate = xmp_truncate
};



int  main(int  argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
