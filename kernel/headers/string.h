#ifndef STRING_H
#define STRING_H

int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, int n);
unsigned int strlen(const char* s);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, int n);
void* memset(void* dest, int val, int n);
void* memcpy(void* dest, const void* src, int n);

#endif
