#ifndef TRANSLATOR_CALCULATOR
#define TRANSLATOR_CALCULATOR

#include <stdio.h>
#include "Stack/stack_values.h"

enum Result assembler(const char *file, const char *outfile);
int RegIndex(char *namereg);

 #endif
