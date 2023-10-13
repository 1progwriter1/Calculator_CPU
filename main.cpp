#include <stdio.h>
#include "Stack/stack.h"
#include "calculator_values.h"
#include "func.h"
#include <assert.h>
#include "assembler.h"
#include "processor.h"
#include "disassembler.h"
#include <math.h>

int main() {

    FILE *fn = fileopen(input_file, READ);
    translator(fn);
    fileclose(fn);

    Calc example = {};
    CalcCtor(&example);

    Calculate(&example, byte_code);



    disassembler(byte_code);
    /* const double PI = 3.14159265;
    printf("%lg", sin ((double) 30 / (int) pow (10, 0) * PI / 180)); */

    return SUCCESS;

}
