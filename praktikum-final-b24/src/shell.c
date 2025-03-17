#include "shell.h"
#include "kernel.h"
#include "std_lib.h"
#include "filesystem.h"

void shell() {
  char buf[64];
  char cmd[64];
  char arg[2][64];
  byte cwd = FS_NODE_P_ROOT;

  while (true) {
    printString("MengOS:");
    printCWD(cwd);
    printString("$ ");
    readString(buf);
    parseCommand(buf, cmd, arg);


    if (strcmp(cmd, "cd")) cd(&cwd, arg[0]);
    else if (strcmp(cmd, "ls")) ls(cwd, arg[0]);
    else if (strcmp(cmd, "mv")) mv(cwd, arg[0], arg[1]);
    else if (strcmp(cmd, "cp")) cp(cwd, arg[0], arg[1]);
    else if (strcmp(cmd, "cat")) cat(cwd, arg[0]);
    else if (strcmp(cmd, "mkdir")) mkdir(cwd, arg[0]);
    else if (strcmp(cmd, "clear")) clearScreen();
    else printString("Invalid command\n");
  }
}

void printCWD(byte cwd) {
    struct node_fs node_fs_buf;
    byte wdpath[64];
    int wdlen;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    wdlen = 0;

    while (cwd != FS_NODE_P_ROOT) {
        wdpath[wdlen++] = cwd;
        cwd = node_fs_buf.nodes[cwd].parent_index;
    }
    wdpath[wdlen] = 0xFF;

    for (; wdlen >= 0; wdlen--) {
        if (wdpath[wdlen] == 0xFF) {
            printString("/");
        } else {
            printString(node_fs_buf.nodes[wdpath[wdlen]].node_name);
            printString("/");
        }
    }
}



void parseCommand(char* buf, char* cmd, char arg[2][64]) {
  int i;
  int j;
  int k;

  strcpy(arg[0], "");
  strcpy(arg[1], "");

  for (i = 0; buf[i] != ' ' && buf[i] != '\0'; i++) cmd[i] = buf[i];
  cmd[i] = '\0';
  if (buf[i] == '\0') return;

  i++;
  for (j = 0; buf[i] != ' ' && buf[i] != '\0'; i++, j++) arg[0][j] = buf[i];
  arg[0][j] = '\0';
  if (buf[j] == '\0') return;

  i++;
  for (k = 0; buf[i] != ' ' && buf[i] != '\0'; i++, k++) arg[1][k] = buf[i];
  arg[1][k] = '\0';
}

void cd(byte* cwd, char* dirname) {
    struct node_fs node_fs_buf;
    int i;
    bool found;
    int slash_pos_index;
    int dir_len;
    bool starts_with_double_dot_slash;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER);

    if (strcmp(dirname, "..") == true) {
        if (*cwd != FS_NODE_SECTOR_NUMBER) {
            *cwd = node_fs_buf.nodes[*cwd].parent_index;
        }
        return;
    }

    if (strcmp(dirname, "/") == true) {
        *cwd = FS_NODE_P_ROOT;
        return;
    }

    slash_pos_index = -1;
    dir_len = strlen(dirname);
    for (i = 0; i < dir_len; i++) {
        if (dirname[i] == '/') {
            slash_pos_index = i;
            break;
        }
    }

    starts_with_double_dot_slash = (dir_len > 2 && dirname[0] == '.' && dirname[1] == '.' && dirname[2] == '/');
    found = false;

    if (slash_pos_index != -1 && starts_with_double_dot_slash) {
        if (*cwd != FS_NODE_SECTOR_NUMBER) {
            *cwd = node_fs_buf.nodes[*cwd].parent_index;
            found = true;
            dirname = dirname + slash_pos_index + 1;

        }
        found = false;
    }

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (strcmp(node_fs_buf.nodes[i].node_name, dirname) == true && node_fs_buf.nodes[i].parent_index == *cwd && node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
            *cwd = i;
            found = true;
            break;
        }
        found = false;
    }

    if (!found) {
        printString("Directory not found\n");

    }

}



void ls(byte cwd, char* dirname) {
    struct node_fs node_fs_buf;
    byte dir;
    int i;
    bool found = false;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    if (!dirname[0] || strcmp(dirname, ".")) {
        dir = cwd;
        found = true;
    } else {
        for (i = 0; i < FS_MAX_NODE; ++i) {
            if (strcmp(node_fs_buf.nodes[i].node_name, dirname) && node_fs_buf.nodes[i].parent_index == cwd) {
                dir = i;
                found = true;
                break;
            }
        }
    }

    if (!found) {
        printString("Directory not found\n");
        return;
    }

    for (i = 0; i < FS_MAX_NODE; ++i) {
        if (node_fs_buf.nodes[i].parent_index == dir && node_fs_buf.nodes[i].node_name[0] != '\0') {
            printString(node_fs_buf.nodes[i].node_name);
            printString(" ");
        }
    }
    printString("\n");
}



