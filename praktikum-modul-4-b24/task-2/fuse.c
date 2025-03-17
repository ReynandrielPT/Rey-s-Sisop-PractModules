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

