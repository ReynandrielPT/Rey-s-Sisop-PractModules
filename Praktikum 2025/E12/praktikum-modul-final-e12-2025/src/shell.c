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

// TODO: 4. Implement printCWD function
void printCWD(byte cwd) {
  struct node_fs node_fs_buf;
  char path[FS_MAX_NODE * MAX_FILENAME];
  int i, length;
  int idx = cwd;
  int pos = 0;

  if (cwd == FS_NODE_P_ROOT) {
    printString("/");
    return;
  }
  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  while (idx != FS_NODE_P_ROOT && pos < sizeof(path) - MAX_FILENAME) {
    length = strlen(node_fs_buf.nodes[idx].node_name);
   
    for (i = pos + length + 1; i > length; i--) path[i] = path[i - length - 1];
    path[length] = '/';
   
    for (i = 0; i < length; i++) path[i] = node_fs_buf.nodes[idx].node_name[i]; 
    pos += length + 1;
    idx = node_fs_buf.nodes[idx].parent_index;
  }

  for (i = pos; i > 0; i--) path[i] = path[i - 1];
  path[0] = '/';
  pos++;
  path[pos] = '\0';

  printString(path);
}

// TODO: 5. Implement parseCommand function
void parseCommand(char* buf, char* cmd, char arg[2][64]) {
  int i = 0, j = 0, k = 0;
  
  for (i = 0; i < 64; i++) {
    cmd[i] = '\0';
    arg[0][i] = '\0';
    arg[1][i] = '\0';
  }

  i = 0;j = 0;k = 0; 
  
  while (buf[i] != '\0' && buf[i] != ' ' && j < 63) cmd[j++] = buf[i++];
  cmd[j] = '\0';

  while (buf[i] == ' ') i++;

  while (buf[i] != '\0' && k < 2) {
    j = 0;
    while (buf[i] != '\0' && buf[i] != ' ' && j < 63) arg[k][j++] = buf[i++];
    arg[k][j] = '\0';
    k++;
    while (buf[i] == ' ') i++;
  }
}

// TODO: 6. Implement cd function
void cd(byte* cwd, char* dirname) {
  struct node_fs node_fs_buf;
  enum fs_return status;

  int i;

  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  if (strcmp(dirname, "/") == 0) {
    *cwd = FS_NODE_P_ROOT; 
    return;
  }
  if (strcmp(dirname, "..") == 0) {
    if (*cwd != FS_NODE_P_ROOT) {
      *cwd = node_fs_buf.nodes[*cwd].parent_index;
    }
    return;
  }
  for (i = 0; i < FS_MAX_NODE; i++) {
    if (node_fs_buf.nodes[i].parent_index == *cwd && strcmp(node_fs_buf.nodes[i].node_name, dirname) == 0) {
      if (node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
        *cwd = i;
        return;
      }
    }
  }
}

// TODO: 7. Implement ls function
void ls(byte cwd, char* dirname) {
  struct node_fs node_fs_buf;
  int i;
  byte target_dir = cwd; 

  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  if (strlen(dirname) > 0 && strcmp(dirname, ".") != 0) {
    int found = -1;
    for (i = 0; i < FS_MAX_NODE; i++) {
      if (node_fs_buf.nodes[i].parent_index == cwd &&
          strcmp(node_fs_buf.nodes[i].node_name, dirname) == 0 &&
          node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
        found = i;
        break;
      }
    }
    if(found != -1) target_dir = found;
  }

  for (i = 0; i < FS_MAX_NODE; i++) {
    if (node_fs_buf.nodes[i].parent_index == target_dir) {
      printString(node_fs_buf.nodes[i].node_name);
      printString("\n");
    }
  }
}

