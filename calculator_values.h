#ifndef CALC_VALUES
#define CALC_VALUES

#define READ "r"
#define WRITE "w"

#define input_file "expression.txt"
#define byte_code "byte_code.bin"
#define disassem_file "dis_file.txt"

#include "Stack/stack_values.h"

const int LEN_COMMAND_PUSH = 4;
const int LEN_COMMAND_POP_R = 5;
const int NUM_OF_REGS = 4;
const int REG_LEN = 3;
const int MAX_STRLEN = 10;
const int MY_SIGN = 'V' + 'L' + 'I';

const char regs[NUM_OF_REGS][4] = {"rax", "rbx", "rcx", "rdx"};

const int NUM_OF_COMMANDS = 12;
const char commands[NUM_OF_COMMANDS + 1][6] = {"sub", "div", "out", "hlt", "add", "mul", "sqrt", "sin", "cos", "in", "push", "pop", "push"};

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
    POP,
    PUSH_R,
};

struct Calc {
    Stack data;
    Elem_t *reg;
};

#endif
