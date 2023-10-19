#ifndef FUNC_CALCULATOR
#define FUNC_CALCULATOR

#include <stdio.h>

FILE *fileopen(const char *filename, const char mode[]);
long long int filelen(const char *filename);
char *readfile(FILE *fn, long long int length);
int FileVerify(const int sign, const int version_icon, const int version_given);
int DayNumber();
void SetLabel(const int label_number, const int address);
int GetLabel(const int label_number);

#endif
