#ifndef DISASS_CALC
#define DISASS_CALC

#include <stdio.h>
#include "../stack/stack_values.h"
#include "../vector/my_vector.h"

#define disasm_file "dis_file.txt"

struct DisasmData {
    char *buf;
    Vector labels;
    unsigned long len_of_file;
    Elem_t len_of_prog;
    FILE *dst_file;
    const char *byte_code_file;
};

int CodeTranslate(const char *file, const char *dis_file);

#endif
