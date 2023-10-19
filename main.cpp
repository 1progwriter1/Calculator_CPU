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

    return SUCCESS;

}
