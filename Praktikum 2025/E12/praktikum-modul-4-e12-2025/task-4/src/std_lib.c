#include "std_lib.h"


unsigned int strlen(char* str) {
    unsigned int len = 0;
    while (*str) {  
        len++;
        str++;
    }
    return len;
}


bool strcmp(char* str1, char* str2) {
    while (*str1 && (*str1 == *str2)) {  
        str1++;
        str2++;
    }
    return *str1 == *str2;
}


void strcpy(char* src, char* dst) {
    while (*src) {  
        *dst = *src;  
        src++;
        dst++;
    }
    *dst = '\0';  
}


void memcpy(byte* src, byte* dst, unsigned int size) {
    while (size--) { 
        *dst = *src;
        src++;
        dst++;
    }
}
