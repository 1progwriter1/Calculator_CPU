#include <stdio.h>
#include <assert.h>
#include "processor.h"

int main(const int argc, const char *argv[]) {

    assert(argv);

    int file_num = 0;
    if (GetFileName(argc, argv, &file_num) != SUCCESS)
        return ERROR;

    Calc processor = {};
    CalcCtor(&processor);

    if (ExecuteProgram(&processor, argv[file_num]) != SUCCESS)
        return ERROR;

    CalcDtor(&processor);

    return SUCCESS;
}