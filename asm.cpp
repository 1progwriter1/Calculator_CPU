#include <stdio.h>
#include <assert.h>
#include "assembler.h"

int main(const int argc, const char *argv[]) {

    assert(argv);

    AsmData data = {};
    if (GetArgsAsm(argc, argv, &data) != SUCCESS)
        return ERROR;

    if (data.output_file != 0) {
        if (assembler(argv[1], argv[2]) != SUCCESS)
            return ERROR;
    }
    else {
        if (assembler(argv[1], proc_code_file) != SUCCESS)
            return ERROR;
    }

    return SUCCESS;
}