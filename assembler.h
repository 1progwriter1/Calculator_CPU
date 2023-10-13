#ifndef TRANSLATOR_CALCULATOR
#define TRANSLATOR_CALCULATOR

#include <stdio.h>
#include "Stack/stack_values.h"

enum Result translator(FILE *fn);
int RegIndex(char *namereg);
int CommandNumber(char *line);

 #endif