// TODO: 8. Implement mv function
void mv(byte cwd, char* src, char* dst) {
    struct node_fs node_fs_buf;
    int i;
    int j;
    int src_index = -1;
    byte dest_parent = 0;
    char dest_name[MAX_FILENAME];
    int found_dir = -1;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].parent_index == cwd && strcmp(node_fs_buf.nodes[i].node_name, src) == 0) {
            src_index = i;
            break;
        }
    }
    for (i = 0; i < MAX_FILENAME; i++) dest_name[i] = 0;

    if (dst[0] == '/') {
        dest_parent = FS_NODE_P_ROOT;
        for (i = 1; i < MAX_FILENAME && dst[i] != 0; i++) {
            dest_name[i - 1] = dst[i];

        }
    } else if (dst[0] == '.' && dst[1] == '.' && dst[2] == '/') {
        dest_parent = node_fs_buf.nodes[cwd].parent_index;
        for (i = 3; i < MAX_FILENAME && dst[i] != 0; i++) {
            dest_name[i - 3] = dst[i];
        }
    } else {
        int slash_pos = -1;
        for (i = 0; i < MAX_FILENAME; i++) {
            if (dst[i] == '/') {
                slash_pos = i;
                break;
            }
            if (dst[i] == 0) break;
        }
        if (slash_pos == -1) {
            dest_parent = cwd;
            for (i = 0; i < MAX_FILENAME && dst[i] != 0; i++) {
                dest_name[i] = dst[i];

            }
        } else {
            char dirname[64];
            for (i = 0; i < slash_pos; i++) {
                dirname[i] = dst[i];
            }
            dirname[slash_pos] = 0;

            found_dir = -1;
            for (i = 0; i < FS_MAX_NODE; i++) {
                if (node_fs_buf.nodes[i].parent_index == cwd &&
                    strcmp(node_fs_buf.nodes[i].node_name, dirname) == 0 && node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
                    found_dir = i;
                    break;
                }
            }
            dest_parent = found_dir;
            j = 0;
            for (i = slash_pos + 1; i < MAX_FILENAME && dst[i] != 0; i++, j++) dest_name[j] = dst[i];
        }
    }
    node_fs_buf.nodes[src_index].parent_index = dest_parent;
    for (i = 0; i < MAX_FILENAME; i++) node_fs_buf.nodes[src_index].node_name[i] = dest_name[i];


    writeSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
}


// TODO: 9. Implement cp function
void cp(byte cwd, char* src, char* dst) {
  struct node_fs node_fs_buf;
    struct data_fs data_fs_buf;
    struct file_metadata metadata;
    struct file_metadata new_metadata;
    enum fs_return status;

    int i, j;
    int src_index = -1;
    byte dest_parent = 0;
    char dest_name[MAX_FILENAME];
    int found_dir = -1;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    readSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
    //copy dr mv aja
    for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].parent_index == cwd &&
            strcmp(node_fs_buf.nodes[i].node_name, src) == 0) {
            src_index = i;
            break;
        }
    }
    for (i = 0; i < MAX_FILENAME; i++) dest_name[i] = 0;

    if (dst[0] == '/') {
        dest_parent = FS_NODE_P_ROOT;
        for (i = 1; i < MAX_FILENAME && dst[i] != 0; i++) {
            dest_name[i - 1] = dst[i];
        }
    } else if (dst[0] == '.' && dst[1] == '.' && dst[2] == '/') {
        dest_parent = node_fs_buf.nodes[cwd].parent_index;
        for (i = 3; i < MAX_FILENAME && dst[i] != 0; i++) {
            dest_name[i - 3] = dst[i];
        }
    } else {
        int slash_pos = -1;
        for (i = 0; i < MAX_FILENAME; i++) {
            if (dst[i] == '/') {
                slash_pos = i;
                break;
            }
            if (dst[i] == 0) break;
        }
        if (slash_pos == -1) {
            dest_parent = cwd;
            for (i = 0; i < MAX_FILENAME && dst[i] != 0; i++) {
                dest_name[i] = dst[i];
            }
        } else {
            char dirname[MAX_FILENAME];
            for (i = 0; i < slash_pos; i++) dirname[i] = dst[i];
            dirname[slash_pos] = 0;

            found_dir = -1;
            for (i = 0; i < FS_MAX_NODE; i++) {
                if (node_fs_buf.nodes[i].parent_index == cwd &&
                    strcmp(node_fs_buf.nodes[i].node_name, dirname) == 0 &&
                    //node_fs_buf.nodes[i].satan_index == FS_NODRE_D_DIR) {
                    node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
                    found_dir = i;
                    break;
                }
            }
            dest_parent = found_dir;
            j = 0;
            for (i = slash_pos + 1; i < MAX_FILENAME && dst[i] != 0; i++, j++) {
                dest_name[j] = dst[i];
            }
            dest_name[j] = 0;
        }
    }
    metadata.parent_index = cwd;
    metadata.filesize = 0;
    for (i = 0; i < MAX_FILENAME; i++) {
        metadata.node_name[i] = src[i];
        if (src[i] == '\0') break;
    }

    fsRead(&metadata, &status);

    new_metadata.parent_index = dest_parent;
    new_metadata.filesize = metadata.filesize;

    for (i = 0; i < MAX_FILENAME; i++) {
        new_metadata.node_name[i] = dest_name[i];
        if (dest_name[i] == '\0') break;
    }
    for (i = 0; i < FS_MAX_SECTOR * SECTOR_SIZE; i++) {
        new_metadata.buffer[i] = metadata.buffer[i];
    }

    fsWrite(&new_metadata, &status);
}

