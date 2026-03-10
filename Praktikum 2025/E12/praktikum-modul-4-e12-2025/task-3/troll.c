#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct{
    const char *name;
    const char *user_other_content;
    const char *user_dain_content;
    const char * leaked;
} troll_file;

const char *ascii_troll() {
    static char buf[1024];
    FILE *f = fopen("troll.txt", "r");
    if (f) {
        size_t len = fread(buf, 1, sizeof(buf)-1, f);
        buf[len] = '\0';
        fclose(f);
        return buf;
    }
    return "Fell for it again reward\n";
}
static troll_file files_name[] = {
    {"very_spicy_info.txt","DainTontas' personal secret!!.txt\n","Very spicy internal developer information: leaked roadmap.docx\n",
                                NULL},
                                {"upload.txt","","",NULL}
};
static int trapped(){
    FILE *f = fopen("/tmp/troll_trap_status","r");
    if(f){
        int status = 0;
        fscanf(f,"%d",&status);fclose(f);
        return status;
    }
    return 0;
}
static void write_status(int status){
    FILE *f = fopen("/tmp/troll_trap_status","w");
    if(f){
        fprintf(f,"%d",status);
        fclose(f);
    }
}
static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    memset(stbuf,0,sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = __S_IFDIR | 0777;
        stbuf->st_nlink = 2;
        return 0;
    }
    if(strcmp(path+1, "upload.txt") == 0) {
        stbuf->st_uid = 1001; 
        stbuf->st_gid = 1001;
        stbuf->st_mode = 0666; 
    }
    for(size_t i = 0;i<sizeof(files_name)/sizeof(files_name[0]);i++){
        if(strcmp(path+1,files_name[i].name)==0){
            stbuf->st_mode = __S_IFREG | 0644;
            stbuf->st_nlink = 1;
            if (trapped()) {
                stbuf->st_size = strlen(files_name[i].leaked);
            } else {
                if (strcmp(files_name[i].name, "very_spicy_info.txt") == 0) {
                    stbuf->st_size = strlen(files_name[i].user_other_content);
                } else {
                    stbuf->st_size = strlen(files_name[i].user_dain_content);
                }
            }
        return 0;
        }
    }
    return -ENOENT;
}



static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{

    (void) offset;
    (void) fi;

    if(strcmp(path,"/")!=0) return -ENOENT;
    
    filler(buf,".",NULL,0);
    filler(buf,"..",NULL,0);

    for(size_t i=0;i<sizeof(files_name)/sizeof(files_name[0]);i++){
        filler(buf,files_name[i].name,NULL,0);
    }
    return 0;
}



static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    for(size_t i=0;i<sizeof(files_name)/sizeof(files_name[0]);i++){
        if(strcmp(path+1,files_name[i].name)==0){
            const char *content;
            if(trapped()){
                if (files_name[i].leaked) {
                    content = files_name[i].leaked;
                } else {
                    content = ascii_troll();
                }
            }
            else if(strcmp(files_name[i].name,"very_spicy_info.txt")==0){
                struct passwd *pw = getpwuid(fuse_get_context()->uid);
                if (pw && strcmp(pw->pw_name, "DainTontas") == 0) {
                    content = files_name[i].user_dain_content;
                } else {
                    content = files_name[i].user_other_content;
                }
            }
            else{
                content = files_name[i].user_other_content;
            }
            size_t len = strlen(content);
            if(offset <len){
                if(offset+size>len) size=len-offset;
                memcpy(buf,content+offset,size);
            }
            else size=0;
            
            if(strcmp(files_name[i].name,"upload.txt")==0 && !trapped()) write_status(1);
            return size;
        }
    }
    return -ENOENT;
}

static int xmp_write(const char *path, const char *buf, size_t size,off_t offset,struct fuse_file_info *fi){
    (void) offset;
    (void) fi;

    for(size_t i=0;i<sizeof(files_name)/sizeof(files_name[0]);i++){
        if(strcmp(path+1,files_name[i].name)==0){
            if(strcmp(files_name[i].name,"upload.txt")==0 && !trapped() && strstr(buf,"upload") != NULL) write_status(1);                
            return size;
        }
    }
    return -ENOENT;
}

static int xmp_chmod(const char *path, mode_t mode) {
    (void)path; (void)mode;
    return 0;
}


static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
    .write = xmp_write,
    .chmod = xmp_chmod,
};



int  main(int  argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
