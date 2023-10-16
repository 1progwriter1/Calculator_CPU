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

    assembler(input_file);

    Calc example = {};
    CalcCtor(&example);

    Calculate(&example, byte_code);



    disassembler(byte_code);
    /* const double PI = 3.14159265;
    printf("%lg", (int) sin ((double) 30000 / (int) pow (10, 3) * PI / 180)); */

    return SUCCESS;

}
