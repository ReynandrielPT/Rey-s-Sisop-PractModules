#define FUSE_USE_VERSION 28
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



int  main(int  argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
