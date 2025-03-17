#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Hero{
    char nama[1024];
    char role[1024];
    int hp;
    int pa;
    float as;
    int mana;
    float mreg;
    float sp;
    struct Hero *next;
} Hero;

Hero* read(const char *filename, int isAttributeFile) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Unable to open file %s\n", filename);
        return NULL;
    }

    Hero *head = NULL, *tail = NULL;
    char buffer[1024];
    int isFirstLine = 1; 
    if(isAttributeFile){
        while (fgets(buffer, sizeof(buffer), file)) {
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        Hero *newHero = (Hero*)malloc(sizeof(Hero));
        if (!newHero) {
            fprintf(stderr, "Memory allocation failed\n");
            fclose(file);
            return head;
        }
        memset(newHero, 0, sizeof(Hero)); 

        char *token;
        int colIndex = 0;
        token = strtok(buffer, ";");
        while (token != NULL) {
            switch(colIndex) {
                case 0: 
                    strncpy(newHero->nama, token, 1023);
                    break;
                case 1:
                    newHero->hp = atoi(token);
                    break;
                case 3:
                    newHero->pa = atoi(token);
                    break;
                case 5: 
                    newHero->as = atof(token);
                    break;
                case 7: 
                    newHero->mana = atoi(token);
                    break;
                case 8: 
                    newHero->mreg = atof(token);
                    break;
                default:
                    break;
            }
            token = strtok(NULL, ";");
            colIndex++;
        }

        newHero->next = NULL;
        if (head == NULL) {
            head = tail = newHero;
        } else {
            tail->next = newHero;
            tail = newHero;
        }
    }
    }else { 
    while (fgets(buffer, sizeof(buffer), file)) {
        if (isFirstLine) {
            isFirstLine = 0;
            continue;
        }

        Hero *newHero = NULL;
        char *token;
        int colIndex = 0;
        char *roles[2] = {NULL, NULL}; 
        int roleCount = 0;

        token = strtok(buffer, ";");
        while (token != NULL) {
            if (colIndex == 0) {  
                newHero = (Hero*)malloc(sizeof(Hero));
                if (!newHero) {
                    fprintf(stderr, "Memory allocation failed\n");
                    fclose(file);
                    return head;
                }
                memset(newHero, 0, sizeof(Hero));
                strncpy(newHero->nama, token, 1023);
            } else if (colIndex == 1) { 
                char *slash = strchr(token, '/');
                if (slash) {  
                    *slash = '\0'; 
                    roles[0] = token;
                    roles[1] = slash + 1;
                    roleCount = 2;
                } else {
                    roles[0] = token;
                    roleCount = 1;
                }
            }
            token = strtok(NULL, ";");
            colIndex++;
        }

        for (int i = 0; i < roleCount; i++) {
            Hero *heroEntry = i == 0 ? newHero : (Hero*)malloc(sizeof(Hero));
            if (i > 0) {  
                memcpy(heroEntry, newHero, sizeof(Hero));
            }
            strncpy(heroEntry->role, roles[i], 1023); 
            heroEntry->next = NULL;

            if (head == NULL) {
                head = tail = heroEntry;
            } else {
                tail->next = heroEntry;
                tail = heroEntry;
            }
        }
    }
}

    
    fclose(file);
    return head;
}



void calculate(Hero *hero) {
    if (strncmp(hero->role, "TANK", 4) == 0) {
        hero->sp = hero->hp;
    } else if (strncmp(hero->role, "ASSASSIN", 8) == 0) {
        hero->sp = hero->pa;
    } else if (strncmp(hero->role, "FIGHTER", 7) == 0) {
        hero->sp = hero->hp + hero->pa;
    } else if (strncmp(hero->role, "MM", 2) == 0) {
        hero->sp = hero->pa * hero->as;
    } else if (strncmp(hero->role, "MAGE", 4) == 0) {
        hero->sp = hero->mana / hero->mreg;
    } else if (strncmp(hero->role, "SUPPORT", 7) == 0) {
        hero->sp = hero->mreg + hero->hp;
    } else {
        hero->sp = 0;
    }
}

void print(Hero *heroes, char * argu) {
    Hero *bestHeroes[6] = {NULL};
    for (int i = 0; i < 6; i++) {
        bestHeroes[i] = NULL;
    }

    Hero *current = heroes;
    while (current) {
        int roleIndex = -1;
        if (strncmp(current->role, "TANK", 4) == 0) roleIndex = 0;
        else if (strncmp(current->role, "ASSASSIN", 8) == 0) roleIndex = 1;
        else if (strncmp(current->role, "FIGHTER", 7) == 0) roleIndex = 2;
        else if (strncmp(current->role, "MM", 2) == 0) roleIndex = 3;
        else if (strncmp(current->role, "MAGE", 4) == 0) roleIndex = 4;
        else if (strncmp(current->role, "SUPPORT", 7) == 0) roleIndex = 5;

        if (roleIndex != -1) {
            if (bestHeroes[roleIndex] == NULL || bestHeroes[roleIndex]->sp < current->sp) {
                bestHeroes[roleIndex] = current;
            }
        }
        current = current->next;
    }

    for (int i = 0; i < 6; i++) {
        if(argu != NULL){
            if(strcmp(bestHeroes[i]->role, argu)==0){
                printf("Hero dengan role %s adalah %s dengan skill points %.2f\n",
                   bestHeroes[i]->role, bestHeroes[i]->nama, bestHeroes[i]->sp);
                break;
            }
        }else{
            if (bestHeroes[i]) {
            printf("Hero dengan role %s adalah %s dengan skill points %.2f\n",
                   bestHeroes[i]->role, bestHeroes[i]->nama, bestHeroes[i]->sp);
        }
    }
}
}
void free_hero(Hero *list) {
    while (list) {
        Hero *tmp = list;
        list = list->next;
        free(tmp);
    }
}


void download(){
    FILE *file = fopen("Data Mobile Legend", "r");
    if (file) {
        fclose(file);
        return;
    }
    system("kaggle datasets download -d irwansyah10010/mobilelegend");
    system("unzip mobilelegend.zip");
}


int main(int ar, char *arv[]){
    download();
    const char * path2 = "Data Mobile Legend/Atribut/atribut-hero.csv";
    const char * path = "Data Mobile Legend/identitas.csv";
    Hero *heroes = read(path, 0);
    Hero *attributes = read(path2, 1);

 


 Hero *currentAttr = attributes;
    while (currentAttr) {
        Hero *currentHero = heroes;
        while (currentHero) {
            if (strcmp(currentHero->nama, currentAttr->nama) == 0) {
                currentHero->hp = currentAttr->hp;
                currentHero->pa = currentAttr->pa;
                currentHero->as = currentAttr->as;
                currentHero->mana = currentAttr->mana;
                currentHero->mreg = currentAttr->mreg;
            }
            currentHero = currentHero->next;
        }
        currentAttr = currentAttr->next;
    }

     Hero *tempHero = heroes;
    while (tempHero) {
        calculate(tempHero);
        tempHero = tempHero->next;
    }



    if(ar>1){
        print(heroes, arv[1]);
    }else{
        print(heroes, NULL);
    }
    
    


    free_hero(heroes);
    free_hero(attributes);


    return 0;
}
