#ifndef TRANSLATOR_CALCULATOR
#define TRANSLATOR_CALCULATOR

#include <stdio.h>
#include "Stack/stack_values.h"
#include "labels.h"
#include "my_vector.h"

#define byte_code "byte_code.bin"

struct AsmData {
    Vector buf;
    Labels lbls;
    FILE *code_file;
    FILE *byte_code_file;
};

enum Result CodeCompile(const char *file, const char *outfile);

#endif
