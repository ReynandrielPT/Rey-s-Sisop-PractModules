#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <b64/cencode.h>

#define path_ba "/home/sakuya/FS++"
#define CONFIG_FILE "/home/sakuya/FS++/lawak.conf"

char SECRET_FILE_BASENAME[256] = "secret";
int ACCESS_START_HOUR = 8, ACCESS_END_HOUR = 18;
char FILTER_WORDS[1024] = "";

void load_config() {
    FILE *config = fopen(CONFIG_FILE, "r");
    if (config == NULL) {
        perror("Unable to open config file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), config)) {
        if (strncmp(line, "SECRET_FILE_BASENAME=", 21) == 0) {
            sscanf(line + 21, "%s", SECRET_FILE_BASENAME);
        } else if (strncmp(line, "ACCESS_START=", 13) == 0) {
            sscanf(line + 13, "%d", &ACCESS_START_HOUR);
        } else if (strncmp(line, "ACCESS_END=", 11) == 0) {
            sscanf(line + 11, "%d", &ACCESS_END_HOUR);
        } else if (strncmp(line, "FILTER_WORDS=", 13) == 0) {
            sscanf(line + 13, "%s", FILTER_WORDS);
        }
    }

    fclose(config);
}

char *strcasestr(const char *haystack, const char *needle) {
    while (*haystack) {
        if (strcasecmp(haystack, needle) == 0) {
            return (char *)haystack;
        }
        haystack++;
    }
    return NULL;
}

void filter_text(char *buf, size_t size) {
    char *lawak_words[] = {"ducati", "ferrari", "mu", "chelsea", "prx", "onic", "sisop"};
    for (int i = 0; i < sizeof(lawak_words) / sizeof(lawak_words[0]); i++) {
        char *pos;
        while ((pos = strcasestr(buf, lawak_words[i])) != NULL) {
            memset(pos, 'l', strlen(lawak_words[i]));
        }
    }
}

void fullPth(char fpath[4100], const char *path) {
    snprintf(fpath, 4100, "%s%s", path_ba, path);
}

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

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    char fpath[4100];
    fullPth(fpath, path);
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);
    if (dp == NULL) return -errno;

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

    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    char fpath[4100];
    fullPth(fpath, path);
    int res;
    (void) fi;
    
    int fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno; 

    res = pread(fd, buf, size, offset);
    if (res == -1) res = -errno;

    if (strstr(path, ".txt")) {
        filter_text(buf, res);
    }

    else if (strstr(path, ".bin")) {
        char encoded[2048];
        
        memcpy(buf, encoded, strlen(encoded));
        res = strlen(encoded);
    }

    close(fd);
    return res;
}

static int xmp_open(const char *path, struct fuse_file_info *fi) {
    char fpath[4100];
    fullPth(fpath, path);
    int fd = open(fpath, O_RDONLY);
    if (fd == -1) return -errno; 
    close(fd);
    return 0;
}

static int xmp_access(const char *path, int mask) {
    char fpath[4100];
    fullPth(fpath, path);
    
    if (strstr(path, SECRET_FILE_BASENAME)) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        if (tm.tm_hour < ACCESS_START_HOUR || tm.tm_hour >= ACCESS_END_HOUR) {
            return -ENOENT;
        }
    }

    int res = access(fpath, mask);
    if (res == -1) return -errno; 
    return 0;
}

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

static int gabole() {
    return -EROFS;
}

static struct fuse_operations xmp_oper = {
    .getattr  = xmp_getattr,
    .readdir  = xmp_readdir,
    .read     = xmp_read,
    .open     = xmp_open,
    .access   = xmp_access,
    .write    = gabole,
    .truncate = gabole,
    .create   = gabole,
    .unlink   = gabole,
    .mkdir    = gabole,
    .rmdir    = gabole,
    .rename   = gabole,
};

int main(int argc, char *argv[]) {
    umask(0);
    load_config();
    return fuse_main(argc, argv, &xmp_oper, NULL);
}


// ╱|、
// (˚ˎ 。7  
//  |、˜〵          
// じしˍ,)ノ

