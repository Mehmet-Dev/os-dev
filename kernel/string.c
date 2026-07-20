// string.c
// Simple string library

#include "headers/string.h"

/// Compares 2 strings against one another.
int strcmp(const char* s1, const char* s2) {
    int i = 0;
    int same = 0;

    while(1) {
        if(s1[i] != s2[i]) {
            break;
        }

        if(s1[i] == '\0' && s2[i] == '\0') {
            same = 1;
            break;
        }

        i++;
    }

    return !same;
}

/// Compares 2 strings until a set interval
int strncmp(const char* s1, const char* s2, int n) {
    int i = 0;
    int same = 1;

    while(i < n) {
        if(s1[i] != s2[i]) {
            same = 0;
            break;
        }

        if(s1[i] == '\0' && s2[i] == '\0') {
            break;
        }

        i++;

    }

    return !same;
}

// returns the length of a string
unsigned int strlen(const char* s) {
    unsigned int size = 0;

    while(s[size] != '\0') size++;

    return size;
}

/// Copies source tring into the destination buffer
char* strcpy(char* dest, const char* src) {
    int i = 0;

    while(src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';

    return dest;
}