void mv(byte cwd, char* src, char* dst) {
    struct node_fs node_fs_buf;
    enum fs_return status;
    bool src_found = false;
    bool dst_found = false;
    bool dst_which = false;
    byte src_index = cwd;
    byte dst_index = cwd;
    bool slash = false;
    int slash_pos_index=-1;
    char dst_str[512];
    char name_str[512];
    int i, j, k;
    int par_index;
    int dir_len = strlen(dst);
    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);




     for (i = 0; i < FS_MAX_NODE; i++) {
            if (strcmp(node_fs_buf.nodes[i].node_name, src)) {
                src_index = i;
                src_found = true;
                break;
            }
     }
     if(!src_found){
        printString("Source file not found\n");
            return;
     }

    if(node_fs_buf.nodes[src_index].data_index == FS_NODE_D_DIR){
        printString("Cannot move directory\n");
            return;
     }

     for (i = 0; i < dir_len; i++) {
        if (dst[i] == '/') {
            slash_pos_index = i;
            break;
        }
    }
    j = 0;
    k = 0;
    if(slash_pos_index!=-1){
    if(dst[0]=='.'&& dst[1]=='.'&&dst[2]=='/'){
        for(i = 3; i < strlen(dst); i++){
            name_str[k] = dst[i];
            k++;
        }
        dst_found=true;
    }else if(dst[0]=='/'){
        for(i = 1; i < strlen(dst); i++){
            name_str[k] = dst[i];
            k++;
        }
        dst_found=true;
        dst_which = true;
    }else{
        for(i = 0; i < strlen(dst); i++){
            if(dst[i]=='/') {
                slash = true;
                continue;
            }
            if(slash){
                name_str[k]=dst[i];
                k++;
            }else{
                dst_str[j] = dst[i];
                j++;
            }

        }
    }}else{
         for(i = 0; i < strlen(dst); i++){
            name_str[k] = dst[i];
            k++;
        }
    }
    name_str[k]='\0';
    dst_str[j]='\0';
    if(strlen(name_str)>0 && slash_pos_index!=-1){
    if(!dst_found ){
        for (i = 0; i < FS_MAX_NODE; i++) {
            if (strcmp(node_fs_buf.nodes[i].node_name, dst_str) == true &&
                node_fs_buf.nodes[i].parent_index == cwd &&
                node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
                dst_index = i;
                dst_found = true;
                break;
            }
        }
        if (!dst_found) {
            printString("Destination directory not found\n");
            return;
        }else{
            node_fs_buf.nodes[src_index].parent_index = dst_index;
            strcpy(node_fs_buf.nodes[src_index].node_name, name_str);
        }
    }else if(dst_which){
        node_fs_buf.nodes[src_index].parent_index=0xFF;
        strcpy(node_fs_buf.nodes[src_index].node_name, name_str);
    }else{
        if(node_fs_buf.nodes[src_index].parent_index==0xFF){
             printString("Cannot move, currently in root dir\n");
            return;
        }else{
          par_index = node_fs_buf.nodes[src_index].parent_index;
          par_index = node_fs_buf.nodes[par_index].parent_index;
          node_fs_buf.nodes[src_index].parent_index=par_index;
          strcpy(node_fs_buf.nodes[src_index].node_name, name_str);
        }
    }
    }else{
        if(strlen(name_str)<=0 ){
             printString("Command not valid!\n");
        return;
        }else{
            for (i = 0; i < FS_MAX_NODE; i++) {
            if (strcmp(node_fs_buf.nodes[i].node_name, name_str) == true &&
                node_fs_buf.nodes[i].parent_index == cwd) {
                 printString("There is already a file with a same name!\n");
                return;
            }
        }
            node_fs_buf.nodes[i].parent_index == cwd;
            strcpy(node_fs_buf.nodes[src_index].node_name, name_str);
        }

    }


    writeSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    return;
}



