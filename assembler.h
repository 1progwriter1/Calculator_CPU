#ifndef TRANSLATOR_CALCULATOR
#define TRANSLATOR_CALCULATOR

#include <stdio.h>
#include "Stack/stack_values.h"

#define proc_code_file "byte_code.bin"

struct AsmData {
    int input_file;
    int output_file;
};

enum Result CodeCompile(const char *file, const char *outfile);
int GetArgsAsm(const int argc, const char *argv[], AsmData *data);

 #endif
