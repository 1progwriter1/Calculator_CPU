#include <stdio.h>
#include <assert.h>
#include "disassembler.h"
#include "calculator_values.h"

int main(const int argc, const char *argv[]) {

    assert(argv);

    const char *src = NULL;
    const char *dst = disasm_file;

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

    if (CodeTranslate(src, dst) != SUCCESS) {
        return ERROR;
    }

    return SUCCESS;
}