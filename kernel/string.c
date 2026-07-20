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

/// Copies source into destination buffer till N
char* strncpy(char* dest, const char* src, int n) {
    int i = 0;

    while(src[i] != '\0' && i < n) {
        dest[i] = src[i];
        i++;
    }

    for(; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}

/// Fills a block of memory with a specific byte.
void* memset(void* dest, int val, int n) {
    unsigned char* destination = (unsigned char*)dest;

    for(int i = 0; i < n; i++) destination[i] = (unsigned char)val;

    return destination;
}

/// Copies a portion from src to dest
void* memcpy(void* dest, const void* src, int n) {
    unsigned char* destination = (unsigned char*)dest;
    const unsigned char* source = (const unsigned char*)src;

    for(int i = 0; i < n; i++) destination[i] = source[i];

    return destination;
}
