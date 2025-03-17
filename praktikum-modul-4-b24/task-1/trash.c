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
