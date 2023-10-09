#ifndef FUNC_CALCULATOR
#define FUNC_CALCULATOR

#include <stdio.h>

FILE *fileopen(const char *filename, const char mode[]);
long long int filelen(const char *filename);
char *readfile(FILE *fn, long long int length);
int FileVerify(FILE *fn, const char *sign, const int version);

#endif
