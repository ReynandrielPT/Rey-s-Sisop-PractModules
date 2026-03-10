#define FUSE_USE_VERSION 28

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <libgen.h>
#include <limits.h>
#include <ctype.h>

#define MAX_FILTER_WORDS 100

static const char *source_dir = "/home/rdtzaaa/task-2/testing";
static char *filter_words[MAX_FILTER_WORDS];
static int filter_word_count = 0;
static char secret_key[256] = "secret";
static int access_start_hour = 8;
static int access_end_hour = 18;

void trim(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

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

static void fullpath(char fpath[PATH_MAX], const char *path) {
    snprintf(fpath, PATH_MAX, "%s%s", source_dir, path);
}

static int complete_name(const char *dirname, const char *basename, char *realname) {
    DIR *dp = opendir(dirname);
    if (!dp) return -1;

    struct dirent *de;
    while ((de = readdir(dp)) != NULL) {
        if (strncmp(de->d_name, basename, strlen(basename)) == 0 && de->d_name[strlen(basename)] == '.') {
            strcpy(realname, de->d_name);
            closedir(dp);
            return 0;
        }
    }

    closedir(dp);
    return -1;
}

void to_lower_str(char *str) {
    for (; *str; ++str) *str = tolower(*str);
}

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

static int xmp_access(const char *path, int mask) 
{
    if (secret_basename(path) && !is_work_hours()) return -ENOENT;

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

    int res = access(fpath, mask);
    if (res == 0) {
        log_access("ACCESS", path);
        return 0;
    } else {
        return -errno;
    }
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

static int xmp_release(const char *path, struct fuse_file_info *fi) {
    close(fi->fh);
    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {return -EROFS;}
static int xmp_truncate(const char *path, off_t size) {return -EROFS;}
static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi) {return -EROFS;}
static int xmp_unlink(const char *path) {return -EROFS;}
static int xmp_mkdir(const char *path, mode_t mode) {return -EROFS;};
static int xmp_rmdir(const char *path) {return -EROFS;}
static int xmp_rename(const char *from, const char *to) {return -EROFS;}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .access = xmp_access,
    .open    = xmp_open,
    .read    = xmp_read,
    .release = xmp_release,
    .readdir = xmp_readdir,

    .write    = xmp_write,
    .truncate = xmp_truncate,
    .create   = xmp_create,
    .unlink   = xmp_unlink,
    .mkdir    = xmp_mkdir,
    .rmdir    = xmp_rmdir,
    .rename   = xmp_rename,
};


int main(int argc, char *argv[]) {
    umask(0);

    if (init_conf("/home/rdtzaaa/task-2/lawak.conf") != 0) {
        printf("Gagal membaca konfigurasi lawak.conf\n");
        return 1;
    }

    return fuse_main(argc, argv, &xmp_oper, NULL);
}

// gcc -Wall `pkg-config fuse --cflags` fuse.c -o output `pkg-config fuse --libs`
