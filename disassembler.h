#ifndef DISASS_CALC
#define DISASS_CALC

#include <stdio.h>
#include "Stack/stack_values.h"

#define disassembler_file "dis_file.txt"

struct DisasmData {
    int input_file;
    int output_file;
};

enum Result CodeTranslate(const char *file, const char *dis_file);
int GetArgsDisasm(const int argc, const char *argv[], DisasmData *data);

#endif
