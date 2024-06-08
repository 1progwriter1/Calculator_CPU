#ifndef CALCULATE_CALC
#define CALCULATE_CALC

#include <stdio.h>
#include "Stack/stack_values.h"
#include "calculator_values.h"

struct ProcData {
    CPU *processor;
    char *buf;
    const char *filename;
    unsigned long len_of_file;
};

enum Result ExecuteProgram(CPU *calcdata, const char *file);
enum Result CPUCtor(CPU *calcdata);
enum Result CPUDtor(CPU *calcdata);

#endif
