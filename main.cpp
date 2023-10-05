#include <stdio.h>
#include "Stack/stack.h"
#include "calculator_values.h"
#include "func.h"
#include <assert.h>
#include "translator.h"
#include "calculate.h"
#include "disassembler.h"

int main() {

    FILE *fn = fileopen(input_file, READ);

    fileclose(translator(fn));
    fileclose(fn);
    FILE *expl = fileopen(byte_code, READ);
    Stack example = {};
    STACK_CTOR(example);

    Calculate(&example, expl);

    fileclose(expl);

    /* FILE *fn2 = fileopen(byte_code, READ);
    disassembler(fn2); */
    //fileclose(fn2);
}