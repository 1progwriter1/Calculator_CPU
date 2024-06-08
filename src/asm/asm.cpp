#include <stdio.h>
#include <assert.h>
#include "assembler.h"
#include "../lib/calculator_values.h"

int main(const int argc, const char *argv[]) {

    assert(argv);

    const char *src = NULL;
    const char *dst = byte_code;

    if (argc < 2) {
        printf(RED "File name expected" END_OF_COLOR "\n");
        return ERROR;
    }
    if (argc > 2) {
        src = argv[1];
        dst = argv[2];
    }
    else {
        src = argv[1];
    }

    if (CodeCompile(src, dst) != SUCCESS) {
        return ERROR;
    }

    return SUCCESS;
}

