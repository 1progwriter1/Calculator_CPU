#ifndef TRANSLATOR_CALCULATOR
#define TRANSLATOR_CALCULATOR

#include <stdio.h>
#include "../stack/stack_values.h"
#include "../labels/labels.h"
#include "../vector/my_vector.h"

#define byte_code "byte_code.bin"

struct AsmData {
    Vector buf;
    Labels lbls;
    FILE *code_file;
    FILE *byte_code_file;
};

int CodeCompile(const char *file, const char *outfile);

#endif
