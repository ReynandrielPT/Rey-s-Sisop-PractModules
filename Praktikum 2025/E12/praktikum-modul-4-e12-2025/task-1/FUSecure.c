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
