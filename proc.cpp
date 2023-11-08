#include <stdio.h>
#include <assert.h>
#include "processor.h"

int main(const int argc, const char *argv[]) {

    assert(argv);

    if (argc < 2) {
        printf(RED "File name expected" END_OF_COLOR "\n");
        return ERROR;
    }
    const char *execute_file = argv[1];

    CPU processor = {};
    if (CPUCtor(&processor) != SUCCESS) {
        printf(RED "CPU creation failed" END_OF_COLOR "\n");
        return ERROR;
    }

    if (ExecuteProgram(&processor, execute_file) != SUCCESS)
        return ERROR;

    CPUDtor(&processor);

    return SUCCESS;
}