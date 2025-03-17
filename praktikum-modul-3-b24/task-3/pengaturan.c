#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void create_container(const char *container_name) {
    char command[256];
    snprintf(command, sizeof(command), "docker run -d --name %s -e NAME=%s -v our_volume:/app/data -t dockermessage:v2 tail -f /dev/null", container_name, container_name);
    system(command);
    snprintf(command, sizeof(command), "docker exec %s mkdir /app/data/%s", container_name, container_name);
    system(command);

    char filename[256];
    printf("Container %s created.\n", container_name);
}

void delete_container(const char *container_name) {
    char command[256];

     snprintf(command, sizeof(command), "docker exec %s rm -rf /app/data/%s", container_name, container_name);
    system(command);

    snprintf(command, sizeof(command), "docker rm -f %s", container_name);
    system(command);
   
    
    printf("Container %s deleted.\n", container_name);
}

void list_containers() {
    system("docker ps -a");
}

void login_container(const char *container_name) {
    char command[256];
    snprintf(command, sizeof(command), "docker exec -it %s /bin/sh", container_name);
    system(command);
}

void connect_containers(const char *container1, const char *container2) {
    char command[256];  


    snprintf(command, sizeof(command), "docker exec %s mkdir -p /app/data/connection", container1);
    system(command);
  
    snprintf(command, sizeof(command), "docker exec %s mkdir -p /app/data/connection", container2);
    system(command);
    
    snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/connection/%s%s.txt", container1, container1, container2);
    system(command);

      snprintf(command, sizeof(command), "docker exec  %s touch /app/data/connection/%s%s.txt", container2, container2, container1);
    system(command);

     snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/%s/%s.txt", container1, container1, container2);
    system(command);

    snprintf(command, sizeof(command), "docker exec  %s touch  /app/data/%s/%s.txt", container2, container2, container1);
    system(command);
  
}

void disconnect_containers(const char *container1, const char *container2) {
    char command[256];

    snprintf(command, sizeof(command), "docker exec %s rm -f /app/data/connection/%s%s.txt", container1, container1, container2);
    system(command);

    snprintf(command, sizeof(command), "docker exec %s rm -f /app/data/connection/%s%s.txt", container2, container2, container1);
    system(command);


}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [container_name]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "create") == 0 && argc == 3) {
        create_container(argv[2]);
    } else if (strcmp(argv[1], "delete") == 0 && argc == 3) {
        delete_container(argv[2]);
    } else if (strcmp(argv[1], "list") == 0) {
        list_containers();
    } else if (strcmp(argv[1], "login") == 0 && argc == 3) {
        login_container(argv[2]);
    }else if (strcmp(argv[1], "connect") == 0 && argc == 4) {
        connect_containers(argv[2], argv[3]);
    }else if (strcmp(argv[1], "unconnect") == 0 && argc == 4) {
         disconnect_containers(argv[2], argv[3]);
    }else {
        fprintf(stderr, "Invalid command or arguments\n");
        return 1;
    }

    return 0;
}
