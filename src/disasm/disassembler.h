#ifndef DISASS_CALC
#define DISASS_CALC

#include <stdio.h>
#include "Stack/stack_values.h"
#include "my_vector.h"

#define disasm_file "dis_file.txt"

struct DisasmData {
    char *buf;
    Vector labels;
    unsigned long len_of_file;
    Elem_t len_of_prog;
    FILE *dst_file;
    const char *byte_code_file;
};

enum Result CodeTranslate(const char *file, const char *dis_file);

#endif
