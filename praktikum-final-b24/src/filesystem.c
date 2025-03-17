#include "kernel.h"
#include "std_lib.h"
#include "filesystem.h"

void fsInit() {
  struct map_fs map_fs_buf;
  int i = 0;

  readSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
  
  for (i = 0; i < 16; i++) map_fs_buf.is_used[i] = true;
  for (i = 256; i < 512; i++) map_fs_buf.is_used[i] = true;
  writeSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);

}


void fsRead(struct file_metadata* metadata, enum fs_return* status) {
    struct node_fs node_fs_buf;
    struct data_fs data_fs_buf;
    int i, j;
    bool file_found = false;

    readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
    readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

    for (i = 0; i < FS_MAX_NODE; i++) {
        if (strcmp(node_fs_buf.nodes[i].node_name, metadata->node_name) && 
            node_fs_buf.nodes[i].parent_index == metadata->parent_index) {
            file_found = true;
            break;
        }
    }


    if (!file_found) {
        *status = FS_R_NODE_NOT_FOUND;
        return;
    }

    if (node_fs_buf.nodes[i].data_index == FS_NODE_D_DIR) {
        *status = FS_R_TYPE_IS_DIRECTORY;
        return;
    }

    readSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
    metadata->filesize = 0;

    for (j = 0; j < FS_MAX_SECTOR; j++) {
        if (data_fs_buf.datas[node_fs_buf.nodes[i].data_index].sectors[j] != 0) {
            readSector(metadata->buffer + (j * SECTOR_SIZE), data_fs_buf.datas[node_fs_buf.nodes[i].data_index].sectors[j]);
            metadata->filesize += SECTOR_SIZE;
        } else {
            break;
        }
    }
    *status = FS_SUCCESS;
}

void fsWrite(struct file_metadata* metadata, enum fs_return* status) {
  struct map_fs map_fs_buf;
  struct node_fs node_fs_buf;
  struct data_fs data_fs_buf;
  int i;
  int empty_node_found;
  int b = (metadata->filesize + SECTOR_SIZE - 1)/SECTOR_SIZE;
  int c;
  int count=0;

  readSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  readSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  readSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);
  

  empty_node_found = -1;
    for (i = 0; i < FS_MAX_NODE; i++) {
        if (node_fs_buf.nodes[i].node_name[0]=='\0') {
            empty_node_found = i;
            break;
        }
    }

  if (empty_node_found == -1) {
    *status = FS_W_NO_FREE_NODE;
    return;
  }

  for(i = 0; i < 32; i++){
    if(data_fs_buf.datas[i].sectors[0]==0x00){
      c = i;
      break;
    }
  }
  strcpy(node_fs_buf.nodes[empty_node_found].node_name, metadata->node_name);
  node_fs_buf.nodes[empty_node_found].parent_index = metadata->parent_index;
  node_fs_buf.nodes[empty_node_found].data_index = c;
  for (i=16; i<256; i++) {
    if(count>=b){
      break;
    }
    if (map_fs_buf.is_used[i] == false) {
      map_fs_buf.is_used[i] = true;
      data_fs_buf.datas[c].sectors[count]=i;
      writeSector(metadata->buffer +  count * SECTOR_SIZE, i);
      count++;
    }
  }

  writeSector(&data_fs_buf, FS_DATA_SECTOR_NUMBER);
  writeSector(&(node_fs_buf.nodes[0]), FS_NODE_SECTOR_NUMBER);
  writeSector(&(node_fs_buf.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
  writeSector(&map_fs_buf, FS_MAP_SECTOR_NUMBER);

  *status = FS_SUCCESS;
}