// TODO: 10. Implement cat function
//buatin cat nya gus
void cat(byte cwd, char* filename) {
  struct file_metadata metadata;
  enum fs_return status;

  int i;

  metadata.parent_index = cwd;
  metadata.filesize = 0;
  strcpy(metadata.node_name, filename);

  fsRead(&metadata, &status);

  for (i = 0; i < metadata.filesize; i++) {
    printChar(metadata.buffer[i]);
  }
  printString("\n");
}

// TODO: 11. Implement mkdir function
void mkdir(byte cwd, char* dirname) {
  struct node_fs node_fs_buf;
  enum fs_return status;
  int i;
  int free_node;

  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

  free_node = -1;
  for (i = 0; i < FS_MAX_NODE; i++) {
      if (node_fs_buf.nodes[i].node_name[0] == 0x00) {
          free_node = i;
          break;
      }
  }
  node_fs_buf.nodes[free_node].parent_index = cwd;
  node_fs_buf.nodes[free_node].data_index = FS_NODE_D_DIR;
    for (i = 0; i < MAX_FILENAME; i++) {
      if (dirname[i] == 0) {
            node_fs_buf.nodes[free_node].node_name[i] = 0;
            break;
        }
        node_fs_buf.nodes[free_node].node_name[i] = dirname[i];
    }
  writeSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  writeSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

}




/*
   _____   _____    _____    ____    _____      __  __   ______   _   _  __     __  ______   _   _              _   _    _____   _  __             _   _        __  
  / ____| |_   _|  / ____|  / __ \  |  __ \    |  \/  | |  ____| | \ | | \ \   / / |  ____| | \ | |     /\     | \ | |  / ____| | |/ /     /\     | \ | |    _  \ \ 
 | (___     | |   | (___   | |  | | | |__) |   | \  / | | |__    |  \| |  \ \_/ /  | |__    |  \| |    /  \    |  \| | | |  __  | ' /     /  \    |  \| |   (_)  | |
  \___ \    | |    \___ \  | |  | | |  ___/    | |\/| | |  __|   | . ` |   \   /   |  __|   | . ` |   / /\ \   | . ` | | | |_ | |  <     / /\ \   | . ` |        | |
  ____) |  _| |_   ____) | | |__| | | |        | |  | | | |____  | |\  |    | |    | |____  | |\  |  / ____ \  | |\  | | |__| | | . \   / ____ \  | |\  |    _   | |
 |_____/  |_____| |_____/   \____/  |_|        |_|  |_| |______| |_| \_|    |_|    |______| |_| \_| /_/    \_\ |_| \_|  \_____| |_|\_\ /_/    \_\ |_| \_|   (_)  | |
                                                                                                                                                                /_/                                                                                                                                                                   
*/
