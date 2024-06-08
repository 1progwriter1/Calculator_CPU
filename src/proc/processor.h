#ifndef CALCULATE_CALC
#define CALCULATE_CALC

#include <stdio.h>
#include "../stack/stack_values.h"
#include "../lib/calculator_values.h"

struct ProcData {
    CPU *processor;
    char *buf;
    const char *filename;
    unsigned long len_of_file;
};

int ExecuteProgram(CPU *calcdata, const char *file);
int CPUCtor(CPU *calcdata);
int CPUDtor(CPU *calcdata);

#endif
