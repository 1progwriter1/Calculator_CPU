#include <stdio.h>
#include <assert.h>
#include "disassembler.h"

int main(const int argc, const char *argv[]) {

    assert(argv);

    DisasmData data = {};
    if (GetArgsDisasm(argc, argv, &data) != SUCCESS)
        return ERROR;

    if (data.output_file != 0) {
        if (disassembler(argv[1], argv[2]) != SUCCESS)
            return ERROR;
    }
    else {
        if (disassembler(argv[1], disasm_file) != SUCCESS)
            return ERROR;
    }

    return SUCCESS;
}