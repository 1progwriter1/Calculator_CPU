#ifndef CALC_VALUES
#define CALC_VALUES

#define READ "r"
#define WRITE "w"

#define input_file "expression.txt"
#define byte_code "byte_code.txt"
#define disassem_file "dis_file.txt"

#include "Stack/stack_values.h"

const int LEN_COMMAND = 4;
const int NUM_OF_REG = 4;

const int NUM_OF_COMMANDS = 11;
const int Push_r = 11;
const int Pop_r = 12;
const char commands[11][5] = {"sub", "div", "out", "hlt", "add", "mul", "sqrt", "sin", "cos", "in", "push"};

enum Commands {
    SUB,
    DIV,
    OUT,
    HLT,
    ADD,
    MUL,
    SQRT,
    SIN,
    COS,
    IN,
    PUSH,
    PUSH_R,
    POP_R
};

struct Calc {
    Stack data;
    Elem_t *reg;
};

#endif
