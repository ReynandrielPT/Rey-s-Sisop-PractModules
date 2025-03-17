#include "std_lib.h"

#define INT_MIN -2147483648
#define INT_MAX 2147483647

int div(int a, int b) {
    int sign, quo;
    if (b == 0) return INT_MAX;
    
    sign = ((a < 0) ^ (b < 0)) ? -1 : 1;

    a = (a < 0 ? -a : a);
    b = (b < 0 ? -b : b);

    quo = 0;
    while (a >= b) {
        int temp = b, cnt = 1;
        while (temp <= (b >> 1)) {
            temp <<= 1;
            cnt <<= 1;
        }
        a -= temp;
        quo += cnt;
    }

    return sign * quo;
}

int mod(int a, int b) {
    if (b == 0) return INT_MAX;

    a = (a < 0 ? -a : a);
    b = (b < 0 ? -b : b);

    while (a >= b) {
        int temp = b;
        while (temp <= (b >> 1)) {
            temp <<= 1;
        }
        a -= temp;
    }

    return a;
}

void memcpy(byte* src, byte* dst, unsigned int size) {
    int i;
    for (i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

unsigned int strlen(char *str) {
    unsigned int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

bool strcmp(char* str1, char* str2) {
    int i;
    unsigned int len1 = strlen(str1);
    unsigned int len2 = strlen(str2);
    if (len1 != len2) {
        return false;
    }
    for (i = 0; i < len1; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

void strcpy(char* src, char* dst) {
    unsigned int len = strlen(src);
    int i;
    for (i = 0; i < len; i++) {
        dst[i] = src[i];
    }
    dst[len] = '\0';
}

void clear(byte* buf, unsigned int size) {
    int i;
    for (i = 0; i < size; i++) {
        buf[i] = 0;
    }
}