#ifndef FUNC_CALCULATOR
#define FUNC_CALCULATOR

#include <stdio.h>
#include "calculator_values.h"

FILE *fileopen(const char *filename, const char mode[]);
unsigned long filelen(const char *filename);
char *readfile(FILE *fn, unsigned long length);
int FileVerify(const int sign, const int version_icon, const int version_given);
int DayNumber();
int CheckArgs(const char args, const char arg);
int MainArgs(int argc, const char *argv[], maindata *data);

#endif