// TODO: 9. Implement cp function
void cp(byte cwd, char* src, char* dst) {
    struct node_fs node_fs_buf;
    struct data_fs data_fs_buf;
    struct file_metadata src_metadata;
    enum fs_return status;
    char cwd_str[4];
    int slash_pos_index;
    int dir_len;
    int i;
    int j;
    int k;
    int g;
    int src_index;
    int par_index;
    char dst1[512];
    char dst2[512];
    bool src_found = false;
    bool dst_found = false;
    bool slash = false;
    bool dst_which = false;
    bool dst_found1 =false;
    readSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    clear(src_metadata.buffer, FS_MAX_SECTOR * SECTOR_SIZE);
    dst1[0]='\0';
    dst2[0]='\0';
    strcpy(src_metadata.node_name, src);

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (strcmp(node_fs_buf.nodes[i].node_name, src) && node_fs_buf.nodes[i].parent_index == cwd) {
           if(node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR){
        printString("Cannot copy directory\n");
            return;
     }
             src_index = i;
             src_found=true;
            break;
        }
    }

    for(j=0;j<FS_MAX_SECTOR;j++){
        if(data_fs_buf.datas[node_fs_buf.nodes[src_index].data_index].sectors[j]==0x00){
        break;
    }
        readSector(src_metadata.buffer + j * SECTOR_SIZE,data_fs_buf.datas[node_fs_buf.nodes[src_index].data_index].sectors[j]);
        src_metadata.filesize+=SECTOR_SIZE;

    }
    if(!src_found){
         printString("No such file found\n");
            return;
    }
    slash_pos_index = -1;
    dir_len = strlen(dst);
     for (i = 0; i < dir_len; i++) {
        if (dst[i] == '/') {
            slash_pos_index = i;
            break;
        }
    }
    if(slash_pos_index==-1){
        slash_pos_index=0;
        slash = true;
    }
    j = 0;
    k = 0;
    if(dst[0]=='.'&& dst[1]=='.'&&dst[2]=='/'){
        for(i = 3; i < strlen(dst); i++){
            dst2[k] = dst[i];
            k++;
        }
        dst_found=true;
    }else if(dst[0]=='/'){
        for(i = 1; i < strlen(dst); i++){
            dst2[k] = dst[i];
            k++;
        }
        dst_found=true;
        dst_which = true;
    }else{
        for(i = 0; i < strlen(dst); i++){
            if(dst[i]=='/') {
                slash = true;
                continue;
            }
            if(slash){
                dst2[k]=dst[i];
                k++;
            }else{
                dst1[j] = dst[i];
                j++;
            }

        }
    }
    dst2[k]='\0';
    dst1[j]='\0';

     if(strlen(dst2)<=0 ){
             printString("Command not valid!\n");
        return;
     }

    if(slash_pos_index!=0 && dst_found==false){
     for (i = 0; i < FS_MAX_NODE; i++) {
        if (strcmp(node_fs_buf.nodes[i].node_name, dst1)== true &&
                node_fs_buf.nodes[i].parent_index == cwd &&
                node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR ){
            src_metadata.parent_index = i;
            dst_found1 = true;
            break;
        }
    }
    }else if(slash_pos_index==0 && dst_found==false){
         src_metadata.parent_index = cwd;
            dst_found1 = true;
    }else if(dst_which){
           src_metadata.parent_index = 0xFF;
           dst_found1 = true;
    }else{
        if(node_fs_buf.nodes[src_index].parent_index==0xFF){
             printString("Cannot move, currently in root dir\n");
            return;
        }else{
          par_index = node_fs_buf.nodes[src_index].parent_index;
          par_index = node_fs_buf.nodes[par_index].parent_index;
          src_metadata.parent_index=par_index;
          dst_found1 = true;
        }
    }
    if(!dst_found1){
        printString("No such directory destination\n");
        return;
    }

    strcpy(src_metadata.node_name, dst2);
    fsWrite(&src_metadata, &status);


    if (status != FS_SUCCESS) {
        printString("Failed to write to destination file\n");
    }


}



// TODO: 10. Implement cat function
void cat(byte cwd, char* filename) {
    struct file_metadata metadata;
    enum fs_return status;

    strcpy(metadata.node_name, filename);
    metadata.parent_index = cwd;
    fsRead(&metadata, &status);

    if (status == FS_SUCCESS) {
        printString(metadata.buffer);
        printString("\n");

    } else {
        printString("Failed to read file\n");

    }

}



// TODO: 11. Implement mkdir function

void mkdir(byte cwd, char* dirname) {
    struct map_fs map_fs_buf;
    struct node_fs node_fs_buf;
    enum fs_return status;
    int i;

    if (strlen(dirname) == 0) {
        printString("Directory name cannot be empty\n");
        return;
    }

    readSector(&(map_fs_buf), FS_MAP_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].parent_index == cwd &&
            strcmp(node_fs_buf.nodes[i].node_name, dirname)) {
            printString("Directory already exists\n");
            return;
        }
    }

     for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].node_name[0]=='\0'){
            break;
        }
    }

    node_fs_buf.nodes[i].data_index = FS_NODE_D_DIR;
    node_fs_buf.nodes[i].parent_index = cwd;
    strcpy(node_fs_buf.nodes[i].node_name, dirname);
    map_fs_buf.is_used[i] = true;

    writeSector(&(map_fs_buf), FS_MAP_SECTOR_NUMBER);
    writeSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    printString("Directory created successfully\n");
}

